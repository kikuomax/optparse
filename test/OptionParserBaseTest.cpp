#include "optparse/DefaultFormatter.h"
#include "optparse/OptionParserBase.h"

#include "gtest/gtest.h"

TEST(OptionParserBaseTest, char_description_can_be_set) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_EQ("test program", parser.getDescription());
}

TEST(OptionParserBaseTest, wchar_t_description_can_be_set) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, wchar_t, optparse::DefaultFormatter >
		parser(L"test program");
	ASSERT_EQ(L"test program", parser.getDescription());
}

TEST(OptionParserBaseTest, program_name_should_be_empty_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_EQ("", parser.getProgramName());
}

TEST(OptionParserBaseTest, int_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("--int", "NUM", "test int field", &Dummy::field);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("--int", parser.getOption(0).getLabel());
	EXPECT_EQ("test int field", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("NUM", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, string_field_option_can_be_added) {
	struct Dummy {
		std::string field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("-s", "name", "test string field", &Dummy::field);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-s", parser.getOption(0).getLabel());
	EXPECT_EQ("test string field", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("name", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_int_option_field_does_not_start_with_dash) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("o", "INT", "test int field", &Dummy::field),
		optparse::ConfigException);
}

TEST(OptionParserBaseTest, const_int_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("-o", "test const int field", &Dummy::field, 1);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-o", parser.getOption(0).getLabel());
	EXPECT_EQ("test const int field", parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ("", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, const_string_field_option_can_be_added) {
	struct Dummy {
		std::string field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption(
		"-o", "test const string field", &Dummy::field, std::string("s"));
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-o", parser.getOption(0).getLabel());
	EXPECT_EQ("test const string field", parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ("", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_const_int_field_does_not_start_with_dash) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("f", "test const int field", &Dummy::field, 123),
		optparse::ConfigException);
}

TEST(OptionParserBaseTest, int_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("-f", "count", "test int function", func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-f", parser.getOption(0).getLabel());
	EXPECT_EQ("test int function", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("count", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, string_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const std::string&) =
		[](Dummy&, const std::string&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("--fun", "SYMBOL", "test string function", func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("--fun", parser.getOption(0).getLabel());
	EXPECT_EQ("test string function", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("SYMBOL", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_int_function_option_does_not_start_with_dash) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(parser.addOption("xyz", "XYZ", "test int function", func),
				 optparse::ConfigException);
}

TEST(OptionParserBaseTest, function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&) = [](Dummy&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("-s", "test function", func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-s", parser.getOption(0).getLabel());
	EXPECT_EQ("test function", parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ("", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_function_option_does_not_start_with_dash) {
	struct Dummy {};
	void (*func)(Dummy&) = [](Dummy&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(parser.addOption("function", "test function", func),
				 optparse::ConfigException);
}

TEST(OptionParserBaseTest, int_field_option_can_be_replaced_with_string_field_option) {
	struct Dummy {
		int intField;
		std::string stringField;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("-f", "N", "test int field", &Dummy::intField);
	parser.addOption("-f", "STR", "test string field", &Dummy::stringField);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("-f", parser.getOption(0).getLabel());
	EXPECT_EQ("test string field", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("STR", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, int_field_argument_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("INT", "int argument", &Dummy::field);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("int argument", parser.getArgument(0).getDescription());
	EXPECT_EQ("INT", parser.getArgument(0).getValueName());
}

TEST(OptionParserBaseTest, string_field_argument_can_be_added) {
	struct Dummy {
		std::string field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("STRING", "string argument", &Dummy::field);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("string argument", parser.getArgument(0).getDescription());
	EXPECT_EQ("STRING", parser.getArgument(0).getValueName());
}

/** Fixture for the tests that parse options. */
class OptionsParsingTest : public ::testing::Test {
protected:
	/** Options to be substituted. */
	struct Options {
		/** Field associated with "-i". 0 by default. */
		int i;

		/** Field associated with "-s". Empty by default. */
		std::string s;

		/** Field associated with "-C". 0 by default. */
		int C;

		/** Field associated with "-S". Empty by default. */
		std::string S;

		/** Field associated with "--fn". 0 by default. */
		int fn;

		/** Field associated with "--fs". Empty by default. */
		std::string fs;

		/** Field associated with "--flag". false by default. */
		bool flag;

		/** Initializes with default values. */
		Options() : i(0), C(0), fn(0), flag(false) {}

		/** Sets the `fn` field to given value. */
		static void setFn(Options& options, const int& x) {
			options.fn = x;
		}

		/** Sets the `fs` field to given value. */
		static void setFs(Options& options, const std::string& str) {
			options.fs = str;
		}

		/** Turns the `flag` field into `true`. */
		static void setFlag(Options& options) {
			options.flag = true;
		}
	};

	/** Shared option parser. */
	optparse::OptionParserBase<
		Options, char, optparse::DefaultFormatter >* pParser;

	/** Creates the shared option parser. */
	virtual void SetUp() {
		this->pParser = new optparse::OptionParserBase<
			Options, char, optparse::DefaultFormatter >("test program");
		// configures optional fields
		this->pParser->addOption(
			"-i", "N", "int option", &Options::i);
		this->pParser->addOption(
			"-s", "STR", "string option", &Options::s);
		this->pParser->addOption(
			"-C", "const int option", &Options::C, 123);
		this->pParser->addOption(
			"-S", "const string option", &Options::S, std::string("constant"));
		this->pParser->addOption(
			"--fn", "INT", "int function option", &Options::setFn);
		this->pParser->addOption(
			"--fs", "STR", "string function option", &Options::setFs);
		this->pParser->addOption(
			"--flag", "function option", &Options::setFlag);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(OptionsParsingTest, parse_should_remember_program_name) {
	const int ARGC = 1;
	const char* const ARGS[] = { "test.exe" };
	this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("test.exe", this->pParser->getProgramName());
}

TEST_F(OptionsParsingTest, int_field_option_should_substitute_int_field_with_given_value) {
	const int ARGC = 3;
	const char* const ARGS[] = { "test.exe", "-i", "4649" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(4649, options.i);
}

TEST_F(OptionsParsingTest, string_field_option_should_substitute_string_field_with_given_value) {
	const int ARGC = 3;
	const char* const ARGS[] = { "test.exe", "-s", "test" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("test", options.s);
}

TEST_F(OptionsParsingTest, const_int_field_option_should_substitute_int_field_with_constant_if_specified) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "-C" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, options.C);
}

TEST_F(OptionsParsingTest, const_string_field_option_should_substitute_string_field_with_constant_if_specified) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "-S" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("constant", options.S);
}

TEST_F(OptionsParsingTest, int_function_should_be_called_if_specified) {
	const int ARGC = 3;
	const char* const ARGS[] = { "test.exe", "--fn", "99" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(99, options.fn);
}

TEST_F(OptionsParsingTest, string_function_should_be_called_if_specified) {
	const int ARGC = 3;
	const char* const ARGS[] = { "test.exe", "--fs", "function" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("function", options.fs);
}

TEST_F(OptionsParsingTest, flag_function_should_be_called_if_specified) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "--flag" };
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_TRUE(options.flag);
}

TEST_F(OptionsParsingTest, TooFewArguments_should_be_thrown_if_no_arguments_are_given) {
	const int ARGC = 0;
	const char* const ARGS[] = { "ignored" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(OptionsParsingTest, TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "arg" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(OptionsParsingTest, ValueNeeded_should_be_thrown_if_no_value_is_given_to_option_which_needs_value) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "-i" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS),
				 optparse::ValueNeeded< char >);
}

TEST_F(OptionsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_field_option) {
	const int ARGC = 3;
	const char* const ARGS[] = { "test.exe", "-i", "AHO" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}

TEST_F(OptionsParsingTest, UnknownOption_should_be_thrown_if_unknown_option_is_given) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "--unknown" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS),
				 optparse::UnknownOption< char >);
}

/** Fixture for the tests that parse arguments. */
class ArgumentsParsingTest : public ::testing::Test {
protected:
	/** Arguments to be substituted. */
	struct Arguments {
		/** Integer argument. 0 initially. */
		int i;

		/** String argument. Empty initially. */
		std::string s;

		/** Set by `setFn`. 0 initially. */
		int fn;

		/** Set by `setFs`. Empty initially. */
		std::string fs;

		/** Initializes arguments. */
		Arguments() : i(0), fn(0) {}

		/** Sets the `fn` field to a given value. */
		static void setFn(Arguments& args, const int& x) {
			args.fn = x;
		}

		/** Sets the `fs` field to a given string. */
		static void setFs(Arguments& args, const std::string& str) {
			args.fs = str;
		}
	};

	/** Shared option parser. */
	optparse::OptionParserBase<
		Arguments, char, optparse::DefaultFormatter >* pParser;

	/** Creates the shared option parser. */
	virtual void SetUp() {
		this->pParser = new optparse::OptionParserBase<
			Arguments, char, optparse::DefaultFormatter >("test program");
		// configures arguments
		this->pParser->appendArgument("i", "int argument", &Arguments::i);
		this->pParser->appendArgument("s", "string argument", &Arguments::s);
		this->pParser->appendArgument(
			"fn", "int function argument", &Arguments::setFn);
		this->pParser->appendArgument(
			"fs", "string function argument", &Arguments::setFs);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(ArgumentsParsingTest, arguments_should_be_substituted) {
	const int ARGC = 5;
	const char* const ARGS[] = { "test.exe", "123", "str", "3", "called" };
	Arguments args = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, args.i);
	EXPECT_EQ("str", args.s);
	EXPECT_EQ(3, args.fn);
	EXPECT_EQ("called", args.fs);
}

TEST_F(ArgumentsParsingTest, TooFewArguments_should_be_thrown_if_not_enough_arguments_are_given) {
	const int ARGC = 2;
	const char* const ARGS[] = { "test.exe", "123" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(ArgumentsParsingTest, TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const int ARGC = 6;
	const char* const ARGS[] =
		{ "test.exe", "123", "str", "3", "called", "extra" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(ArgumentsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_argument) {
	const int ARGC = 5;
	const char* const ARGS[] = { "test.exe", "num", "str", "3", "called" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}

TEST_F(ArgumentsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_function_argument) {
	const int ARGC = 5;
	const char* const ARGS[] = { "test.exe", "123", "str", "three", "called" };
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}
