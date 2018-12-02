/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018  Simon Arlott

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
#if defined(__APPLE__)
// Wrap sigaction manually because the linker can't do it 😩
# define sigaction __wrap_sigaction
#endif

#include "input.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <climits>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "application.h"
#include "temp_directory.h"

using ::std::bind;
using ::std::max;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;
using ::boost::asio::buffer;
using ::boost::asio::local::datagram_protocol;
using ::boost::asio::io_service;
using ::boost::format;
using ::boost::system::error_code;

namespace p = ::std::placeholders;

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

namespace dtee {

Input::Input(const CommandLine &command_line, shared_ptr<Dispatch> output)
		: input_(io_),
		  out_(io_),
		  err_(io_),
		  child_exited_(io_, SIGCHLD),
		  interrupt_signals_(io_),
		  ignored_signals_(io_),
		  pipe_signal_(io_),
		  output_(output) {
	handle_signals_ = command_line.cron_mode();
	ignore_sigint_ = command_line.ignore_interrupts();
}

void Input::print_socket_error(format message, const error_code &ec) {
	Application::print_error(message % ec.message());
}

void Input::print_socket_error(format message, const std::exception &e) {
	Application::print_error(message % e.what());
}

void Input::print_system_error(format message, std::string cause) {
	Application::print_error(message % cause);
}

bool Input::open() {
	// There is no equivalent of mkstemp() for local sockets,
	// so we create them in a temporary directory instead.
	TempDirectory temp_dir{"input"};

	if (!temp_dir.valid()) {
		return false;
	}

	if (handle_signals_) {
		interrupt_signals_.add(SIGHUP);
		interrupt_signals_.add(SIGTERM);

		if (!ignore_sigint_) {
			interrupt_signals_.add(SIGINT);
		}

		pipe_signal_.add(SIGPIPE);
	}

	if (ignore_sigint_) {
		ignored_signals_.add(SIGINT);
	}

	const string input_name = temp_dir.register_file("i");
	const datagram_protocol::endpoint input_ep{input_name};

	const string out_name = temp_dir.register_file("o");
	out_ep_ = datagram_protocol::endpoint{out_name};

	const string err_name = temp_dir.register_file("e");
	err_ep_ = datagram_protocol::endpoint{err_name};

#if defined(__CYGWIN__)
	constexpr int PLATFORM_MINIMUM_RCVBUF_SIZE = 2 * 1024 * 1024;
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__APPLE__) || defined(__CYGWIN__)
	constexpr int PLATFORM_MINIMUM_RCVBUF_SIZE = 512 * 1024;
#elif defined(__NetBSD__)
	constexpr int PLATFORM_MINIMUM_RCVBUF_SIZE = 128 * 1024;
#else
	constexpr int PLATFORM_MINIMUM_RCVBUF_SIZE = 0;
#endif

	// Ensure the receive buffer is large enough at least as large as both
	// PIPE_BUF (for pipe writes) and BUFSIZ (for file writes)
	const int MINIMUM_RCVBUF_SIZE = max(PIPE_BUF, max(BUFSIZ, PLATFORM_MINIMUM_RCVBUF_SIZE));

	datagram_protocol::socket::receive_buffer_size so_rcvbuf;

	try {
		input_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		input_.bind(input_ep);

#if !defined(__GNU__)
		input_.get_option(so_rcvbuf);

		if (so_rcvbuf.value() < MINIMUM_RCVBUF_SIZE) {
			so_rcvbuf = MINIMUM_RCVBUF_SIZE;
			input_.set_option(so_rcvbuf);
			input_.get_option(so_rcvbuf);
		}
#else
		so_rcvbuf = MINIMUM_RCVBUF_SIZE;
#endif
	} catch (std::exception &e) {
		print_socket_error(format("input socket: %1%"), e);
		return false;
	}

	int buffer_size = so_rcvbuf.value();
#ifdef __linux__
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
#endif

	if (buffer_size < MINIMUM_RCVBUF_SIZE) {
		buffer_size = MINIMUM_RCVBUF_SIZE;
	}
	buffer_.resize(buffer_size);

	datagram_protocol::socket::send_buffer_size so_sndbuf{so_rcvbuf.value()};

	try {
		open_output(input_ep, out_, out_ep_, so_sndbuf);
	} catch (std::exception &e) {
		print_socket_error(format("stdout socket: %1%"), e);
		return false;
	}

	try {
		open_output(input_ep, err_, err_ep_, so_sndbuf);
	} catch (std::exception &e) {
		print_socket_error(format("stderr socket: %1%"), e);
		return false;
	}

#if defined(__GNU__)
	if (out_ep_ == err_ep_) {
		// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
		// so they both look the same.
		Application::print_error(format("output socket endpoints are not unique"));
		return false;
	}
#endif

	return true;
}

