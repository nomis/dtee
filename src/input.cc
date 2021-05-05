/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021  Simon Arlott

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "input.h"

#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "application.h"
#include "i18n.h"
#include "platform.h"
#include "print_error.h"
#include "temp_directory.h"

using ::boost::asio::buffer;
using ::boost::asio::io_service;
using ::boost::asio::local::datagram_protocol;
using ::boost::format;
using ::boost::system::error_code;
using ::std::bind;
using ::std::max;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace p = ::std::placeholders;

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

namespace dtee {

Input::Input(shared_ptr<boost::asio::io_service> io, shared_ptr<Dispatch> output)
		: io_(io),
		  input_(*io_),
		  out_(*io_),
		  err_(*io_),
		  output_(output) {
}

bool Input::open() {
	// There is no equivalent of mkstemp() for local sockets,
	// so we create them in a temporary directory instead.
	TempDirectory temp_dir{"I"};

	if (!temp_dir.valid()) {
		return false;
	}

	const string input_name = temp_dir.register_file("i");
	const datagram_protocol::endpoint input_ep{input_name};

	const string out_name = temp_dir.register_file("o");
	out_ep_ = datagram_protocol::endpoint{out_name};

	const string err_name = temp_dir.register_file("e");
	err_ep_ = datagram_protocol::endpoint{err_name};

	// Ensure the receive buffer is large enough at least as large as both
	// PIPE_BUF (for pipe writes) and BUFSIZ (for file writes)
	const int MINIMUM_RCVBUF_SIZE = max(PIPE_BUF, max(BUFSIZ, platform::MINIMUM_RCVBUF_SIZE));

	datagram_protocol::socket::receive_buffer_size so_rcvbuf;

	try {
		input_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		input_.bind(input_ep);

		if (!platform::Hurd) {
			input_.get_option(so_rcvbuf);

			if (so_rcvbuf.value() < MINIMUM_RCVBUF_SIZE) {
				so_rcvbuf = MINIMUM_RCVBUF_SIZE;
				input_.set_option(so_rcvbuf);
				input_.get_option(so_rcvbuf);
			}
		} else {
			so_rcvbuf = MINIMUM_RCVBUF_SIZE;
		}
	} catch (std::exception &e) {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("input socket: %1%")), e);
		return false;
	}

	int buffer_size = so_rcvbuf.value();

	if (platform::Linux) {
		// From SOCKET(7): "The kernel doubles this value (to allow space for bookkeeping overhead)"
		// From Boost (1.62): "Linux puts additional stuff into the
		//     buffers so that only about half is actually available to the application.
		//     The retrieved value is divided by 2 here to make it appear as though the
		//     correct value has been set."
		//
		// Boost is wrong because the kernel is not specified as using half the buffer size.
		// It would be simpler if the kernel didn't return its doubled value either.
		// For a 208KB receive buffer, Linux (4.13) uses less than 1KB on x86_64.
		buffer_size *= 2;
	}

	if (buffer_size < MINIMUM_RCVBUF_SIZE) {
		buffer_size = MINIMUM_RCVBUF_SIZE;
	}

	buffer_.resize(buffer_size);

	datagram_protocol::socket::send_buffer_size so_sndbuf{so_rcvbuf.value()};

	try {
		open_output(input_, input_ep, out_, out_ep_, so_sndbuf);
	} catch (std::exception &e) {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("stdout socket: %1%")), e);
		return false;
	}

	try {
		open_output(input_, input_ep, err_, err_ep_, so_sndbuf);
	} catch (std::exception &e) {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("stderr socket: %1%")), e);
		return false;
	}

	if (platform::OpenBSD || platform::Hurd || platform::Cygwin) {
		if (out_ep_ == err_ep_) {
			// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
			// so they both look the same.
			print_error(format(_( // i18n: %1 = socket path; %2 = socket path
				"output socket endpoints are indistinguishable:\n"
				"\tstdout socket: %1%\n\tstderr socket: %2%"))
				% out_ep_.path() % err_ep_.path());
			return false;
		}
	}

	return true;
}

