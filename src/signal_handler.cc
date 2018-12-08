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

#include "signal_handler.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "application.h"
#include "print_error.h"

using ::boost::asio::io_service;
using ::boost::format;
using ::boost::system::error_code;
using ::std::bind;
using ::std::shared_ptr;

namespace p = ::std::placeholders;

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

namespace dtee {

SignalHandler::SignalHandler(const CommandLine &command_line, shared_ptr<boost::asio::io_service> &io, shared_ptr<ResultHandler> output)
		: io_(io),
		  child_exited_(*io_, SIGCHLD),
		  interrupt_signals_(*io_),
		  ignored_signals_(*io_),
		  pipe_signal_(*io_),
		  output_(output) {
	handle_signals_ = command_line.cron_mode();
	ignore_sigint_ = command_line.ignore_interrupts();
}

void SignalHandler::start(pid_t pid) {
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

	child_ = pid;

	child_exited_.async_wait(bind(&SignalHandler::handle_child_exited, this, p::_1, p::_2));
	interrupt_signals_.async_wait(bind(&SignalHandler::handle_interrupt_signals, this, p::_1, p::_2));
	pipe_signal_.async_wait(bind(&SignalHandler::handle_pipe_signal, this, p::_1, p::_2));
}

bool SignalHandler::stop() {
	interrupt_signals_.clear();

	return !io_error_;
}

void SignalHandler::handle_child_exited(const error_code &ec, int signal_number) {
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
		io_->stop();
	}
}

void SignalHandler::handle_interrupt_signals(const error_code &ec, int signal_number) {
	if (!ec) {
		output_->interrupted(signal_number);
		interrupt_signals_.async_wait(bind(&SignalHandler::handle_interrupt_signals, this, p::_1, p::_2));
		io_->stop();
	}
}

void SignalHandler::handle_pipe_signal(const error_code &ec, int signal_number) {
	if (!ec) {
		output_->interrupted(signal_number);
		pipe_signal_.async_wait(bind(&SignalHandler::handle_pipe_signal, this, p::_1, p::_2));
		io_->stop();
	}
}

} // namespace dtee