void Input::open_output(const datagram_protocol::endpoint &input_ep,
		datagram_protocol::socket &output, datagram_protocol::endpoint &output_ep,
		const datagram_protocol::socket::send_buffer_size &so_sndbuf) {
	output.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
	output.bind(output_ep);
#if defined(__OpenBSD__) || defined(__GNU__)
	// Workaround Boost.Asio (1.66.0) bug on OpenBSD 6.4
	// Endpoint paths can't be compared correctly if they weren't both
	// constructed by the application or both returned by the OS.
	// https://github.com/boostorg/asio/issues/161
	//
	// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
	// so they both look the same. Retrieve the address after binding so that
	// this can be checked.
	output_ep = output.local_endpoint();
#endif
	output.connect(input_ep);
	output.shutdown(datagram_protocol::socket::shutdown_receive);
#if !defined(__GNU__)
	// Not supported on GNU (Hurd 0.9, Mach 1.8)
	output.set_option(so_sndbuf);
#endif
#if defined(__CYGWIN__)
	// On Cygwin, getsockname() does not return the same value as
	// recvfrom() or getpeername() does for the other socket.
	// Send an empty message to obtain the real socket address.
	vector<char> empty{0};
	output.send(buffer(empty));
	input_.receive_from(buffer(empty), output_ep);
#endif
}

void Input::fork_prepare() {
	io_.notify_fork(io_service::fork_event::fork_prepare);
}

bool Input::fork_parent(pid_t pid) {
	error_code ec;

	io_.notify_fork(io_service::fork_event::fork_parent);

	out_.close(ec);
	if (ec) {
		print_socket_error(format("stdout socket close: %1%"), ec);
	}

	err_.close(ec);
	if (ec) {
		print_socket_error(format("stderr socket close: %1%"), ec);
	}

	child_ = pid;

	input_.async_receive_from(buffer(buffer_), recv_ep_,
			bind(&Input::handle_receive_from, this, p::_1, p::_2));

	child_exited_.async_wait(bind(&Input::handle_child_exited, this, p::_1, p::_2));
	interrupt_signals_.async_wait(bind(&Input::handle_interrupt_signals, this, p::_1, p::_2));
	pipe_signal_.async_wait(bind(&Input::handle_pipe_signal, this, p::_1, p::_2));

	do {
		io_.run(ec);

		if (ec) {
			print_socket_error(format("asio run: %1%"), ec);
			io_error_ = true;
			break;
		}
	} while (!io_.stopped());

	size_t events;
	do {
		io_.reset();
		events = io_.poll(ec);

		if (ec) {
			print_socket_error(format("asio poll: %1%"), ec);
			io_error_ = true;
			break;
		}
	} while (events > 0);

	interrupt_signals_.clear();

	return !io_error_;
}

void Input::fork_child() {
	error_code ec;

	io_.notify_fork(io_service::fork_event::fork_child);

	errno = 0;
	if (::dup2(out_.native_handle(), STDOUT_FILENO) < 0) {
		print_system_error(format("stdout dup2: %1%"));
	}

	errno = 0;
	if (::dup2(err_.native_handle(), STDERR_FILENO) < 0) {
		print_system_error(format("stderr dup2: %1%"));
	}

	input_.close(ec);
	if (ec) {
		print_socket_error(format("input socket close: %1%"), ec);
	}

	out_.close(ec);
	if (ec) {
		print_socket_error(format("stdout socket close: %1%"), ec);
	}

	err_.close(ec);
	if (ec) {
		print_socket_error(format("stderr socket close: %1%"), ec);
	}
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
		print_socket_error(format("socket receive: %1%"), ec);

		output_->interrupted();
		io_error_ = true;
		io_.stop();
	}
}

void Input::handle_child_exited(const error_code &ec, int signal_number) {
	if (!ec) {
		int wait_status;

		errno = 0;
		pid_t ret = ::waitpid(child_, &wait_status, WNOHANG);
		if (ret <= 0) {
			if (ret != 0) {
				print_system_error(format("waitpid: %1%"));
			}

			output_->interrupted(signal_number);
			io_error_ = true;
		} else {
			bool core_dumped = false;
			int exit_status = -1;
			int exit_signum = -1;

			if (WIFEXITED(wait_status)) {
				exit_status = WEXITSTATUS(wait_status);
			}

			if (WIFSIGNALED(wait_status)) {
				exit_signum = WTERMSIG(wait_status);
			}

#ifdef WCOREDUMP
			if (WCOREDUMP(wait_status)) {
				core_dumped = true;
			}
#endif

			output_->terminated(exit_status, exit_signum, core_dumped);
		}

		child_exited_.clear();
		io_.stop();
	}
}

void Input::handle_interrupt_signals(const error_code &ec, int signal_number) {
	if (!ec) {
		output_->interrupted(signal_number);
		interrupt_signals_.async_wait(bind(&Input::handle_interrupt_signals, this, p::_1, p::_2));
		io_.stop();
	}
}

void Input::handle_pipe_signal(const error_code &ec, int signal_number) {
	if (!ec) {
		output_->interrupted(signal_number);
		pipe_signal_.async_wait(bind(&Input::handle_pipe_signal, this, p::_1, p::_2));
		io_.stop();
	}
}

} // namespace dtee
