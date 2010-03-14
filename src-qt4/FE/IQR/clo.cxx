/*
 * This file was automaticly generated by clo++ version 0.6.1 (REL-0_6_1)
 * http://pmade.org/pjones/software/clo++/
 */

/** @file
 * This file contains the implementation of the parser class.
 * You can control the name of this source file using the
 * cxx_source_file variable.
**/

#include "clo.h"
#include <cstring>
#include <cstdlib>
#include <cctype>

namespace {
    const char const_usage[] =
"  -c, --config string          configuration file to open\n"
"  -f, --file string            system file to open\n"
"  -l, --logging file | syslog  Where to write logging messages to\n"
"  -p, --harbor string          Harbor configuration file to open\n"
"  -r, --run                    automatically start simulation\n"
"  -v, --version                show version\n";

    const char const_help_comment[] =
"use the -h option for help";

    const char* expand_long_name (const std::string &name);
}

//#########################################################################
clo::parser::parser (void)
    : state_(state_option)
{
    std::memset(&locations_, 0, sizeof(locations_));
}
//#########################################################################
void clo::parser::parse (int argc, char *argv[], bool call_finalize) {
    for (int i=1; i<argc; ++i) parse_element(argv[i], i, source_cl);
    if (call_finalize) finalize();
}
//#########################################################################
void clo::parser::finalize (void) {
    if (state_ == state_value) {
	switch (openum_) {
	    case option_autorun:
		throw option_error("missing value for 'run' option");
	    case option_configfile:
		throw option_error("missing value for 'config' option");
	    case option_harborconfigfile:
		throw option_error("missing value for 'harbor' option");
	    case option_logging:
		throw option_error("missing value for 'logging' option");
	    case option_systemfile:
		throw option_error("missing value for 'file' option");
	    case option_version:
		throw option_error("missing value for 'version' option");
	}

    }

}
//#########################################################################
void clo::parser::parse_element (const char *element, int position, opsource source) {
    std::size_t length = std::strlen(element);

    switch (state_) {
	case state_consume:
	    non_options_.push_back(element);
	    break;
	case state_option:
	    if (length >= 2 && element[0] == '-' && element[1] == '-') {
		if (length == 2) { state_ = state_consume; return; }
		element += 2;
		const char *value = element;
		while (*value != 0 && *value != '=') ++value;
		if (*value == '=') {
		    std::string selement(element, value - element), svalue(++value);

		    parse_long_option(selement.c_str(), position, source);
		    if (state_ != state_value) {
			std::string error("the '"); error += element; error += "' option does not take a value";
			throw option_error(error);
		    }

		    parse_value(svalue.c_str());
		    state_ = state_option;
		} else {
		    parse_long_option(element, position, source);
		}
	    } else if (length >= 2 && element[0] == '-') {
		++element;

		if (length > 2) {
		    while (*element != 0) {
			parse_short_option(*element, position, source);
			++element;
	    
			if (state_ == state_value && *element != 0) {
			    parse_value(element);
			    state_ = state_option;
			    break;
			}
		    }
		} else {
		    parse_short_option(*element, position, source);
		}
	    } else {
		non_options_.push_back(element);
	    }
	    break;
	case state_value:
	    parse_value(element);
	    state_ = state_option;
	    break;
    }
}
//#########################################################################
void clo::parser::parse_short_option (char option, int position, opsource source) {
switch (option) {
	case 'c':
    	    openum_ = option_configfile;
	    state_ = state_value;
	    locations_.configfile = position;
    	    return;
	case 'f':
    	    openum_ = option_systemfile;
	    state_ = state_value;
	    locations_.systemfile = position;
    	    return;
	case 'l':
    	    openum_ = option_logging;
	    state_ = state_value;
	    locations_.logging = position;
    	    return;
	case 'p':
    	    openum_ = option_harborconfigfile;
	    state_ = state_value;
	    locations_.harborconfigfile = position;
    	    return;
	case 'r':
    	    openum_ = option_autorun;
	    options_.autorun = !options_.autorun;
	    locations_.autorun = position;
    	    return;
	case 'v':
    	    openum_ = option_version;
	    options_.version = !options_.version;
	    locations_.version = position;
    	    return;
	case 'h':
    	    if (source != source_cl) break;
    	    throw autoexcept(autothrow_help, const_usage);
	case '?':
    	    if (source != source_cl) break;
    	    throw autoexcept(autothrow_help, const_usage);
}

    std::string error("unknown option: '"); error += option; error += "'";
    throw option_error(error);
}
//#########################################################################
void clo::parser::parse_long_option (const char *option, int position, opsource source) {
    option = expand_long_name(option);

if (std::strcmp(option, "config") == 0) {
		openum_ = option_configfile;
		locations_.configfile = position;
state_ = state_value;
return;
} else if (std::strcmp(option, "file") == 0) {
		openum_ = option_systemfile;
		locations_.systemfile = position;
state_ = state_value;
return;
} else if (std::strcmp(option, "harbor") == 0) {
		openum_ = option_harborconfigfile;
		locations_.harborconfigfile = position;
state_ = state_value;
return;
} else if (std::strcmp(option, "logging") == 0) {
		openum_ = option_logging;
		locations_.logging = position;
state_ = state_value;
return;
} else if (std::strcmp(option, "run") == 0) {
		openum_ = option_autorun;
		locations_.autorun = position;
options_.autorun = !options_.autorun;
return;
} else if (std::strcmp(option, "version") == 0) {
		openum_ = option_version;
		locations_.version = position;
options_.version = !options_.version;
return;
} else if (source == source_cl && std::strcmp(option, "help") == 0) {
		throw autoexcept(autothrow_help, const_usage);
}

    std::string error("unknown option '"); error += option; error += "'";
    throw option_error(error);
}
//#########################################################################
void clo::parser::parse_value (const char *value) {
switch (openum_) {
	case option_autorun:
	    break;
	case option_configfile:
	    {
		options_.configfile = value;
	    }
	    break;
	case option_harborconfigfile:
	    {
		options_.harborconfigfile = value;
	    }
	    break;
	case option_logging:
	    {
		logging_enum evalue;

		if (std::strcmp(value, "file") == 0) {
    		    evalue = logging_file;
		} else if (std::strcmp(value, "syslog") == 0) {
    		    evalue = logging_syslog;
		} else {
    		    std::string error("'"); error += value; error += "' is an invalid value for the 'logging' option";
    		    throw option_error(error);
    		}

		options_.logging = evalue;
	    }
	    break;
	case option_systemfile:
	    {
		options_.systemfile = value;
	    }
	    break;
	case option_version:
	    break;
}
}
//#########################################################################
const char* clo::option_error::get_help_comment (void) const {
    return const_help_comment;
}
//#########################################################################
namespace {
    const char* expand_long_name (const std::string &name) {
	std::string::size_type name_size = name.size();
	std::vector<const char*> matches;

    if (name_size <= 6 && name.compare(0, name_size, "config", name_size) == 0)
        	matches.push_back("config");

    if (name_size <= 4 && name.compare(0, name_size, "file", name_size) == 0)
        	matches.push_back("file");

    if (name_size <= 6 && name.compare(0, name_size, "harbor", name_size) == 0)
        	matches.push_back("harbor");

    if (name_size <= 7 && name.compare(0, name_size, "logging", name_size) == 0)
        	matches.push_back("logging");

    if (name_size <= 3 && name.compare(0, name_size, "run", name_size) == 0)
        	matches.push_back("run");

    if (name_size <= 7 && name.compare(0, name_size, "version", name_size) == 0)
        	matches.push_back("version");

    if (name_size <= 4 && name.compare(0, name_size, "help", name_size) == 0)
        	matches.push_back("help");


	if (matches.empty()) {
	    std::string error("unknown option '"); error += name; error += "'";
	    throw clo::option_error(error);
	}

	if (matches.size() == 1) {
	    return matches[0];
	}

	std::string error("the option name '"); error += name; error += "' is ambiguous";
	throw clo::option_error(error);
    }
} // end anonymous namespace
