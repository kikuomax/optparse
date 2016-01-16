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

TEST(OptionParserBaseTest, parser_should_have_no_option_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_EQ(0, parser.getOptionCount());
}

TEST(OptionParserBaseTest, parser_should_have_no_argument_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_EQ(0, parser.getArgumentCount());
}

TEST(OptionParserBaseTest, string_starting_with_dash_followed_by_alphabet_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel("-o"));
	EXPECT_TRUE(Parser::isLabel("--option"));
}

TEST(OptionParserBaseTest, dash_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel("-"));
	EXPECT_TRUE(Parser::isLabel("--"));
}

TEST(OptionParserBaseTest, empty_string_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel(""));
}

TEST(OptionParserBaseTest, string_starting_with_other_than_dash_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel("o"));
	EXPECT_FALSE(Parser::isLabel("option"));
}

TEST(OptionParserBaseTest, string_starting_with_dash_followed_by_digit_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel("-0"));
	EXPECT_FALSE(Parser::isLabel("-1"));
	EXPECT_FALSE(Parser::isLabel("-2"));
	EXPECT_FALSE(Parser::isLabel("-3"));
	EXPECT_FALSE(Parser::isLabel("-4"));
	EXPECT_FALSE(Parser::isLabel("-5"));
	EXPECT_FALSE(Parser::isLabel("-6"));
	EXPECT_FALSE(Parser::isLabel("-7"));
	EXPECT_FALSE(Parser::isLabel("-8"));
	EXPECT_FALSE(Parser::isLabel("-9"));
}

TEST(OptionParserBaseTest, string_starting_with_dash_followed_by_dot_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel("-."));
	EXPECT_FALSE(Parser::isLabel("-.1"));
}

TEST(OptionParserBaseTest, string_starting_with_dash_followed_by_another_dash_and_digit_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, char, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel("--0"));
	EXPECT_TRUE(Parser::isLabel("--1"));
	EXPECT_TRUE(Parser::isLabel("--2"));
	EXPECT_TRUE(Parser::isLabel("--3"));
	EXPECT_TRUE(Parser::isLabel("--4"));
	EXPECT_TRUE(Parser::isLabel("--5"));
	EXPECT_TRUE(Parser::isLabel("--6"));
	EXPECT_TRUE(Parser::isLabel("--7"));
	EXPECT_TRUE(Parser::isLabel("--8"));
	EXPECT_TRUE(Parser::isLabel("--9"));
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

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_int_field_option_is_invalid) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("o", "INT", "test int field", &Dummy::field),
		optparse::ConfigException);
}

