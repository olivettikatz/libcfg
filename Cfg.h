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

/*! \file Cfg.h
 *  \brief Contains the Cfg class.
 *  The Cfg class contains configuration data which can be read from command line arguments
 *  or configuration files. */

#ifndef __LIB_CFG_CFG_H
#define __LIB_CFG_CFG_H

#include "Util.h"
#include <float.h>
#include <unistd.h>

namespace cfg
{
	using namespace std;

	/*! minimum value for a double. */
	static double doubleMin = -DBL_MAX;

	/*! maximum value for a double. */
	static double doubleMax = DBL_MAX;

	/*! Class for storing configuration data. */
	class Cfg
	{
	public:
		/*! The type of an option. */
		typedef enum
		{
			Bool,
			Double,
			String
		} Type;

		/*! Subtype for storing options. */
		typedef struct Option
		{
			/*! The type of the option. */
			Type type;

			/*! The key name. */
			string key;

			/*! The boolean value. */
			bool valueBool;

			/*! The numeric value. */
			double valueDouble;

			/*! The numeric value range. */
			double valueDoubleMin, valueDoubleMax;

			/*! The string value. */
			string valueString;

			/*! The string value range. */
			vector<string> valueStringRange;

			/*! The string value length requirement (-1 if none). */
			int valueStringLength;

			/*! The description. */
			string description;

			/*! Constructor. */
			Option(Type t = Bool, string k = "") : type(t), key(k), valueBool(false), valueDouble(0.0), valueDoubleMin(doubleMin), valueDoubleMax(doubleMax), valueStringLength(-1) {}

			/*! Gets the type. */
			Type getType();

			/*! Gets the key. */
			string getKey();

			/*! Gets the boolean value. */
			bool toBool();

			/*! Gets the numeric value. */
			double toDouble();

			/*! Gets the string value. */
			string toString();

			/*! Sets the double range. */
			Option &restrictDouble(double min = doubleMin, double max = doubleMax);

			/*! Sets the string range. */
			Option &restrictString(vector<string> range);

			/*! Adds \a s to the string range. */
			Option &restrictString(string s);

			/*! Adds a length restriction to the string value. */
			Option &restrictStringLength(size_t l);

			/*! Adds a description. */
			Option &describe(string d);

			/*! Checks if the current value is valid. */
			bool isValid();

			/*! Parses value from a string. */
			Option &parse(string from);

			/*! Displays the option. */
			string display();
		} Option;

	protected:
		bool hasFromArguments;
		vector<Option> content;
		map<string, string> shorthandMapping;

		static bool containsOnly(string s, string only);
		static vector<string> splitString(string s, char d);
		static string stripString(string s);

	public:
		string name, version, author, copyright;

		/*! Constructor. */
		Cfg() : hasFromArguments(false) {}

		/*! Sets shorthand version of \a s to \a l, for argument parsing. */
		void shorthand(string s, string l);

		/*! Adds an option. */
		void add(Option o);

		/*! Checks if option with \a key exists. */
		bool has(string key);

		/*! Gets option with \a key. */
		Option &get(string key);

		/*! Displays all options. */
		string display();

		/*! Parses from arguments. All non-parameter arguments are returned in a vector. */
		vector<string> fromArguments(int *argc, char ***argv);

		/*! Parses from configuration file. An example configuration file would be:
		 *  \include ConfigurationFile.example */
		void fromFile(string path, bool ignoreOrderChecking = false);
	};
}

#endif