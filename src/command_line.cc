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
#include "command_line.h"

#include <libgen.h>
#include <sysexits.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include "i18n.h"
#include "print_error.h"
#include "src/version.h"

using ::boost::format;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::exit;
using ::std::flush;
using ::std::string;
using ::std::vector;

namespace po = boost::program_options;

namespace dtee {

const string CommandLine::DEFAULT_PROGRAM_NAME{"dtee"};
const string CommandLine::CRON_MODE_NAME{"cronty"};
string CommandLine::internal_name_{CommandLine::DEFAULT_PROGRAM_NAME};
string CommandLine::display_name_{CommandLine::DEFAULT_PROGRAM_NAME};

// Boost (1.62) is going to consume "--=argument" as a positional option
// whether we want it to or not. It will also insist on accepting a long
// name for positional arguments. To avoid having hidden names like
// "--command", just explicitly use "--" as the name. 😩
const string CommandLine::BOOST_COMMAND_OPT{"-"};

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
	po::options_description visible_opts{_("Output files")};
	visible_opts.add_options()
		("out-append,o",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("append standard output to FILE"))
		("out-overwrite,O",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("copy standard output to FILE"))
		("err-append,e",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("append standard error to FILE"))
		("err-overwrite,E",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("copy standard error to FILE"))
		("combined-append,c",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("append standard output and error to FILE"))
		("combined-overwrite,C",
				po::value<vector<string>>()->value_name(_("FILE")),
				_("copy standard output and error to FILE"))
		;

	po::options_description hidden_opts;
	hidden_opts.add_options()
		("debug-options", po::bool_switch())
		(BOOST_COMMAND_OPT.c_str(), po::value<vector<string>>())
		;

	po::options_description mode_opts{_("Options")};
	mode_opts.add_options()
		("ignore-interrupts,i", po::bool_switch(), _("ignore interrupt signals"))
		;

	(cron_mode_ ? hidden_opts : mode_opts).add_options()
		("cron,q", po::bool_switch(), _("operate in cron mode"
				" (suppress output unless the process outputs"
				" an error message or has a non-zero exit status)"))
		;

	po::options_description misc_opts{_("Miscellaneous")};
	misc_opts.add_options()
		("help,h", po::bool_switch(), _("display this help and exit"))
		("version,V", po::bool_switch(), _("output version information and exit"))
		;

	visible_opts.add(mode_opts);
	visible_opts.add(misc_opts);

	po::options_description all_opts;
	all_opts.add(visible_opts);
	all_opts.add(hidden_opts);

	po::positional_options_description positional_opts;
	positional_opts.add(BOOST_COMMAND_OPT.c_str(), -1);
	// LCOV_EXCL_BR_STOP

	try {
		po::store(po::command_line_parser(argc, argv)
			.style(po::command_line_style::unix_style & ~po::command_line_style::allow_guessing)
			.extra_style_parser(end_of_opts_parser)
			.options(all_opts)
			.positional(positional_opts)
			.run(), variables_);

		if (variables_["help"].as<bool>()) {
			display_usage(visible_opts);
			exit(EXIT_SUCCESS);
		}

		po::notify(variables_);

		if (variables_["version"].as<bool>()) {
			display_version();
			exit(EXIT_SUCCESS);
		}

		if (variables_["debug-options"].as<bool>()) {
			display_variables();
			exit(EXIT_SUCCESS);
		}

		if (command().empty()) {
			display_usage(visible_opts);
			exit(EX_USAGE);
		}
	} catch (std::exception &e) {
		print_error(format("%1%"), e);
		// i18n: %1 = program name; %2 = help argument
		cerr << format(_("Try '%1% %2%' for more information.")) % display_name_ % "-h" << endl;
		exit(EX_USAGE);
	}

	cron_mode_ |= flag("cron");
}

void CommandLine::display_usage(const po::options_description &options) const {
	// i18n: %1 = program name
	cout << format(_("Usage: %1% [OPTION]... COMMAND [ARG]...")) % display_name_ << "\n\n";
	if (cron_mode()) {
		cout << _( // i18n: Line wrap to a maximum of 79 characters.
			"Run COMMAND with standard output and standard error copied to each FILE\n"
			"suppressing all output unless the process outputs an error message or has a\n"
			"non-zero exit status whereupon the original output will be written as normal\n"
			"and the exit code will be appended to standard error.\n");
	} else {
		cout << _( // i18n: Line wrap to a maximum of 79 characters.
			"Run COMMAND with standard output and standard error copied to each FILE\n"
			"while maintaining the original standard output and standard error as normal.\n");
	}
	cout << "\n" << options << endl;
}

void CommandLine::display_version() const {
	cout << DEFAULT_PROGRAM_NAME << " " << VERSION << "\n";
	// i18n: %1 = list of years; %2 = name
	cout << format(_("Copyright %1%  %2%\n")) % "2018-2021" % "Simon Arlott";
	// i18n: %1 = licence URL
	cout << format(_( // i18n: Line wrap to a maximum of 79 characters.
		"Licence GPLv3+: GNU GPL version 3 or later <%1%>.\n"
		"This program comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law.\n"
		"This is free software: you are free to change and redistribute it.\n"))
		% "https://gnu.org/licenses/gpl.html";

	// i18n: Prefix credits with "Translation: xx_XX\n"
	auto translator_credits_noop = N_("translator-credits");
	auto translator_credits = _(translator_credits_noop);
	if (translator_credits != translator_credits_noop) {
		cout << "\n" << translator_credits << "\n";
	}

	cout << flush;
}

void CommandLine::display_variables() const {
	for (const auto& variable : variables_) {
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
	return variables_[name].as<bool>();
}

const vector<string>& CommandLine::list(const string &name) const {
	const auto& value = variables_[name];

	if (!value.empty()) {
		return value.as<vector<string>>();
	} else {
		return empty_list_;
	}
}

} // namespace dtee
