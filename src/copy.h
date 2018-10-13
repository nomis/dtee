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
#ifndef DTEE_COPY_H_
#define DTEE_COPY_H_

#include <list>
#include <memory>
#include <vector>

#include "output.h"

namespace dtee {

class Copy: public Output {
public:
	explicit Copy(std::list<std::shared_ptr<Output>> outputs);
	~Copy();

	bool open() override;
	bool output(OutputType type, const std::vector<char> &buffer, size_t len) override;
	void terminated(int status, int signum, bool core_dumped) override;
	void interrupted(int signum) override;

private:
	std::list<std::shared_ptr<Output>> outputs_;
};

} // namespace dtee

#endif
