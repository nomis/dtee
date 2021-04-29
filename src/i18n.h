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
#ifndef DTEE_I18N_H_
#define DTEE_I18N_H_

#include <libintl.h>

namespace dtee {

static inline const char* _(const char *msgid) {
	return gettext(msgid);
}

// gettext_noop
static constexpr inline const char* N_(const char *msgid) {
	return msgid;
}

} // namespace dtee

#endif

