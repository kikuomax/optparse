#include "optparse/DefaultFormatter.h"
#include "optparse/DefaultUsagePrinter.h"
#include "optparse/OptionParserBase.h"
#include "optparse/optparse.h"

#include <cwchar>
#include <iostream>

// this example is compiled for `wchar_t` if `WCHAR_EXAMPLE` is defined
//  - Char: character type
//  - String: string of Char
//  - stdOut: standard output of Char
//  - stdErr: standard error of Char
//  - STR: macro which wraps a string literal
//  - fromChar: function which converts char into wchar_t
//
#ifdef WCHAR_EXAMPLE
typedef wchar_t Char;
typedef std::wstring String;
std::wostream& stdOut = std::wcout;
std::wostream& stdErr = std::wcerr;
#define STR(str)  L ## str
inline static String fromChar(const std::string& str) {
	return String(str.begin(), str.end());
}
#else
typedef char Char;
typedef std::string String;
std::ostream& stdOut = std::cout;
std::ostream& stdErr = std::cerr;
#define STR(str)  str
inline static const String& fromChar(const String& str) {
	return str;
}
#endif

/** Options for the test. */
struct Options {
	/** Whether the version information is required. `false` by default. */
	bool isVersionRequired;

	/** Numeric option. 0 by default. */
	int optionalNumber;

	/** String option. Empty by default. */
	String optionalString;

	/** Numberic argument. 0 by default. */
	int positionalNumber;

	/** String argument. Empty by default. */
	String positionalString;

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
static void setDebugLevel(Options& options, const int& level) {
	stdOut << STR("set debug level to ") << level << std::endl;
}

/** Sets the global flag. */
static void setGlobalFlag(Options& options) {
	stdOut << STR("set global flag") << std::endl;
}

/** Runs a test. */
int main(int argc, char** argv) {
	optparse::OptionParserBase< Options, Char, optparse::DefaultFormatter >
		parser(STR("Example program"));
	try {
		// optional arguments
		parser.addOption(
			STR("-h"), STR("prints usage"), &triggerHelp);
		parser.addOption(
			STR("-d"), STR("sets debug level"), &setDebugLevel);
		parser.addOption(
			STR("-g"), STR("sets global flag"), &setGlobalFlag);
		parser.addOption(
			STR("--number"), STR("NUM"), STR("optional numberic value"),
			&Options::optionalNumber);
		parser.addOption(
			STR("--string"), STR("optional string value"),
			&Options::optionalString);
		parser.addOption(
			STR("--version"), STR("prints version information and exits"),
			&Options::isVersionRequired, true);
		// positional arguments
		parser.appendArgument(
			STR("P0"), STR("positonal numeric value"),
			&Options::positionalNumber);
		parser.appendArgument(
			STR("POS1"), STR("positional string value"),
			&Options::positionalString);
#ifdef WCHAR_EXAMPLE
		struct Args {
			int argc;
			wchar_t** argv;

			Args(int argc, char** argv) : argc(argc) {
				this->argv = new wchar_t*[argc];
				for (int i = 0; i < argc; ++i) {
					size_t n = strlen(argv[i]) + 1;
					this->argv[i] = new wchar_t[n];
					std::copy(argv[i], argv[i] + n, this->argv[i]);
				}
			}

			~Args() {
				for (int i = 0; i < this->argc; ++i) {
					delete[] this->argv[i];
				}
				delete[] this->argv;
			}
		};
		Args argWrapper(argc, argv);
		wchar_t** args = argWrapper.argv;
#else
		char** args = argv;
#endif
		Options options = parser.parse(argc, args);
		if (options.isVersionRequired) {
			stdOut
				<< STR("version: ")
				<< fromChar(OPTPARSE_VERSION)
				<< std::endl;
			return 0;
		}
		stdOut
			<< STR("optional number: ")
			<< options.optionalNumber
			<< std::endl;
		stdOut
			<< STR("optional string: ")
			<< options.optionalString
			<< std::endl;
		stdOut
			<< STR("positional number: ")
			<< options.positionalNumber
			<< std::endl;
		stdOut
			<< STR("positional string: ")
			<< options.positionalString
			<< std::endl;
	} catch (optparse::TooFewArguments& ex) {
		stdErr << STR("too few arguments") << std::endl;
		return 1;
	} catch (optparse::TooManyArguments& ex) {
		stdErr << STR("too many arguments") << std::endl;
		return 1;
	} catch (optparse::ValueNeeded< Char >& ex) {
		stdErr << ex.getLabel() << STR(" needs a value") << std::endl;
		return 1;
	} catch (optparse::BadValue< Char >& ex) {
		stdErr
			<< ex.getValue()
			<< STR(" is invalid for ")
			<< ex.getLabel()
			<< std::endl;
		return 1;
	} catch (optparse::UnknownOption< Char >& ex) {
		stdErr << STR("unknown option: ") << ex.getLabel();
		return 1;
	} catch (optparse::HelpNeeded& ex) {
		optparse::DefaultUsagePrinter< Char > printer;
		printer.printUsage(parser);
		return 1;
	} catch (optparse::Exception& ex) {
		stdErr << STR("Exception: ") << fromChar(ex.getMessage()) << std::endl;
	}
	return 0;
}
