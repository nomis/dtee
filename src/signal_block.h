/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2021  Simon Arlott

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
#ifndef DTEE_SIGNAL_BLOCK_H_
#define DTEE_SIGNAL_BLOCK_H_

#include <signal.h>

#include <cstddef>

namespace dtee {

class SignalBlock {
public:
	SignalBlock();
	~SignalBlock();

	void add(int signal_number); //!< Block signal
	void clear(); //!< Unblock all signals

private:
	bool empty_ = true;
	sigset_t sigset_;
};

} // namespace dtee

#endif