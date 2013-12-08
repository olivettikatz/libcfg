#include "Cfg.h"

namespace cfg
{
	Cfg::Type Cfg::Option::getType()
	{
		return type;
	}

	string Cfg::Option::getKey()
	{
		return key;
	}

	bool Cfg::Option::toBool()
	{
		return valueBool;
	}

	double Cfg::Option::toDouble()
	{
		return valueDouble;
	}

	string Cfg::Option::toString()
	{
		return valueString;
	}

	Cfg::Option &Cfg::Option::restrictDouble(double min, double max)
	{
		valueDoubleMin = min;
		valueDoubleMax = max;
		return *this;
	}

	Cfg::Option &Cfg::Option::restrictString(vector<string> range)
	{
		valueStringRange = range;
		return *this;
	}

	Cfg::Option &Cfg::Option::restrictString(string s)
	{
		valueStringRange.push_back(s);
		return *this;
	}

	Cfg::Option &Cfg::Option::restrictStringLength(size_t l)
	{
		valueStringLength = (int)l;
		return *this;
	}

	Cfg::Option &Cfg::Option::describe(string d)
	{
		description = d;
		return *this;
	}

	bool Cfg::Option::isValid()
	{
		if (type == Double)
		{
			if (valueDouble < valueDoubleMin)
				return false;
			
			if (valueDouble > valueDoubleMax)
				return false;

			return true;
		}
		else if (type == String)
		{
			bool match = false;

			if (valueStringRange.empty() == false)
			{
				for (vector<string>::iterator i = valueStringRange.begin(); i != valueStringRange.end(); i++)
				{
					if (i->compare(valueString) == 0)
					{
						match = true;
						break;
					}
				}

				if (valueStringLength >= 0)
				{
					if (valueString.size() != valueStringLength)
					{
						return false;
					}
				}

				return match;
			}
		}
		else
		{
			return true;
		}

		return true;
	}

	Cfg::Option &Cfg::Option::parse(string from)
	{
		if (type == Bool)
		{
			if (from.compare("true") == 0)
			{
				valueBool = true;
			}
			else if (from.compare("false") == 0)
			{
				valueBool = false;
			}
			else
			{
				CFG_ERROR("invalid value for Bool type option ('" << from << "', valid are 'true' and 'false').");
			}
		}
		else if (type == Double)
		{
			valueDouble = atof(from.c_str());
		}
		else
		{
			if (from.empty() == false && from[0] == '"')
				valueString = from.substr(1, from.size()-2);
			else
				valueString = from;
		}

		return *this;
	}

	string Cfg::Option::display()
	{
		stringstream ss;
		if (type == Bool)
			ss << "bool";
		else if (type == Double)
			ss << "double";
		else if (type == String)
			ss << "string";
		ss << " " << key;

		if (type == Double && (valueDoubleMin != doubleMin || valueDoubleMax != doubleMax))
		{
			ss << " (range " << valueDoubleMin << "-" << valueDoubleMax << ")";
		}
		else if (type == String)
		{
			if (valueStringLength >= 0)
				ss << " (must be " << valueStringLength << " char(s) long)";

			if (valueStringRange.empty() == false)
			{
				ss << " (can be";
				for (vector<string>::iterator i = valueStringRange.begin(); i != valueStringRange.end(); i++)
				{
					ss << " '" << *i << "', ";
				}
				string tmp = ss.str();
				tmp = tmp.substr(0, tmp.size()-2);
				ss.str(tmp);
				ss << ")";
			}
		}

		ss << ": " << description;
		return ss.str();
	}

	bool Cfg::containsOnly(string s, string only)
	{
		for (string::iterator i = s.begin(); i != s.end(); i++)
		{
			if (only.find(*i) == string::npos)
			{
				return false;
			}
		}

		return true;
	}

	vector<string> Cfg::splitString(string s, char d)
	{
		vector<string> rtn;
		string buf;

		for (string::iterator i = s.begin(); i != s.end(); i++)
		{
			if (*i == d)
			{
				if (buf.empty() == false)
				{
					rtn.push_back(buf);
					buf = "";
				}
			}
			else
			{
				buf += *i;
			}
		}

		if (buf.empty() == false)
		{
			rtn.push_back(buf);
		}

		return rtn;
	}

	string Cfg::stripString(string s)
	{
		string tmp = s;
		while(tmp[0] == ' ' || tmp[0] == '\t')
			tmp = tmp.substr(1);
		while(tmp.back() == ' ' || tmp.back() == '\t')
			tmp.pop_back();
		return tmp;
	}

	void Cfg::shorthand(string s, string l)
	{
		shorthandMapping[s] = l;
	}

	void Cfg::add(Cfg::Option o)
	{
		content.push_back(o);
	}

	bool Cfg::has(string key)
	{
		for (vector<Option>::iterator i = content.begin(); i != content.end(); i++)
		{
			if (i->key.compare(key) == 0)
			{
				return true;
			}
		}

		return false;
	}

	Cfg::Option &Cfg::get(string key)
	{
		for (vector<Option>::iterator i = content.begin(); i != content.end(); i++)
		{
			if (i->key.compare(key) == 0)
			{
				return *i;
			}
		}

		return *content.end();
	}

