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
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>

#include "input.h"
#include "output.h"

namespace dtee {

class Input {
public:
	explicit Input(std::shared_ptr<Output> output);
	virtual ~Input();
	void fork_prepare();
	void fork_parent(pid_t pid);
	void fork_child();

	Input(Input&&);
	Input& operator=(Input&&);

private:
	void handle_receive_from(const boost::system::error_code &ec, size_t len);
	void handle_signal(const boost::system::error_code &ec, int signal_number);

	boost::asio::io_service io_;
	boost::asio::local::datagram_protocol::socket combined_; //!< Incoming socket for data from child process
	boost::asio::local::datagram_protocol::socket out_; //!< Standard output of child process
	boost::asio::local::datagram_protocol::socket err_; //!< Standard error of child process
	boost::asio::local::datagram_protocol::endpoint out_ep_; //!< Endpoint name for child process standard output
	boost::asio::local::datagram_protocol::endpoint err_ep_; //!< Endpoint name for child process standard error

	pid_t child_ = -1;
	bool terminated_ = false;
	boost::asio::signal_set signals_;
	std::vector<char> buffer_; //!< Incoming data
	boost::asio::local::datagram_protocol::endpoint recv_ep_; //!< Sender of incoming data
	bool data_received_ = false;

	std::shared_ptr<Output> output_;
};

} // namespace dtee

#endif
