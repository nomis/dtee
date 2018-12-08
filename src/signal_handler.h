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
#ifndef DTEE_SIGNAL_HANDLER_H_
#define DTEE_SIGNAL_HANDLER_H_

#include <sys/types.h>
#include <cstddef>
#include <memory>

#include <boost/asio.hpp>

#include "command_line.h"
#include "result_handler.h"

namespace dtee {

class SignalHandler {
public:
	SignalHandler(const CommandLine &command_line, boost::asio::io_service &io, std::shared_ptr<ResultHandler> output);
	~SignalHandler() = default;

	void start(pid_t pid);
	bool stop();

	SignalHandler(const SignalHandler&) = delete;
	SignalHandler& operator=(const SignalHandler&) = delete;

private:
	void handle_child_exited(const boost::system::error_code &ec, int signal_number);
	void handle_interrupt_signals(const boost::system::error_code &ec, int signal_number);
	void handle_pipe_signal(const boost::system::error_code &ec, int signal_number);

	boost::asio::io_service &io_;

	pid_t child_ = -1;
	bool io_error_ = false;

	boost::asio::signal_set child_exited_;
	boost::asio::signal_set interrupt_signals_;
	boost::asio::signal_set ignored_signals_;
	boost::asio::signal_set pipe_signal_;

	std::shared_ptr<ResultHandler> output_;
	bool handle_signals_;
	bool ignore_sigint_;
};

} // namespace dtee

#endif