	string Cfg::display()
	{
		stringstream ss;
		for (vector<Option>::iterator i = content.begin(); i != content.end(); i++)
		{
			ss << "--" << i->key << "\n";
			ss << "  " << i->display() << "\n";
		}
		return ss.str();
	}

	vector<string> Cfg::fromArguments(int *argc, char ***argv)
	{
		hasFromArguments = true;

		if (name.empty())
			CFG_FATAL("Cfg has no name.");

		if (version.empty())
			CFG_FATAL("Cfg has no version.");

		vector<string> rtn;

		if (argc == NULL)
		{
			CFG_FATAL("cannot read null arguments.");
		}

		if (argv == NULL)
		{
			CFG_FATAL("cannot read null arguments.");
		}

		int c = *argc;
		vector<string> v;

		for (int i = 0; i < c; i++)
		{
			v.push_back((*argv)[i]);
		}

		for (int i = 0; i < c; i++)
		{
			if (v[i].compare("--help") == 0)
			{
				cout << name << " " << version << "\n";

				if (author.empty() == false)
					cout << author << "\n";
				if (copyright.empty() == false)
					cout << copyright << "\n";

				if (shorthandMapping.empty() == false)
					cout << "\n";
				for (map<string, string>::iterator i = shorthandMapping.begin(); i != shorthandMapping.end(); i++)
				{
					cout << "-" << i->first << " --" << i->second << "\n";
				}

				cout << "\n" << display() << "\n";
				_exit(0);
			}
			else if (v[i].compare("--version") == 0)
			{
				cout << name << " " << version << "\n";
				_exit(0);
			}

			if (v[i][0] == '-')
			{
				string arg, val;

				if (v[i].size() == 1)
				{
					cerr << "error: '-' is not a valid argument.\n";
					_exit(1);
				}
				else if (v[i][1] == '-')
				{
					if (v[i].find("=") != string::npos)
					{
						vector<string> tmp = splitString(v[i].substr(2), '=');
						arg = tmp[0];
						val = tmp[1];
					}
					else
					{
						arg = v[i].substr(2);
					}
				}
				else
				{
					string sht = string(1, v[i][1]);
					if (shorthandMapping.find(sht) == shorthandMapping.end())
					{
						cerr << "error: unknown argument '-" << sht << "'.\n";
						_exit(1);
					}

					arg = shorthandMapping[sht];
					if (has(arg) == false)
					{
						CFG_FATAL("no such option for shorthand mapping to '" << arg << "'.");
					}
					Option *ref = &get(arg);
					if (ref->getType() != Bool)
					{
						if (i+1 >= c)
						{
							cerr << "error: argument '-" << sht << "' (long: '--" << arg << "') expects an argument.\n";
							_exit(1);
						}

						if (v[i+1][0] == '-')
						{
							cerr << "error: argument '-" << sht << "' (long: '--" << arg << "') expects an argument, not another paramter.\n";
							_exit(1);
						}

						val = v[i+1];
						i++;
					}
				}

				if (has(arg) == false)
				{
					cerr << "error: no such argument '--" << arg << "'.\n";
					_exit(1);
				}

				Option *ref = &get(arg);
				if (ref->getType() != Bool && val.empty())
				{
					cerr << "error: configuration key '" << arg << "' requires parameter (non-bool).\n";
					_exit(1);
				}

				if (ref->getType() == Bool)
				{
					ref->valueBool = !ref->valueBool;
				}
				else if (ref->getType() == Double)
				{
					ref->parse(val);
				}
				else if (ref->getType() == String)
				{
					ref->parse(val);
				}
			}
			else
			{
				rtn.push_back(v[i]);
			}
		}

		return rtn;
	}

	void Cfg::fromFile(string path, bool ignoreOrderChecking)
	{
		if (hasFromArguments && ignoreOrderChecking == false)
			CFG_FATAL("cannot call fromFile after fromArguments.");

		if (fileExists(path) == false)
		{
			CFG_ERROR("cannot open configuration file '" << path << "'.");
			return ;
		}

		string page = loadFile(path);
		vector<string> lines = splitString(page, '\n');

		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i].empty() || lines[i][0] == '#')
				continue;

			if (lines[i].find("=") != string::npos)
			{
				vector<string> tmp = splitString(lines[i], '=');
				string arg = stripString(tmp[0]);
				string val = stripString(tmp[1]);

				if (arg.empty())
				{
					CFG_ERROR("error [" << path << ":" << i << "]: invalid key ''.");
					continue;
				}

				if (has(arg) == false)
				{
					CFG_ERROR("error [" << path << ":" << i << "]: unknown key '" << arg << "'.");
					continue;
				}

				Option *ref = &get(arg);
				if (ref->getType() != Bool && val.empty())
				{
					CFG_ERROR("error [" << path << ":" << i << "]: configuration option '" << arg << "' requires a value (non-bool).");
					continue;
				}

				if (ref->getType() == Bool)
				{
					ref->valueBool = !ref->valueBool;
				}
				else if (ref->getType() == Double)
				{
					ref->parse(val);
				}
				else if (ref->getType() == String)
				{
					ref->parse(val);
				}
			}
		}
	}
}