/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018-2019,2021,2024  Simon Arlott

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
#pragma once

#include <sys/types.h>

#include <cstddef>
#include <memory>

// https://github.com/chriskohlhoff/asio/issues/721
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wshadow"
#include <boost/asio.hpp>
#pragma GCC diagnostic pop

#include "command_line.h"
#include "result_handler.h"
#include "signal_block.h"

namespace dtee {

class SignalHandler {
public:
	SignalHandler(const CommandLine &command_line, std::shared_ptr<boost::asio::io_service> io, std::shared_ptr<ResultHandler> output);

	bool open();
	void start(pid_t pid);
	void stop();

	SignalHandler(const SignalHandler&) = delete;
	SignalHandler& operator=(const SignalHandler&) = delete;

private:
	bool add(boost::asio::signal_set &signal_set, int signal_number);

	void handle_child_exited(const boost::system::error_code &ec, int signal_number);
	void handle_interrupt_signals(const boost::system::error_code &ec, int signal_number);
	void handle_pipe_signal(const boost::system::error_code &ec, int signal_number);

	std::shared_ptr<boost::asio::io_service> io_;

	pid_t child_ = -1;

	// Must be before the signal_sets so that they are destructed first
	// (removing the signal handlers before the signals are unblocked)
	SignalBlock blocked_signals_;

	boost::asio::signal_set child_exited_;
	boost::asio::signal_set interrupt_signals_;
	boost::asio::signal_set ignored_signals_;
	boost::asio::signal_set pipe_signal_;

	std::shared_ptr<ResultHandler> output_;
	const bool handle_signals_;
	const bool ignore_sigint_;
};

} // namespace dtee
