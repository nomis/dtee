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
#include "stream_output.h"

using ::std::ostream;
using ::std::string;

namespace dtee {

StreamOutput::StreamOutput(ostream &stream, OutputType type) : stream_(stream), type_(type) {

}

StreamOutput::~StreamOutput() {

}
bool StreamOutput::output(OutputType type, const std::vector<char> &buffer, size_t len) {
	if (type == type_) {
		stream_.write(buffer.data(), len);
		stream_.flush();
	}

	return true;
}

} // namespace dtee
