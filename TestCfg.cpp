#include "Cfg.h"

using namespace cfg;

int main(int argc, char *argv[])
{
	CFG_TEST_HEADER("Cfg");
	Cfg c;
	c.name = "TestCfg";
	c.version = "0.01-TEST";
	c.shorthand("o", "an-option");
	c.add(Cfg::Option(Cfg::Bool, "an-option").describe("This is an option."));
	c.add(Cfg::Option(Cfg::String, "another-option").describe("This is another option."));
	c.fromFile("ConfigurationFile.example");
	c.fromArguments(&argc, &argv);
	CFG_TEST_ASSERT(c.get("an-option").toBool(), true);
	CFG_TEST_ASSERT(c.get("another-option").toString().compare("hi"), 0);
	return 0;
}
