/* Copyright 2013 Oliver Katz
 *
 * This file is part of LibCfg.
 *
 * LibCfg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * LibCfg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with LibCfg.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file Util.h
  *  \brief Basic utilities for all of LibCfg.
  *  Util.h contains console message formatting, macros to display debug messages,
  *  warnings, non-fatal errors and fatal errors. It also contains the testing system,
  *  which is very primitive, but hey. Lastly, it contains file IO for the basic loading
  *  of configuration/script files. */

/*! \def CFG_MESSAGE_STREAM
 *  \brief The IO stream to output console messages to.
 *  This can easily be changed to cerr, if stdout conflicts with other messages. */

/*! \def CFG_MESSAGE_PREFIX
 *  \brief The prefix to use for all console messages.
 *  Contains the file and line number location that the message originated from. */

/*! \def CFG_MESSAGE_PREFIX_ERROR
 *  \brief The prefix to use for all errors. */

/*! \def CFG_MESSAGE_PREFIX_WARNING
 *  \brief The prefix to use for all warnings. */

/*! \def CFG_MESSAGE_PREFIX_DEBUG
 *  \brief The prefix to use for all debug messages. */

/*! \def CFG_MESSAGE_PREFIX_TEST
 *  \brief The prefix to use for all test message. */

/*! \def CFG_MESSAGE_DISPLAY(msg)
 *  \brief Displays text in \a msg to console. 
 *  Displays text with no prefix to stream defined in CFG_MESSAGE_STREAM. */

/*! \def CFG_FATAL(msg)
 *  \brief Displays fatal error to console, then halts program.
 *  After it displays the fatal error, it will throw a std::runtime_error
 *  exception, so that the source of the fatal error may be backtraced
 *  with a debugger. */

/*! \def CFG_ERROR(msg)
 *  \brief Displays error to console, but doesn't halt program.
 *  Fun fact: the output to the console is identical to that of CFG_FATAL. */

/*! \def CFG_WARNING(msg)
 *  \brief Displays a warning to console. */

/*! \def CFG_DEBUG(msg)
 *  \brief Displays a debug message to console. */

/*! \def CFG_TEST_HEADER(name)
 *  \brief Denotes the beginning of a test.
 *  When you are testing something in a test file, use this at the beginning of a test
 *  to tell the user that a new test is starting.
 *
 *  For example:
 *  \include TestUtil.cpp */

/*! \def CFG_TEST_ASSERT(expected, recieved)
 *  \brief Displays the result of a test.
 *  Displays the value of \a expected and \a recieved, as well as the code for \a recieved
 *  so that the user can see what the test was for. */

#ifndef __LIB_CFG_UTIL_H
#define __LIB_CFG_UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#define CFG_MESSAGE_STREAM cout
#define CFG_MESSAGE_PREFIX "[UnixEscape "<<__FILE__<<":"<<__LINE__<<"] "
#define CFG_MESSAGE_PREFIX_ERROR CFG_MESSAGE_PREFIX<<"error: "
#define CFG_MESSAGE_PREFIX_WARNING CFG_MESSAGE_PREFIX<<"warning: "
#define CFG_MESSAGE_PREFIX_DEBUG CFG_MESSAGE_PREFIX<<"debug: "
#define CFG_MESSAGE_PREFIX_TEST CFG_MESSAGE_PREFIX<<"test: "
#define CFG_MESSAGE_DISPLAY(msg) {CFG_MESSAGE_STREAM<<msg<<"\n";}

#define CFG_FATAL(msg) {CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_ERROR<<msg);throw std::runtime_error("internal unix escape error");}
#define CFG_ERROR(msg) {CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_ERROR<<msg);}
#define CFG_WARNING(msg) {CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_WARNING<<msg);}
#define CFG_DEBUG(msg) {CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_WARNING<<msg);}

#define CFG_TEST_HEADER(name) {CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_TEST<<"running '"<<name<<"'");}
#define CFG_TEST_ASSERT(expected, recieved) { \
	std::stringstream _ssExpected; \
	_ssExpected << expected; \
	std::stringstream _ssRecieved; \
	_ssRecieved << recieved; \
	if (_ssExpected.str().compare(_ssRecieved.str()) == 0) \
	{ \
		CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_TEST << "    '" << #expected << "' succeeded ('" << _ssExpected.str() << "' == '" << _ssRecieved.str() << "')."); \
	} \
	else \
	{ \
		CFG_MESSAGE_DISPLAY(CFG_MESSAGE_PREFIX_TEST << "    '" << #expected << "' failed ('" << _ssExpected.str() << "' != '" << _ssRecieved.str() << "')."); \
	} \
}

/*! Namespace for all LibCfg classes/methods/global variables. */
namespace cfg
{
	using namespace std;

	/*! Tests to see if a file exists.
	 *  \param path The path of the file. */
	bool fileExists(string path);

	/*! Reads the contents of a text file into memory and returns it as a string.
	 *  \param path The path of the file. */
	string loadFile(string path);
}

#endif