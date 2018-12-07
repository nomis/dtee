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
#include "dispatch.h"

#include <cstddef>
#include <string>

using ::std::string;

namespace dtee {

Dispatch::Dispatch(std::list<std::shared_ptr<Output>> &outputs, std::list<std::shared_ptr<ResultHandler>> &result_handlers)
		: outputs_(outputs), result_handlers_(result_handlers) {

}

bool Dispatch::open() {
	bool success = true;

	for (auto& output : outputs_) {
		success &= output->open();
	}

	return success;
}

bool Dispatch::output(OutputType type, const std::vector<char> &buffer, size_t len) {
	bool success = true;

	for (auto& output : outputs_) {
		success &= output->output(type, buffer, len);
	}

	return success;
}

void Dispatch::terminated(int status, int signum, bool core_dumped) {
	for (auto& result_handler : result_handlers_) {
		result_handler->terminated(status, signum, core_dumped);
	}
}

void Dispatch::interrupted(int signum) {
	for (auto& result_handler : result_handlers_) {
		result_handler->interrupted(signum);
	}
}

} // namespace dtee
