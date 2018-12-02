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
#ifndef DTEE_INPUT_H_
#define DTEE_INPUT_H_

#include <sys/types.h>
#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "command_line.h"
#include "dispatch.h"
#include "input.h"
#include "to_string.h"

namespace dtee {

class Input {
public:
	Input(const CommandLine &command_line, std::shared_ptr<Dispatch> output);
	~Input() = default;

	bool open();
	void fork_prepare();
	bool fork_parent(pid_t pid);
	void fork_child();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private:
	void open_output(const boost::asio::local::datagram_protocol::endpoint &input_ep,
			boost::asio::local::datagram_protocol::socket &output,
			boost::asio::local::datagram_protocol::endpoint &output_ep,
			const boost::asio::local::datagram_protocol::socket::send_buffer_size &so_sndbuf);

	void handle_receive_from(const boost::system::error_code &ec, size_t len);
	void handle_child_exited(const boost::system::error_code &ec, int signal_number);
	void handle_interrupt_signals(const boost::system::error_code &ec, int signal_number);
	void handle_pipe_signal(const boost::system::error_code &ec, int signal_number);
	void print_socket_error(boost::format message, const boost::system::error_code &ec);
	void print_socket_error(boost::format message, const std::exception &e);
	void print_system_error(boost::format message, std::string cause = errno_to_string());

	boost::asio::io_service io_;
	boost::asio::local::datagram_protocol::socket input_; //!< Incoming socket for data from child process
	boost::asio::local::datagram_protocol::socket out_; //!< Standard output of child process
	boost::asio::local::datagram_protocol::socket err_; //!< Standard error of child process
	boost::asio::local::datagram_protocol::endpoint out_ep_; //!< Endpoint name for child process standard output
	boost::asio::local::datagram_protocol::endpoint err_ep_; //!< Endpoint name for child process standard error

	pid_t child_ = -1;
	bool io_error_ = false;

	boost::asio::signal_set child_exited_;
	boost::asio::signal_set interrupt_signals_;
	boost::asio::signal_set ignored_signals_;
	boost::asio::signal_set pipe_signal_;
	std::vector<char> buffer_; //!< Incoming data
	boost::asio::local::datagram_protocol::endpoint recv_ep_; //!< Sender of incoming data

	std::shared_ptr<Dispatch> output_;
	bool handle_signals_;
	bool ignore_sigint_;
};

} // namespace dtee

#endif
