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
constexpr bool linux = true;
constexpr int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr bool linux = false;
#endif

#if defined(__FreeBSD__)
constexpr bool freebsd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool freebsd = false;
#endif

#if defined(__OpenBSD__)
constexpr bool openbsd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool openbsd = false;
#endif

#if defined(__NetBSD__)
constexpr bool netbsd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 128 * 1024;
#else
constexpr bool netbsd = false;
#endif

#if defined(__DragonFly__)
constexpr bool dragonflybsd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool dragonflybsd = false;
#endif

#if defined(__GNU__)
constexpr bool hurd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr bool hurd = false;
#endif

#if defined(__APPLE__)
constexpr bool darwin = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool darwin = false;
#endif

#if defined(__CYGWIN__)
constexpr bool cygwin = true;
constexpr int MINIMUM_RCVBUF_SIZE = 2 * 1024 * 1024;
#else
constexpr bool cygwin = false;
#endif

static_assert(MINIMUM_RCVBUF_SIZE >= 0, "Unknown platform");

} // namespace platform

} // namespace dtee
