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
#include "input.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sysexits.h>
#include <unistd.h>
#include <memory>
#include <string>

#include "application.h"
#include "temp_directory.h"

using ::std::bind;
using ::std::shared_ptr;
using ::std::string;
using ::std::to_string;
using ::boost::asio::local::datagram_protocol;
using ::boost::asio::io_service;
using ::boost::system::error_code;

namespace p = ::std::placeholders;

namespace dtee {

Input::Input(shared_ptr<Output> output)
		: combined_(io_),
		  out_(io_),
		  err_(io_),
		  signals_(io_, SIGCHLD),
		  output_(output) {
}

Input::~Input() {

}

bool Input::open() {
	// There is no equivalent of mkstemp() for local sockets,
	// so we create them in a temporary directory instead.
	TempDirectory temp_dir("input");

	if (!temp_dir.valid()) {
		return false;
	}

	try {
		string combined_name = temp_dir.register_file("c");
		datagram_protocol::endpoint combined_ep{combined_name};

		string out_name = temp_dir.register_file("o");
		out_ep_ = datagram_protocol::endpoint(out_name);

		string err_name = temp_dir.register_file("e");
		err_ep_ = datagram_protocol::endpoint{err_name};

		combined_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		combined_.bind(combined_ep);
		combined_.shutdown(datagram_protocol::socket::shutdown_send);

		datagram_protocol::socket::receive_buffer_size so_rcvbuf;
		combined_.get_option(so_rcvbuf);

		// Ensure the receive buffer is at least as large as PIPE_BUF
		if (so_rcvbuf.value() < PIPE_BUF) {
			datagram_protocol::socket::send_buffer_size so_sndbuf{PIPE_BUF};
			combined_.set_option(so_sndbuf);
			combined_.get_option(so_rcvbuf);
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

		if (buffer_size < PIPE_BUF) {
			buffer_size = PIPE_BUF;
		}
		buffer_.resize(buffer_size);

		datagram_protocol::socket::send_buffer_size so_sndbuf{so_rcvbuf.value()};

		out_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		out_.bind(out_ep_);
		out_.connect(combined_ep);
		out_.shutdown(datagram_protocol::socket::shutdown_receive);
		out_.set_option(so_sndbuf);

		err_.open(); // Boost (1.62) has no support for SOCK_CLOEXEC
		err_.bind(err_ep_);
		err_.connect(combined_ep);
		err_.shutdown(datagram_protocol::socket::shutdown_receive);
		err_.set_option(so_sndbuf);
	} catch (std::exception &e) {
		Application::print_error(string("socket ") + e.what());
		return false;
	}

	return true;
}

void Input::fork_prepare() {
	io_.notify_fork(io_service::fork_event::fork_prepare);
}

int Input::fork_parent(pid_t pid) {
	io_.notify_fork(io_service::fork_event::fork_parent);
	out_.close();
	err_.close();

	child_ = pid;

	combined_.async_receive_from(boost::asio::buffer(buffer_), recv_ep_,
			bind(&Input::handle_receive_from, this, p::_1, p::_2));

	signals_.async_wait(bind(&Input::handle_signal, this, p::_1, p::_2));

	error_code ec;
	while (1) {
		if (io_.stopped()) {
			io_.reset();
		}

		if (terminated_) {
			size_t events = io_.poll(ec);

			if (ec) {
				Application::print_error(ec.message());
				break;
			}

			if (!events) {
				break;
			}
		} else {
			io_.run(ec);

			if (ec) {
				Application::print_error(ec.message());
				break;
			}
		}
	}

	// Replicate original exit status (mostly... the only way to replicate
	// signals is to use the default action for the signal and send it to
	// ourselves, but not all of the signals will terminate by default).
	if (exit_status_ == EXIT_SUCCESS) {
		if (output_failed_) {
			return EX_IOERR;
		} else {
			return EXIT_SUCCESS;
		}
	} else if (exit_status_ >= 0) {
		return exit_status_;
	} else if (exit_signum_ >= 0){
		return SHELL_EXIT_CODE_SIGNAL + exit_signum_;
	} else {
		return EXIT_FAILURE;
	}
}

void Input::fork_child() {
	io_.notify_fork(io_service::fork_event::fork_child);
	combined_.close();
	dup2(out_.native_handle(), STDOUT_FILENO);
	out_.close();
	dup2(err_.native_handle(), STDERR_FILENO);
	err_.close();
}

void Input::handle_receive_from(const error_code &ec, size_t len) {
	if (!ec) {
		if (recv_ep_ == out_ep_) {
			output_failed_ |= !output_->output(OutputType::STDOUT, buffer_, len);
		} else if (recv_ep_ == err_ep_) {
			output_failed_ |= !output_->output(OutputType::STDERR, buffer_, len);
		} else {
			// Data from any other sockets should not be possible and is ignored
		}

		combined_.async_receive_from(boost::asio::buffer(buffer_), recv_ep_,
				bind(&Input::handle_receive_from, this, p::_1, p::_2));
	} else {
		Application::print_error("socket receive: " + ec.message());

		terminated_ = true;
		io_.stop();
	}
}

void Input::handle_signal(const error_code &ec, int signal_number) {
	if (!ec) {
		if (signal_number == SIGCHLD) {
			int wait_status;

			errno = 0;
			pid_t ret = waitpid(child_, &wait_status, WNOHANG);
			if (ret != 0) {
				if (ret < 0) {
					Application::print_error("waitpid", errno);
				} else {
					bool core_dumped = false;

					if (WIFEXITED(wait_status)) {
						exit_status_ = WEXITSTATUS(wait_status);
					}

					if (WIFSIGNALED(wait_status)) {
						exit_signum_ = WTERMSIG(wait_status);
					}

#ifdef WCOREDUMP
					if (WCOREDUMP(wait_status)) {
						core_dumped = true;
					}
#endif

					output_->terminated(exit_status_, exit_signum_, core_dumped);
				}

				terminated_ = true;
				io_.stop();
			} else {
				signals_.async_wait(bind(&Input::handle_signal, this, p::_1, p::_2));
			}
		}
	}

}

} // namespace dtee
