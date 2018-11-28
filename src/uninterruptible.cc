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

#include <csignal>

#include <boost/asio.hpp>

static_assert(BOOST_ASIO_HAS_SIGACTION, "Boost must use sigaction() so that the SA_RESTART flag can be used");

#if !defined(__APPLE__)
extern "C" int __real_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
#endif

extern "C" int __attribute__((used)) __wrap_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
	struct sigaction newact;

	if (act != nullptr) {
		newact = *act;
		newact.sa_flags |= SA_RESTART;
		act = &newact;
	}

#if defined(__APPLE__)
	return sigaction(signum, act, oldact);
#else
	return __real_sigaction(signum, act, oldact);
#endif
}

#if defined(__NetBSD__)
extern "C" int __real___sigaction14(int signum, const struct sigaction *act, struct sigaction *oldact);

extern "C" int __attribute__((used)) __wrap___sigaction14(int signum, const struct sigaction *act, struct sigaction *oldact) {
	struct sigaction newact;

	if (act != nullptr) {
		newact = *act;
		newact.sa_flags |= SA_RESTART;
		act = &newact;
	}

	return __real___sigaction14(signum, act, oldact);
}
#endif
