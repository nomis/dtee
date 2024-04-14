/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021,2024  Simon Arlott

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

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

// https://github.com/chriskohlhoff/asio/issues/721
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wshadow"
#include <boost/asio.hpp>
#pragma GCC diagnostic pop
#include <boost/system/error_code.hpp>

#include "dispatch.h"
#include "input.h"
#include "platform.h"
#include "temp_directory.h"

namespace dtee {

// Ensure the receive buffer is large enough; at least as large as both
// PIPE_BUF (for pipe writes) and BUFSIZ (for file writes)
constexpr const int MINIMUM_RCVBUF_SIZE = std::max(PIPE_BUF,
	std::max(BUFSIZ, platform::os::MINIMUM_RCVBUF_SIZE));

class Input {
public:
	Input(std::shared_ptr<boost::asio::io_service> io, std::shared_ptr<Dispatch> output);
	~Input() = default;

	bool open();
	void fork_parent();
	void start();
	void fork_child();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private:
	bool open_input(TempDirectory &temp_dir,
			boost::asio::local::datagram_protocol::endpoint &input_ep,
			boost::asio::local::datagram_protocol::socket::receive_buffer_size &so_rcvbuf);
	bool open_outputs(TempDirectory &temp_dir,
			const boost::asio::local::datagram_protocol::endpoint &input_ep,
			const boost::asio::local::datagram_protocol::socket::receive_buffer_size &so_rcvbuf);
	void open_output(
			const boost::asio::local::datagram_protocol::endpoint &input_ep,
			boost::asio::local::datagram_protocol::socket &output,
			std::string &output_path,
			const boost::asio::local::datagram_protocol::socket::receive_buffer_size &so_rcvbuf);
	void prepare_buffer(
			const boost::asio::local::datagram_protocol::socket::receive_buffer_size &so_rcvbuf);
	void close_outputs();

	void handle_receive_from(const boost::system::error_code &ec, size_t len);

	std::shared_ptr<boost::asio::io_service> io_;
	boost::asio::local::datagram_protocol::socket input_; //!< Incoming socket for data from child process
	boost::asio::local::datagram_protocol::socket out_; //!< Standard output of child process
	boost::asio::local::datagram_protocol::socket err_; //!< Standard error of child process
	std::string out_path_; //!< Endpoint path for child process standard output
	std::string err_path_; //!< Endpoint path for child process standard error

	std::vector<char> buffer_; //!< Incoming data
	boost::asio::local::datagram_protocol::endpoint recv_ep_; //!< Sender of incoming data

	std::shared_ptr<Dispatch> output_;
};

} // namespace dtee
