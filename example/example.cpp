#include "optparse/DefaultFormatter.h"
#include "optparse/DefaultUsagePrinter.h"
#include "optparse/OptionParserBase.h"
#include "optparse/optparse.h"

#include <iostream>

/** Options for the test. */
struct Options {
	/** Whether the version information is required. `false` by default. */
	bool isVersionRequired;

	/** Numeric option. 0 by default. */
	int optionalNumber;

	/** String option. Empty by default. */
	std::string optionalString;

	/** Numberic argument. 0 by default. */
	int positionalNumber;

	/** String argument. Empty by default. */
	std::string positionalString;

	/** Initializes with default values. */
	Options()
		: isVersionRequired(false),
		  optionalNumber(0),
		  positionalNumber(0) {}
};

/** Triggers a help message. */
static void triggerHelp(Options& options) {
	throw optparse::HelpNeeded();
}

/** Sets the debug level. */
static void setDebugLevel(Options& options, int level) {
	std::cout << "set debug level to " << level << std::endl;
}

/** Sets the global flag. */
static void setGlobalFlag(Options& options) {
	std::cout << "set global flag" << std::endl;
}

// runs a test
int main(int argc, char** argv) {
	optparse::OptionParserBase< Options,
								char,
								optparse::DefaultFormatter > parser;
	try {
		// optional arguments
		parser.addOption("-h", "prints usage", &triggerHelp);
		parser.addOption("-d", "sets debug level", &setDebugLevel);
		parser.addOption("-g", "sets global flag", &setGlobalFlag);
		parser.addOption("--number", "optional numberic value",
						 &Options::optionalNumber);
		parser.addOption("--string", "optional string value",
						 &Options::optionalString);
		parser.addOption("--version", "prints version information and exits",
						 &Options::isVersionRequired, true);
		// positional arguments
		parser.appendArgument("pos0", "positonal numeric value",
							  &Options::positionalNumber);
		parser.appendArgument("pos1", "positional string value",
							  &Options::positionalString);
		Options options = parser.parse(argc, argv);
		if (options.isVersionRequired) {
			std::cout << "version: " << OPTPARSE_VERSION << std::endl;
			return 0;
		}
		std::cout << "optional number: "
				  << options.optionalNumber << std::endl;
		std::cout << "optional string: "
				  << options.optionalString << std::endl;
		std::cout << "positional number: "
				  << options.positionalNumber << std::endl;
		std::cout << "positional string: "
				  << options.positionalString << std::endl;
	} catch (optparse::TooFewArguments& ex) {
		std::cerr << "too few arguments" << std::endl;
		return 1;
	} catch (optparse::TooManyArguments& ex) {
		std::cerr << "too many arguments" << std::endl;
		return 1;
	} catch (optparse::ValueNeeded< char >& ex) {
		std::cerr << ex.getLabel() << " needs a value" << std::endl;
		return 1;
	} catch (optparse::BadValue< char >& ex) {
		std::cerr << ex.getValue() << " is invalid for " << ex.getLabel()
			<< std::endl;
		return 1;
	} catch (optparse::UnknownOption< char >& ex) {
		std::cerr << "unknown option: " << ex.getLabel();
		return 1;
	} catch (optparse::HelpNeeded& ex) {
		optparse::DefaultUsagePrinter< char > printer;
		printer.printUsage(parser);
		return 1;
	} catch (optparse::Exception& ex) {
		std::cerr << "Exception: " << ex.getMessage() << std::endl;
	}
	return 0;
}
