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

#include <cstddef>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "dispatch.h"
#include "input.h"

namespace dtee {

class Input {
public:
	Input(boost::asio::io_service &io, std::shared_ptr<Dispatch> output);
	~Input() = default;

	bool open();
	void fork_parent();
	void start();
	bool stop();
	void fork_child();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private:
	void open_output(const boost::asio::local::datagram_protocol::endpoint &input_ep,
			boost::asio::local::datagram_protocol::socket &output,
			boost::asio::local::datagram_protocol::endpoint &output_ep,
			const boost::asio::local::datagram_protocol::socket::send_buffer_size &so_sndbuf);
	void close_outputs();

	void handle_receive_from(const boost::system::error_code &ec, size_t len);

	boost::asio::io_service &io_;
	boost::asio::local::datagram_protocol::socket input_; //!< Incoming socket for data from child process
	boost::asio::local::datagram_protocol::socket out_; //!< Standard output of child process
	boost::asio::local::datagram_protocol::socket err_; //!< Standard error of child process
	boost::asio::local::datagram_protocol::endpoint out_ep_; //!< Endpoint name for child process standard output
	boost::asio::local::datagram_protocol::endpoint err_ep_; //!< Endpoint name for child process standard error

	bool io_error_ = false;

	std::vector<char> buffer_; //!< Incoming data
	boost::asio::local::datagram_protocol::endpoint recv_ep_; //!< Sender of incoming data

	std::shared_ptr<Dispatch> output_;
};

} // namespace dtee

#endif