TEST(OptionParserBaseTest, custom_format_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	int (*format)(const std::string&) = [](const std::string& value) {
		return static_cast< int >(value.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption(
		"--custom", "X", "custom int field", &Dummy::field, format);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("--custom", parser.getOption(0).getLabel());
	EXPECT_EQ("custom int field", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("X", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_custom_format_field_option_is_invalid) {
	struct Dummy {
		int field;
	};
	int (*format)(const std::string&) = [](const std::string& value) {
		return static_cast< int >(value.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("-3", "X", "custom int field", &Dummy::field, format),
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

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_const_int_field_is_invalid) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("", "test const int field", &Dummy::field, 123),
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

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_int_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(parser.addOption("-.9", "XYZ", "test int function", func),
				 optparse::ConfigException);
}

TEST(OptionParserBaseTest, custom_format_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const std::string&) = [](const std::string& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.addOption("--xfun", "X", "custom int function", func, format);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ("--xfun", parser.getOption(0).getLabel());
	EXPECT_EQ("custom int function", parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ("X", parser.getOption(0).getValueName());
}

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_custom_format_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const std::string&) = [](const std::string& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(
		parser.addOption("-9fun", "X", "custom int function", func, format),
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

TEST(OptionParserBaseTest, ConfigException_should_be_thrown_if_label_of_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&) = [](Dummy&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	ASSERT_THROW(parser.addOption("-.-", "test function", func),
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

TEST(OptionParserBaseTest, custom_format_field_argument_can_be_added) {
	struct Dummy {
		int field;
	};
	int (*format)(const std::string&) = [](const std::string& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("X", "custom int argument", &Dummy::field, format);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("custom int argument", parser.getArgument(0).getDescription());
	EXPECT_EQ("X", parser.getArgument(0).getValueName());
}

TEST(OptionParserBaseTest, int_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("IFUN", "int function argument", func);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("int function argument", parser.getArgument(0).getDescription());
	EXPECT_EQ("IFUN", parser.getArgument(0).getValueName());
}

TEST(OptionParserBaseTest, string_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const std::string&) =
		[](Dummy&, const std::string&) {};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("SFUN", "string function argument", func);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("string function argument",
			  parser.getArgument(0).getDescription());
	EXPECT_EQ("SFUN", parser.getArgument(0).getValueName());
}

TEST(OptionParserBaseTest, custom_format_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const std::string&) = [](const std::string& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, char, optparse::DefaultFormatter >
		parser("test program");
	parser.appendArgument("XFUN", "custom function argument", func, format);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ("custom function argument",
			  parser.getArgument(0).getDescription());
	EXPECT_EQ("XFUN", parser.getArgument(0).getValueName());
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

		/** Field associated with "--custom". 0 by default. */
		int custom;

		/** Field associated with "-C". 0 by default. */
		int C;

		/** Field associated with "-S". Empty by default. */
		std::string S;

		/** Field associated with "--fn". 0 by default. */
		int fn;

		/** Field associated with "--fs". Empty by default. */
		std::string fs;

		/** Field associated with "--customf". 0 by default. */
		int customf;

		/** Field associated with "--flag". false by default. */
		bool flag;

		/** Initializes with default values. */
		Options() : i(0), C(0), fn(0), flag(false) {}

		/** Formats the custom field value. */
		static int formatCustom(const std::string& value) {
			return static_cast< int >(value.size());
		}

		/** Sets the `fn` field to a given value. */
		static void setFn(Options& options, const int& x) {
			options.fn = x;
		}

		/** Sets the `fs` field to a given value. */
		static void setFs(Options& options, const std::string& str) {
			options.fs = str;
		}

		/** Sets the `customf` field to a given value. */
		static void setCustomf(Options& options, const int& x) {
			options.customf = x;
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
			"--custom", "X", "custom int option",
			&Options::custom, &Options::formatCustom);
		this->pParser->addOption(
			"-C", "const int option", &Options::C, 123);
		this->pParser->addOption(
			"-S", "const string option", &Options::S, std::string("constant"));
		this->pParser->addOption(
			"--fn", "INT", "int function option", &Options::setFn);
		this->pParser->addOption(
			"--fs", "STR", "string function option", &Options::setFs);
		this->pParser->addOption(
			"--customf", "X", "custom int function option",
			&Options::setCustomf, &Options::formatCustom);
		this->pParser->addOption(
			"--flag", "function option", &Options::setFlag);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(OptionsParsingTest, parse_should_remember_program_name) {
	const char* const ARGS[] = { "test.exe" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("test.exe", this->pParser->getProgramName());
}

TEST_F(OptionsParsingTest, int_field_option_should_substitute_int_field_with_given_value) {
	const char* const ARGS[] = { "test.exe", "-i", "4649" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(4649, options.i);
}

TEST_F(OptionsParsingTest, string_field_option_should_substitute_string_field_with_given_value) {
	const char* const ARGS[] = { "test.exe", "-s", "test" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("test", options.s);
}

TEST_F(OptionsParsingTest, custom_format_field_option_should_substitute_field_with_value_formatted_by_custom_format) {
	const char* const ARGS[] = { "test.exe", "--custom", "value" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(5, options.custom);
}

TEST_F(OptionsParsingTest, const_int_field_option_should_substitute_int_field_with_constant_if_specified) {
	const char* const ARGS[] = { "test.exe", "-C" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, options.C);
}

TEST_F(OptionsParsingTest, const_string_field_option_should_substitute_string_field_with_constant_if_specified) {
	const char* const ARGS[] = { "test.exe", "-S" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("constant", options.S);
}

TEST_F(OptionsParsingTest, int_function_should_be_called_if_specified) {
	const char* const ARGS[] = { "test.exe", "--fn", "99" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(99, options.fn);
}

TEST_F(OptionsParsingTest, string_function_should_be_called_if_specified) {
	const char* const ARGS[] = { "test.exe", "--fs", "function" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ("function", options.fs);
}

TEST_F(OptionsParsingTest, custom_format_function_option_should_be_called_if_specified) {
	const char* const ARGS[] = { "test.exe", "--customf", "custom function" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(15, options.customf);
}

TEST_F(OptionsParsingTest, flag_function_should_be_called_if_specified) {
	const char* const ARGS[] = { "test.exe", "--flag" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_TRUE(options.flag);
}

TEST_F(OptionsParsingTest, TooFewArguments_should_be_thrown_if_no_arguments_are_given) {
	const char* const ARGS[] = { "ignored" };
	const int ARGC = 0;
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(OptionsParsingTest, TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const char* const ARGS[] = { "test.exe", "arg" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(OptionsParsingTest, ValueNeeded_should_be_thrown_if_no_value_is_given_to_option_which_needs_value) {
	const char* const ARGS[] = { "test.exe", "-i" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS),
				 optparse::ValueNeeded< char >);
}

TEST_F(OptionsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_field_option) {
	const char* const ARGS[] = { "test.exe", "-i", "AHO" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}

TEST_F(OptionsParsingTest, UnknownOption_should_be_thrown_if_unknown_option_is_given) {
	const char* const ARGS[] = { "test.exe", "--unknown" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
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

		/** Custom int argument. 0 initially. */
		int custom;

		/** Set by `setFn`. 0 initially. */
		int fn;

		/** Set by `setFs`. Empty initially. */
		std::string fs;

		/** Set by `setCustomf`. 0 initially. */
		int customf;

		/** Initializes arguments. */
		Arguments() : i(0), custom(0), fn(0) {}

		/** Custom formatter function. */
		static int format(const std::string& str) {
			return static_cast< int >(str.size());
		}

		/** Sets the `fn` field to a given value. */
		static void setFn(Arguments& args, const int& x) {
			args.fn = x;
		}

		/** Sets the `fs` field to a given string. */
		static void setFs(Arguments& args, const std::string& str) {
			args.fs = str;
		}

		/** Sets the `customf` field to a given value. */
		static void setCustomf(Arguments& args, const int& x) {
			args.customf = x;
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
			"custom", "custom field argument", &Arguments::custom,
			&Arguments::format);
		this->pParser->appendArgument(
			"fn", "int function argument", &Arguments::setFn);
		this->pParser->appendArgument(
			"fs", "string function argument", &Arguments::setFs);
		this->pParser->appendArgument(
			"customf", "custom function argument", &Arguments::setCustomf,
			&Arguments::format);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(ArgumentsParsingTest, arguments_should_be_substituted) {
	const char* const ARGS[] = {
		"test.exe", "123", "str", "custom", "-3", "called", "custom function"
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Arguments args = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, args.i);
	EXPECT_EQ("str", args.s);
	EXPECT_EQ(6, args.custom);
	EXPECT_EQ(-3, args.fn);
	EXPECT_EQ("called", args.fs);
	EXPECT_EQ(15, args.customf);
}

TEST_F(ArgumentsParsingTest, TooFewArguments_should_be_thrown_if_not_enough_arguments_are_given) {
	const char* const ARGS[] = { "test.exe", "123" };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(ArgumentsParsingTest, TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const char* const ARGS[] = {
		"test.exe", "123", "str", "custom", "-3", "called", "custom function",
		"extra"
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(ArgumentsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_argument) {
	const char* const ARGS[] = {
		"test.exe", "num", "str", "custom", "-3", "called", "custom function"
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}

TEST_F(ArgumentsParsingTest, BadValue_should_be_thrown_if_non_number_is_given_to_int_function_argument) {
	const char* const ARGS[] = {
		"test.exe", "123", "str", "custom", "three", "called", "custom function"
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< char >);
}