void Input::open_output(datagram_protocol::socket &input,
		const datagram_protocol::endpoint &input_ep,
		datagram_protocol::socket &output,
		datagram_protocol::endpoint &output_ep,
		const datagram_protocol::socket::send_buffer_size &so_sndbuf) {
	output.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
	output.bind(output_ep);

	if (platform::OpenBSD || platform::Hurd) {
		// Workaround Boost.Asio (1.66.0) bug on OpenBSD 6.4
		// Endpoint paths can't be compared correctly if they weren't both
		// constructed by the application or both returned by the OS.
		// https://github.com/boostorg/asio/issues/161
		//
		// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
		// so they both look the same. Retrieve the address after binding so that
		// this can be checked.
		output_ep = output.local_endpoint();
	}

	output.connect(input_ep);
	output.shutdown(datagram_protocol::socket::shutdown_receive);

	if (!platform::Hurd) {
		// Not supported on GNU (Hurd 0.9, Mach 1.8)
		output.set_option(so_sndbuf);
	}

	if (platform::Cygwin) {
		// On Cygwin, getsockname() does not return the same value as
		// recvfrom() or getpeername() does for the other socket.
		// Send an empty message to obtain the real socket address.
		vector<char> empty;
		output.send(buffer(empty));
		input.receive_from(buffer(empty), output_ep);
	}
}

void Input::close_outputs() {
	try {
		out_.close();
	} catch (std::exception &e) {
		// i18n: %1 = errno message
		print_error(format(_("stdout socket: %1%")), e);
	}

	try {
		err_.close();
	} catch (std::exception &e) {
		// i18n: %1 = errno message
		print_error(format(_("stderr socket: %1%")), e);
	}
}

void Input::fork_parent() {
	close_outputs();
}

void Input::start() {
	input_.async_receive_from(buffer(buffer_), recv_ep_,
			bind(&Input::handle_receive_from, this, p::_1, p::_2));
}

bool Input::stop() {
	return !io_error_;
}

void Input::fork_child() {
	errno = 0;
	if (::dup2(out_.native_handle(), STDOUT_FILENO) < 0) {
		auto errno_copy = errno;
		// i18n: %1 = system call name; %2 = errno message
		print_system_error(format(_("stdout: %1%: %2%")) % "dup2", errno_copy);
	}

	errno = 0;
	if (::dup2(err_.native_handle(), STDERR_FILENO) < 0) {
		auto errno_copy = errno;
		// i18n: %1 = system call name; %2 = errno message
		print_system_error(format(_("stderr: %1%: %2%")) % "dup2", errno_copy);
	}

	try {
		input_.close();
	} catch (std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("input socket: %1%")), e);
	}

	close_outputs();
}

void Input::handle_receive_from(const error_code &ec, size_t len) {
	if (!ec) {
		// It is not practical to prevent the same endpoint path from
		// being used by a new socket (especially if it is a relative
		// path) after it has been unlinked because multiple binds are
		// only prevented for the file inode not the path. However, it
		// will not be possible to make additional connections to the
		// input socket after it has been unlinked (even if the same
		// path is created).
		if (recv_ep_ == out_ep_) {
			io_error_ |= !output_->output(OutputType::STDOUT, buffer_, len);
		} else if (recv_ep_ == err_ep_) {
			io_error_ |= !output_->output(OutputType::STDERR, buffer_, len);
		} else {
			// Ignore data from any other sockets
#ifdef GCOV_ENABLED
			__gcov_flush(); // LCOV_EXCL_LINE
#endif
		}

		input_.async_receive_from(buffer(buffer_), recv_ep_,
				bind(&Input::handle_receive_from, this, p::_1, p::_2));
	} else {
		// i18n: %1 = system call name; %2 = Boost.Asio error message
		print_error(format(_("input socket: %1%: %2%")) % "recv", ec);

		output_->interrupted();
		io_error_ = true;
		io_->stop();
	}
}

} // namespace dtee
