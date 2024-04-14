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
#include "signal_block.h"

#include <signal.h>

#include <cstddef>

namespace dtee {

SignalBlock::SignalBlock() {
	sigemptyset(&sigset_);
}

SignalBlock::~SignalBlock() {
	clear();
}

void SignalBlock::add(int signal_number) {
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, signal_number);
	::sigprocmask(SIG_BLOCK, &set, NULL);

	sigaddset(&sigset_, signal_number);
	empty_ = false;
}

void SignalBlock::clear() {
	if (!empty_) {
		::sigprocmask(SIG_UNBLOCK, &sigset_, NULL);

		sigemptyset(&sigset_);
		empty_ = true;
	}
}

} // namespace dtee
