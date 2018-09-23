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
#include "application.h"

int main(int argc, char *argv[]) {
	try {
		dtee::Application application;
		return application.run(argc, argv);
	} catch (...) {
		// If exceptions are never caught, then object destructors may not be
		// called and so temporary sockets and directories that were created
		// will not be deleted automatically.
		//
		// https://stackoverflow.com/a/8311472/388191

		// Throw the exception again because we're not going to handle it
		throw;
	}
}
