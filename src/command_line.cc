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
#include "command_line.h"

#include <libgen.h>
#include <stdlib.h>
#include <sysexits.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

using ::std::string;
using ::std::vector;

using ::std::cout;
using ::std::endl;
using ::std::flush;

namespace po = boost::program_options;

namespace dtee {

const string CommandLine::DEFAULT_PROGRAM_NAME {"dtee"};
const string CommandLine::CRON_MODE_NAME {"cronty"};

// Boost (1.62) is going to consume "--=argument" as a positional option
// whether we want it to or not. It will also insist on accepting a long
// name for positional arguments. To avoid having hidden names like
// "--command", just explicitly use "--" as the name. 😩
const string CommandLine::BOOST_COMMAND_OPT {"-"};

void CommandLine::update_name(const string &program_name) {
	// Make a copy because basename() modifies its argument
	vector<char> program_name_copy{program_name.cbegin(), program_name.cend() + 1};
	const char *base_program_name = basename(program_name_copy.data());

	display_name_ = program_name;
	if (base_program_name == CRON_MODE_NAME) {
		cron_mode_ = true;

		// This is the only alternative internal name allowed
		// because it's used to construct temporary files.
		internal_name_ = CRON_MODE_NAME;
	}
}

vector<po::option> CommandLine::end_of_opts_parser(std::vector<std::string> &args) {
	vector<po::option> result;
	vector<string>::const_iterator i{args.cbegin()};
	bool capture;

	if (i == args.cend()) {
		capture = false;
	} else if (i->empty()) {
		// The empty string implies the end of command options, but Boost also
		// calls this function for every option with its arguments... trying
		// to capture the rest of the parameters does nothing but if we are
		// called with args={""} then Boost (1.62) throws an exception when we
		// remove it, so don't do that.
		//
		// https://svn.boost.org/trac10/ticket/11891
		capture = args.size() > 1;

		// This also means that if you specify only {""} on the entire command
		// line, or at the end of it, then the capture process will not run.
		// In that scenario, Boost (1.62) considers it a positional argument
		// as normal so the command line is still parsed correctly.
	} else if (*i == "--") {
		// Standard end of command options signifier
		capture = true;
		++i;
	} else if (i->at(0) != '-') {
		// Any unrecognised bare word signifies the end of command options
		capture = true;
	} else {
		capture = false;
	}

    if (capture) {
		while (i != args.cend()) {
			po::option opt;

			opt.string_key = BOOST_COMMAND_OPT;
			opt.value.push_back(*i);
			opt.original_tokens.push_back(*i);
			result.push_back(opt);

			++i;
		}

		args.clear();
	}

    return result;
}

void CommandLine::parse(int argc, const char* const argv[]) {
	if (argc > 0) {
		update_name(argv[0]);
	}

	// LCOV_EXCL_BR_START
	po::options_description visible_opts{"Allowed options"};
	visible_opts.add_options()
		("out-overwrite,O",
				po::value<vector<string>>()->value_name("FILE"),
				"copy standard output to FILE")
		("out-append,o",
				po::value<vector<string>>()->value_name("FILE"),
				"append standard output to FILE")
		("err-overwrite,E",
				po::value<vector<string>>()->value_name("FILE"),
				"copy standard error to FILE")
		("err-append,e",
				po::value<vector<string>>()->value_name("FILE"),
				"append standard error to FILE")
		("combined-overwrite,C",
				po::value<vector<string>>()->value_name("FILE"),
				"copy standard output and error to FILE")
		("combined-append,c",
				po::value<vector<string>>()->value_name("FILE"),
				"append standard output and error to FILE")
		;

	po::options_description hidden_opts;
	hidden_opts.add_options()
		("debug-options", po::bool_switch())
		(BOOST_COMMAND_OPT.c_str(), po::value<vector<string>>())
		;

	po::options_description cron_opts;
	cron_opts.add_options()
		("cron,q", po::bool_switch(), "operate in cron mode (combine original output but suppress it unless the process outputs an error message or has a non-zero exit status)")
		;

	po::options_description help_opts;
	help_opts.add_options()
		("help,h", po::bool_switch(), "display this help and exit")
		("version,V", po::bool_switch(), "output version information and exit")
		;
	// LCOV_EXCL_BR_STOP

	(cron_mode_ ? hidden_opts : visible_opts).add(cron_opts);
	visible_opts.add(help_opts);

	po::options_description all_opts;
	all_opts.add(visible_opts).add(hidden_opts);

	po::positional_options_description pd;
	pd.add(BOOST_COMMAND_OPT.c_str(), -1);

	try {
		po::store(po::command_line_parser(argc, argv)
			.style(po::command_line_style::unix_style & ~po::command_line_style::allow_guessing)
			.extra_style_parser(end_of_opts_parser)
			.options(all_opts)
			.positional(pd)
			.run(), *this);


		if (at("help").as<bool>()) {
			display_usage(visible_opts);
			exit(EXIT_SUCCESS);
		}

		po::notify(*this);

		if (at("version").as<bool>()) {
			display_version();
			exit(EXIT_SUCCESS);
		}

		if (at("debug-options").as<bool>()) {
			display_variables();
			exit(EXIT_SUCCESS);
		}

		if (!count(BOOST_COMMAND_OPT)) {
			display_usage(visible_opts);
			exit(EX_USAGE);
		}
	} catch (std::exception &e) {
	    cout << display_name_ << ": " << e.what() << endl;
	    cout << "Try '" << display_name_ << " -h' for more information." << endl;
	    exit(EX_USAGE);
	}

	cron_mode_ |= flag("cron");
}

void CommandLine::display_usage(po::options_description &options) const {
	cout << "Usage: " << display_name_ << " [OPTION]... COMMAND [ARG]..." << endl << endl;
	cout << "Run COMMAND with standard output and standard error copied to each FILE," << endl;
	if (cron_mode()) {
		cout << "suppressing all normal output unless the process outputs an error message" << endl;
		cout << "or has a non-zero exit status whereupon the original output will be combined" << endl;
		cout << "and written to standard output. The exit code will be written to standard" << endl;
		cout << "error." << endl;
	} else {
		cout << "while maintaining the original standard output and standard error as normal." << endl;
	}
	cout << endl;
	cout << options << endl;
}

void CommandLine::display_version() const {
	cout << DEFAULT_PROGRAM_NAME << " - run a program with standard output and standard error copied to files" << endl;
	cout << "Copyright 2018  Simon Arlott" << endl;
	cout << "Licence GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law." << endl;
	cout << "This is free software: you are free to change and redistribute it." << endl;
}

void CommandLine::display_variables() const {
	for (const auto& variable : *this) {
		if (variable.second.defaulted()) {
			continue;
		} else if (variable.second.value().type() == typeid(vector<string>)) {
			for (const auto& value : variable.second.as<vector<string>>()) {
				cout << variable.first << "=" << value << endl;
			}
		} else if (variable.second.value().type() == typeid(bool)) {
			cout << variable.first << "=" << variable.second.as<bool>() << endl;
		}
	}
}

bool CommandLine::flag(const string &name) const {
	return operator[](name).as<bool>();
}

const vector<string>& CommandLine::list(const string &name) const {
	const auto& value = operator[](name);

	if (!value.empty()) {
		return value.as<vector<string>>();
	} else {
		return empty_list_;
	}
}

} // namespace dtee