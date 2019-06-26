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

static_assert(BOOST_ASIO_HAS_SIGACTION, "Boost must use sigaction() so that the SA_RESTART flag can be used");

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
		  child_exited_(*io_),
		  interrupt_signals_(*io_),
		  ignored_signals_(*io_),
		  pipe_signal_(*io_),
		  output_(output) {
	handle_signals_ = command_line.cron_mode();
	ignore_sigint_ = command_line.ignore_interrupts();
}

void SignalHandler::start(pid_t pid) {
	add_non_interrupting_signal(child_exited_, SIGCHLD);

	if (handle_signals_) {
		add_non_interrupting_signal(interrupt_signals_, SIGHUP);
		add_non_interrupting_signal(interrupt_signals_, SIGTERM);

		if (!ignore_sigint_) {
			add_non_interrupting_signal(interrupt_signals_, SIGINT);
		}

		add_non_interrupting_signal(pipe_signal_, SIGPIPE);
	}

	if (ignore_sigint_) {
		add_non_interrupting_signal(ignored_signals_, SIGINT);
	}

	child_ = pid;

	child_exited_.async_wait(bind(&SignalHandler::handle_child_exited, this, p::_1, p::_2));
	interrupt_signals_.async_wait(bind(&SignalHandler::handle_interrupt_signals, this, p::_1, p::_2));
	pipe_signal_.async_wait(bind(&SignalHandler::handle_pipe_signal, this, p::_1, p::_2));
}

// Workaround for missing SA_RESTART support in Boost.Asio (1.62)
// https://github.com/boostorg/asio/issues/157
//
// This is not thread-safe when the same signal_number is added/removed from the signal_set
void SignalHandler::add_non_interrupting_signal(boost::asio::signal_set &signal_set, int signal_number) {
	struct sigaction act;

	signal_set.add(signal_number);

	if (::sigaction(signal_number, NULL, &act) != 0) {
		print_system_error(format("sigaction: %1%"));
	} else if ((act.sa_flags & SA_RESTART) == 0) {
		act.sa_flags |= SA_RESTART;

		if (::sigaction(signal_number, &act, NULL) != 0) {
			print_system_error(format("sigaction: %1%"));
		}
	}
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
