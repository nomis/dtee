/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021,2024-2025  Simon Arlott

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
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// https://github.com/chriskohlhoff/asio/issues/721
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wshadow"
#include <boost/asio.hpp>
#pragma GCC diagnostic pop
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "application.h"
#include "i18n.h"
#include "platform.h"
#include "print_error.h"
#include "temp_directory.h"

using ::boost::asio::buffer;
using ::boost::asio::io_context;
using ::boost::asio::local::datagram_protocol;
using ::boost::format;
using ::boost::system::error_code;
using ::std::bind;
using ::std::max;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace p = ::std::placeholders;

namespace dtee {

Input::Input(shared_ptr<boost::asio::io_context> io, shared_ptr<Dispatch> output)
		: io_(io),
		  input_(*io_),
		  out_(*io_),
		  err_(*io_),
		  output_(output) {
}

bool Input::open() {
	// There is no equivalent of mkstemp() for local sockets, so we
	// create them in a temporary directory instead. It will be deleted
	// automatically when the variable goes out of scope at the end of
	// the function.
	TempDirectory temp_dir{"I"};
	datagram_protocol::endpoint input_ep;
	datagram_protocol::socket::receive_buffer_size so_rcvbuf;

	if (temp_dir.valid()
			&& open_input(temp_dir, input_ep, so_rcvbuf)
			&& open_outputs(temp_dir, input_ep, so_rcvbuf)) {
		prepare_buffer(so_rcvbuf);
		return true;
	} else {
		output_->error(ErrorType::OPEN_INPUT);
		return false;
	}
}

bool Input::open_input(TempDirectory &temp_dir,
		datagram_protocol::endpoint &input_ep,
		datagram_protocol::socket::receive_buffer_size &so_rcvbuf) {
	try {
		input_ep = temp_dir.register_file("i");
		input_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		input_.bind(input_ep);

		if (platform::have::so_rcvbuf) {
			input_.get_option(so_rcvbuf);

			if (so_rcvbuf.value() < MINIMUM_RCVBUF_SIZE) {
				so_rcvbuf = MINIMUM_RCVBUF_SIZE;
				input_.set_option(so_rcvbuf);
				input_.get_option(so_rcvbuf);
			}
		} else {
			so_rcvbuf = MINIMUM_RCVBUF_SIZE;
		}

		return true;
	} catch (const std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("input socket: %1%")), e);
		return false;
	}
}

bool Input::open_outputs(TempDirectory &temp_dir,
		const datagram_protocol::endpoint &input_ep,
		const datagram_protocol::socket::receive_buffer_size &so_rcvbuf) {
	try {
		out_path_ = temp_dir.register_file("o");
		open_output(input_ep, out_, out_path_, so_rcvbuf);
	} catch (const std::exception &e) {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("stdout socket: %1%")), e);
		return false;
	}

	try {
		err_path_ = temp_dir.register_file("e");
		open_output(input_ep, err_, err_path_, so_rcvbuf);
	} catch (const std::exception &e) {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("stderr socket: %1%")), e);
		return false;
	}

	if (platform::hack::modified_endpoint_paths) {
		if (out_path_ == err_path_) {
			// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
			// so they both look the same.
			print_error(format(_( // i18n: %1 = socket path; %2 = socket path
				"output socket endpoints are indistinguishable:\n"
				"\tstdout socket: %1%\n\tstderr socket: %2%"))
				% out_path_ % err_path_);
			return false;
		}
	}

	return true;
}

void Input::open_output(const datagram_protocol::endpoint &input_ep,
		datagram_protocol::socket &output, std::string &output_path,
		const datagram_protocol::socket::receive_buffer_size &so_rcvbuf) {
	datagram_protocol::socket::send_buffer_size so_sndbuf{so_rcvbuf.value()};

	output.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
	output.bind(output_path);

	if (platform::hack::read_back_endpoint_path) {
		// Retrieve the address after binding so that it matches received
		// messages.
		output_path = output.local_endpoint().path();
	}

	output.connect(input_ep);
	output.shutdown(datagram_protocol::socket::shutdown_receive);

	if (platform::have::so_sndbuf) {
		output.set_option(so_sndbuf);
	}

	if (platform::hack::loopback_endpoint_path) {
		// Send an empty message to obtain the real socket address.
		vector<char> empty;
		datagram_protocol::endpoint output_ep;

		output.send(buffer(empty));
		input_.receive_from(buffer(empty), output_ep);

		output_path = output_ep.path();
	}
}

void Input::prepare_buffer(
		const datagram_protocol::socket::receive_buffer_size &so_rcvbuf) {
	int buffer_size = so_rcvbuf.value();

	if (platform::hack::socket_buffer_size_doubled) {
		// From Boost (1.62): "Linux puts additional stuff into the buffers so
		//     that only about half is actually available to the application.
		//     The retrieved value is divided by 2 here to make it appear as
		//     though the correct value has been set."
		//
		// Boost is wrong because the kernel is not specified as using half the
		// buffer size. It would be simpler if the kernel didn't return its
		// doubled value either. For a 208KB receive buffer, Linux (4.13) uses
		// less than 1KB on x86_64.
		buffer_size *= 2;
	}

	buffer_size = max(buffer_size, MINIMUM_RCVBUF_SIZE);
	buffer_.resize(buffer_size);
}

void Input::close_outputs() {
	try {
		out_.close();
	} catch (const std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("stdout socket: %1%")), e);

		output_->error(ErrorType::CLOSE_OUTPUT);
	}

	try {
		err_.close();
	} catch (const std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("stderr socket: %1%")), e);

		output_->error(ErrorType::CLOSE_OUTPUT);
	}
}

void Input::fork_parent() {
	close_outputs();
}

void Input::start() {
	input_.async_receive_from(buffer(buffer_), recv_ep_,
			bind(&Input::handle_receive_from, this, p::_1, p::_2));
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
	} catch (const std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("input socket: %1%")), e);
	}

	close_outputs();
}

void Input::handle_receive_from(const error_code &ec, size_t len) {
	if (!ec) {
		const auto recv_path = recv_ep_.path();
		if (recv_path == out_path_) {
			output_->output(OutputType::STDOUT, buffer_, len);
		} else if (recv_path == err_path_) {
			output_->output(OutputType::STDERR, buffer_, len);
		} else {
			// It is not practical to prevent the same endpoint path from
			// being used by a new socket (especially if it is a relative
			// path) after it has been unlinked because multiple binds are
			// only prevented for the file inode not the path. However, it
			// will not be possible to make additional connections to the
			// input socket after it has been unlinked (even if the same
			// path is created). Ignore data from any other sockets.
		}

		input_.async_receive_from(buffer(buffer_), recv_ep_,
				bind(&Input::handle_receive_from, this, p::_1, p::_2));
	} else {
		// i18n: %1 = Boost.Asio error message
		print_error(format(_("input socket: %1%")), ec);

		output_->error(ErrorType::READ_INPUT);
		io_->stop();
	}
}

} // namespace dtee
