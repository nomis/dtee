/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021  Simon Arlott

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

namespace dtee {

namespace platform {

#if defined(__linux__)
constexpr const bool linux = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr const bool linux = false;
#endif

#if defined(__FreeBSD__)
constexpr const bool freebsd = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr const bool freebsd = false;
#endif

#if defined(__OpenBSD__)
constexpr const bool openbsd = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr const bool openbsd = false;
#endif

#if defined(__NetBSD__)
constexpr const bool netbsd = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 128 * 1024;
#else
constexpr const bool netbsd = false;
#endif

#if defined(__DragonFly__)
constexpr const bool dragonflybsd = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr const bool dragonflybsd = false;
#endif

#if defined(__GNU__)
constexpr const bool hurd = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr const bool hurd = false;
#endif

#if defined(__APPLE__)
constexpr const bool darwin = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr const bool darwin = false;
#endif

#if defined(__CYGWIN__)
constexpr const bool cygwin = true;
constexpr const int MINIMUM_RCVBUF_SIZE = 2 * 1024 * 1024;
#else
constexpr const bool cygwin = false;
#endif

static_assert(MINIMUM_RCVBUF_SIZE >= 0, "Unknown platform");

} // namespace platform

} // namespace dtee
