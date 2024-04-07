/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021,2024  Simon Arlott

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

namespace os {

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

} // namespace os

namespace have {

// Not supported on GNU (Hurd 0.9, Mach 1.8)
constexpr const bool so_rcvbuf = !os::hurd;
constexpr const bool so_sndbuf = !os::hurd;

} // namespace have

namespace hack {

// From SOCKET(7): "The kernel doubles this value (to allow space for bookkeeping
//     overhead) when it is set using setsockopt(2), and this doubled value is
//     returned by getsockopt(2)."
constexpr const bool socket_buffer_size_doubled = os::linux;

// Workaround Boost.Asio (1.66.0) bug on OpenBSD 6.4
// Endpoint paths can't be compared correctly if they weren't both
// constructed by the application or both returned by the OS.
// https://github.com/chriskohlhoff/asio/issues/649
//
// The addresses of Unix sockets are not stored on GNU (Hurd 0.9, Mach 1.8),
// so they both look the same.
constexpr const bool read_back_endpoint_path = os::openbsd || os::hurd;

// On Cygwin, getsockname() does not return the same value as
// recvfrom() or getpeername() does for the other socket.
// Send an empty message to obtain the real socket address.
constexpr const bool loopback_endpoint_path = os::cygwin;

constexpr const bool modified_endpoint_paths = read_back_endpoint_path || loopback_endpoint_path;

} // namespace hack

} // namespace platform

} // namespace dtee
