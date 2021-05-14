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
#pragma once

namespace dtee {

namespace platform {

#if defined(__linux__)
constexpr bool Linux = true;
constexpr int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr bool Linux = false;
#endif

#if defined(__FreeBSD__)
constexpr bool FreeBSD = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool FreeBSD = false;
#endif

#if defined(__OpenBSD__)
constexpr bool OpenBSD = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool OpenBSD = false;
#endif

#if defined(__NetBSD__)
constexpr bool NetBSD_ = true;
constexpr int MINIMUM_RCVBUF_SIZE = 128 * 1024;
#else
constexpr bool NetBSD_ = false;
#endif

#if defined(__DragonFly__)
constexpr bool DragonFlyBSD = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool DragonFlyBSD = false;
#endif

#if defined(__GNU__)
constexpr bool Hurd = true;
constexpr int MINIMUM_RCVBUF_SIZE = 0;
#else
constexpr bool Hurd = false;
#endif

#if defined(__APPLE__)
constexpr bool Darwin = true;
constexpr int MINIMUM_RCVBUF_SIZE = 512 * 1024;
#else
constexpr bool Darwin = false;
#endif

#if defined(__CYGWIN__)
constexpr bool Cygwin = true;
constexpr int MINIMUM_RCVBUF_SIZE = 2 * 1024 * 1024;
#else
constexpr bool Cygwin = false;
#endif

} // namespace platform

} // namespace dtee
