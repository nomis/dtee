/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2021-2022  Simon Arlott

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

#ifdef GCOV_ENABLED
# if __has_include(<gcov.h>)
extern "C" {
#  include <gcov.h>
}
# else
#  undef GCOV_ENABLED
# endif
#endif

namespace dtee {

// LCOV_EXCL_START
static void inline coverage_dump() {
#ifdef GCOV_ENABLED
	::__gcov_dump();
#endif
}
// LCOV_EXCL_STOP

} // namespace dtee
