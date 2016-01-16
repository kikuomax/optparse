// This file provides tests for OptionParserBase regardless of character type.
// You need to define the followings before including this header,
//  - Ch: character type
//  - String: string type of Ch. must be compatible with std::basic_string
//  - STR(str): macro to create a character and string literal
//  - PREFIX(name): macro which prefixes a test case name to avoid conflict
//

#include "optparse/DefaultFormatter.h"
#include "optparse/OptionParserBase.h"

#include "gtest/gtest.h"

TEST(PREFIX(OptionParserBaseTest), description_can_be_set) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_EQ(STR("test program"), parser.getDescription());
}

TEST(PREFIX(OptionParserBaseTest), program_name_should_be_empty_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_EQ(STR(""), parser.getProgramName());
}

TEST(PREFIX(OptionParserBaseTest), parser_should_have_no_option_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_EQ(0, parser.getOptionCount());
}

TEST(PREFIX(OptionParserBaseTest), parser_should_have_no_argument_by_default) {
	struct Dummy {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_EQ(0, parser.getArgumentCount());
}

TEST(PREFIX(OptionParserBaseTest), string_starting_with_dash_followed_by_alphabet_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel(STR("-o")));
	EXPECT_TRUE(Parser::isLabel(STR("--option")));
}

TEST(PREFIX(OptionParserBaseTest), dash_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel(STR("-")));
	EXPECT_TRUE(Parser::isLabel(STR("--")));
}

TEST(PREFIX(OptionParserBaseTest), empty_string_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel(STR("")));
}

TEST(PREFIX(OptionParserBaseTest), string_starting_with_other_than_dash_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel(STR("o")));
	EXPECT_FALSE(Parser::isLabel(STR("option")));
}

TEST(PREFIX(OptionParserBaseTest), string_starting_with_dash_followed_by_digit_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel(STR("-0")));
	EXPECT_FALSE(Parser::isLabel(STR("-1")));
	EXPECT_FALSE(Parser::isLabel(STR("-2")));
	EXPECT_FALSE(Parser::isLabel(STR("-3")));
	EXPECT_FALSE(Parser::isLabel(STR("-4")));
	EXPECT_FALSE(Parser::isLabel(STR("-5")));
	EXPECT_FALSE(Parser::isLabel(STR("-6")));
	EXPECT_FALSE(Parser::isLabel(STR("-7")));
	EXPECT_FALSE(Parser::isLabel(STR("-8")));
	EXPECT_FALSE(Parser::isLabel(STR("-9")));
}

TEST(PREFIX(OptionParserBaseTest), string_starting_with_dash_followed_by_dot_cannot_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_FALSE(Parser::isLabel(STR("-.")));
	EXPECT_FALSE(Parser::isLabel(STR("-.1")));
}

TEST(PREFIX(OptionParserBaseTest), string_starting_with_dash_followed_by_another_dash_and_digit_can_be_option_label) {
	struct Dummy {};
	typedef optparse::OptionParserBase<
		Dummy, Ch, optparse::DefaultFormatter > Parser;
	EXPECT_TRUE(Parser::isLabel(STR("--0")));
	EXPECT_TRUE(Parser::isLabel(STR("--1")));
	EXPECT_TRUE(Parser::isLabel(STR("--2")));
	EXPECT_TRUE(Parser::isLabel(STR("--3")));
	EXPECT_TRUE(Parser::isLabel(STR("--4")));
	EXPECT_TRUE(Parser::isLabel(STR("--5")));
	EXPECT_TRUE(Parser::isLabel(STR("--6")));
	EXPECT_TRUE(Parser::isLabel(STR("--7")));
	EXPECT_TRUE(Parser::isLabel(STR("--8")));
	EXPECT_TRUE(Parser::isLabel(STR("--9")));
}

TEST(PREFIX(OptionParserBaseTest), int_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("--int"), STR("NUM"), STR("test int field"), &Dummy::field);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("--int"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test int field"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("NUM"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), string_field_option_can_be_added) {
	struct Dummy {
		String field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("-s"), STR("name"), STR("test string field"), &Dummy::field);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-s"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test string field"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("name"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_int_field_option_is_invalid) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(
		parser.addOption(
			STR("o"), STR("INT"), STR("test int field"), &Dummy::field),
		optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), custom_format_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	int (*format)(const String&) = [](const String& value) {
		return static_cast< int >(value.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("--custom"), STR("X"), STR("custom int field"),
		&Dummy::field, format);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("--custom"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("custom int field"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("X"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_custom_format_field_option_is_invalid) {
	struct Dummy {
		int field;
	};
	int (*format)(const String&) = [](const String& value) {
		return static_cast< int >(value.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(
		parser.addOption(STR("-3"), STR("X"), STR("custom int field"),
						 &Dummy::field, format),
		optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), const_int_field_option_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(STR("-o"), STR("test const int field"), &Dummy::field, 1);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-o"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test const int field"),
			  parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR(""), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), const_string_field_option_can_be_added) {
	struct Dummy {
		String field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("-o"), STR("test const string field"),
		&Dummy::field, String(STR("s")));
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-o"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test const string field"),
			  parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR(""), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_const_int_field_is_invalid) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(
		parser.addOption(
			STR(""), STR("test const int field"), &Dummy::field, 123),
		optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), int_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(STR("-f"), STR("count"), STR("test int function"), func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-f"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test int function"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("count"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), string_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const String&) = [](Dummy&, const String&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("--fun"), STR("SYMBOL"), STR("test string function"), func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("--fun"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test string function"),
			  parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("SYMBOL"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_int_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(
		parser.addOption(
			STR("-.9"), STR("XYZ"), STR("test int function"), func),
		optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), custom_format_function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const String&) = [](const String& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("--xfun"), STR("X"), STR("custom int function"), func, format);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("--xfun"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("custom int function"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("X"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_custom_format_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const String&) = [](const String& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(
		parser.addOption(
			STR("-9fun"), STR("X"), STR("custom int function"), func, format),
		optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), function_option_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&) = [](Dummy&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(STR("-s"), STR("test function"), func);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-s"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test function"), parser.getOption(0).getDescription());
	ASSERT_FALSE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR(""), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), ConfigException_should_be_thrown_if_label_of_function_option_is_invalid) {
	struct Dummy {};
	void (*func)(Dummy&) = [](Dummy&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	ASSERT_THROW(parser.addOption(STR("-.-"), STR("test function"), func),
				 optparse::ConfigException);
}

TEST(PREFIX(OptionParserBaseTest), int_field_option_can_be_replaced_with_string_field_option) {
	struct Dummy {
		int intField;
		String stringField;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.addOption(
		STR("-f"), STR("N"), STR("test int field"), &Dummy::intField);
	parser.addOption(
		STR("-f"), STR("STR"), STR("test string field"), &Dummy::stringField);
	ASSERT_EQ(1, parser.getOptionCount());
	EXPECT_EQ(STR("-f"), parser.getOption(0).getLabel());
	EXPECT_EQ(STR("test string field"), parser.getOption(0).getDescription());
	ASSERT_TRUE(parser.getOption(0).needsValue());
	EXPECT_EQ(STR("STR"), parser.getOption(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), int_field_argument_can_be_added) {
	struct Dummy {
		int field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(STR("INT"), STR("int argument"), &Dummy::field);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("int argument"), parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("INT"), parser.getArgument(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), string_field_argument_can_be_added) {
	struct Dummy {
		String field;
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(STR("STRING"), STR("string argument"), &Dummy::field);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("string argument"), parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("STRING"), parser.getArgument(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), custom_format_field_argument_can_be_added) {
	struct Dummy {
		int field;
	};
	int (*format)(const String&) = [](const String& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(
		STR("X"), STR("custom int argument"), &Dummy::field, format);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("custom int argument"),
			  parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("X"), parser.getArgument(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), int_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(STR("IFUN"), STR("int function argument"), func);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("int function argument"),
			  parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("IFUN"), parser.getArgument(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), string_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const String&) = [](Dummy&, const String&) {};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(STR("SFUN"), STR("string function argument"), func);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("string function argument"),
			  parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("SFUN"), parser.getArgument(0).getValueName());
}

TEST(PREFIX(OptionParserBaseTest), custom_format_function_argument_can_be_added) {
	struct Dummy {};
	void (*func)(Dummy&, const int&) = [](Dummy&, const int&) {};
	int (*format)(const String&) = [](const String& str) {
		return static_cast< int >(str.size());
	};
	optparse::OptionParserBase< Dummy, Ch, optparse::DefaultFormatter >
		parser(STR("test program"));
	parser.appendArgument(
		STR("XFUN"), STR("custom function argument"), func, format);
	ASSERT_EQ(1, parser.getArgumentCount());
	EXPECT_EQ(STR("custom function argument"),
			  parser.getArgument(0).getDescription());
	EXPECT_EQ(STR("XFUN"), parser.getArgument(0).getValueName());
}

/** Fixture for the tests that parse options. */
class PREFIX(OptionsParsingTest) : public ::testing::Test {
protected:
	/** Options to be substituted. */
	struct Options {
		/** Field associated with "-i". 0 by default. */
		int i;

		/** Field associated with "-s". Empty by default. */
		String s;

		/** Field associated with "--custom". 0 by default. */
		int custom;

		/** Field associated with "-C". 0 by default. */
		int C;

		/** Field associated with "-S". Empty by default. */
		String S;

		/** Field associated with "--fn". 0 by default. */
		int fn;

		/** Field associated with "--fs". Empty by default. */
		String fs;

		/** Field associated with "--customf". 0 by default. */
		int customf;

		/** Field associated with "--flag". false by default. */
		bool flag;

		/** Initializes with default values. */
		Options() : i(0), C(0), fn(0), flag(false) {}

		/** Formats the custom field value. */
		static int formatCustom(const String& value) {
			return static_cast< int >(value.size());
		}

		/** Sets the `fn` field to a given value. */
		static void setFn(Options& options, const int& x) {
			options.fn = x;
		}

		/** Sets the `fs` field to a given value. */
		static void setFs(Options& options, const String& str) {
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
		Options, Ch, optparse::DefaultFormatter >* pParser;

	/** Creates the shared option parser. */
	virtual void SetUp() {
		this->pParser = new optparse::OptionParserBase<
			Options, Ch, optparse::DefaultFormatter >(STR("test program"));
		// configures optional fields
		this->pParser->addOption(
			STR("-i"), STR("N"), STR("int option"), &Options::i);
		this->pParser->addOption(
			STR("-s"), STR("STR"), STR("string option"), &Options::s);
		this->pParser->addOption(
			STR("--custom"), STR("X"), STR("custom int option"),
			&Options::custom, &Options::formatCustom);
		this->pParser->addOption(
			STR("-C"), STR("const int option"), &Options::C, 123);
		this->pParser->addOption(
			STR("-S"), STR("const string option"),
			&Options::S, String(STR("constant")));
		this->pParser->addOption(
			STR("--fn"), STR("INT"), STR("int function option"),
			&Options::setFn);
		this->pParser->addOption(
			STR("--fs"), STR("STR"), STR("string function option"),
			&Options::setFs);
		this->pParser->addOption(
			STR("--customf"), STR("X"), STR("custom int function option"),
			&Options::setCustomf, &Options::formatCustom);
		this->pParser->addOption(
			STR("--flag"), STR("function option"), &Options::setFlag);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(PREFIX(OptionsParsingTest), parse_should_remember_program_name) {
	const Ch* const ARGS[] = { STR("test.exe") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(STR("test.exe"), this->pParser->getProgramName());
}

TEST_F(PREFIX(OptionsParsingTest), int_field_option_should_substitute_int_field_with_given_value) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-i"), STR("4649") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(4649, options.i);
}

TEST_F(PREFIX(OptionsParsingTest), string_field_option_should_substitute_string_field_with_given_value) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-s"), STR("test") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(STR("test"), options.s);
}

TEST_F(PREFIX(OptionsParsingTest), custom_format_field_option_should_substitute_field_with_value_formatted_by_custom_format) {
	const Ch* const ARGS[] =
		{ STR("test.exe"), STR("--custom"), STR("value") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(5, options.custom);
}

TEST_F(PREFIX(OptionsParsingTest), const_int_field_option_should_substitute_int_field_with_constant_if_specified) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-C") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, options.C);
}

TEST_F(PREFIX(OptionsParsingTest), const_string_field_option_should_substitute_string_field_with_constant_if_specified) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-S") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(STR("constant"), options.S);
}

TEST_F(PREFIX(OptionsParsingTest), int_function_should_be_called_if_specified) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("--fn"), STR("99") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(99, options.fn);
}

TEST_F(PREFIX(OptionsParsingTest), string_function_should_be_called_if_specified) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("--fs"), STR("function") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(STR("function"), options.fs);
}

TEST_F(PREFIX(OptionsParsingTest), custom_format_function_option_should_be_called_if_specified) {
	const Ch* const ARGS[] =
		{ STR("test.exe"), STR("--customf"), STR("custom function") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(15, options.customf);
}

TEST_F(PREFIX(OptionsParsingTest), flag_function_should_be_called_if_specified) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("--flag") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Options options = this->pParser->parse(ARGC, ARGS);
	EXPECT_TRUE(options.flag);
}

TEST_F(PREFIX(OptionsParsingTest), TooFewArguments_should_be_thrown_if_no_arguments_are_given) {
	const Ch* const ARGS[] = { STR("ignored") };
	const int ARGC = 0;
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(PREFIX(OptionsParsingTest), TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("arg") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(PREFIX(OptionsParsingTest), ValueNeeded_should_be_thrown_if_no_value_is_given_to_option_which_needs_value) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-i") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS),
				 optparse::ValueNeeded< Ch >);
}

TEST_F(PREFIX(OptionsParsingTest), BadValue_should_be_thrown_if_non_number_is_given_to_int_field_option) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("-i"), STR("AHO") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< Ch >);
}

TEST_F(PREFIX(OptionsParsingTest), UnknownOption_should_be_thrown_if_unknown_option_is_given) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("--unknown") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS),
				 optparse::UnknownOption< Ch >);
}

/** Fixture for the tests that parse arguments. */
class PREFIX(ArgumentsParsingTest) : public ::testing::Test {
protected:
	/** Arguments to be substituted. */
	struct Arguments {
		/** Integer argument. 0 initially. */
		int i;

		/** String argument. Empty initially. */
		String s;

		/** Custom int argument. 0 initially. */
		int custom;

		/** Set by `setFn`. 0 initially. */
		int fn;

		/** Set by `setFs`. Empty initially. */
		String fs;

		/** Set by `setCustomf`. 0 initially. */
		int customf;

		/** Initializes arguments. */
		Arguments() : i(0), custom(0), fn(0) {}

		/** Custom formatter function. */
		static int format(const String& str) {
			return static_cast< int >(str.size());
		}

		/** Sets the `fn` field to a given value. */
		static void setFn(Arguments& args, const int& x) {
			args.fn = x;
		}

		/** Sets the `fs` field to a given string. */
		static void setFs(Arguments& args, const String& str) {
			args.fs = str;
		}

		/** Sets the `customf` field to a given value. */
		static void setCustomf(Arguments& args, const int& x) {
			args.customf = x;
		}
	};

	/** Shared option parser. */
	optparse::OptionParserBase<
		Arguments, Ch, optparse::DefaultFormatter >* pParser;

	/** Creates the shared option parser. */
	virtual void SetUp() {
		this->pParser = new optparse::OptionParserBase<
			Arguments, Ch, optparse::DefaultFormatter >(STR("test program"));
		// configures arguments
		this->pParser->appendArgument(
			STR("i"), STR("int argument"), &Arguments::i);
		this->pParser->appendArgument(
			STR("s"), STR("string argument"), &Arguments::s);
		this->pParser->appendArgument(
			STR("custom"), STR("custom field argument"),
			&Arguments::custom, &Arguments::format);
		this->pParser->appendArgument(
			STR("fn"), STR("int function argument"), &Arguments::setFn);
		this->pParser->appendArgument(
			STR("fs"), STR("string function argument"), &Arguments::setFs);
		this->pParser->appendArgument(
			STR("customf"), STR("custom function argument"),
			&Arguments::setCustomf, &Arguments::format);
	}

	/** Releases the shared option parser. */
	virtual void TearDown() {
		delete this->pParser;
	}
};

TEST_F(PREFIX(ArgumentsParsingTest), arguments_should_be_substituted) {
	const Ch* const ARGS[] = {
		STR("test.exe"),
		STR("123"),
		STR("str"),
		STR("custom"),
		STR("-3"),
		STR("called"),
		STR("custom function")
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	Arguments args = this->pParser->parse(ARGC, ARGS);
	EXPECT_EQ(123, args.i);
	EXPECT_EQ(STR("str"), args.s);
	EXPECT_EQ(6, args.custom);
	EXPECT_EQ(-3, args.fn);
	EXPECT_EQ(STR("called"), args.fs);
	EXPECT_EQ(15, args.customf);
}

TEST_F(PREFIX(ArgumentsParsingTest), TooFewArguments_should_be_thrown_if_not_enough_arguments_are_given) {
	const Ch* const ARGS[] = { STR("test.exe"), STR("123") };
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooFewArguments);
}

TEST_F(PREFIX(ArgumentsParsingTest), TooManyArguments_should_be_thrown_if_unnecessary_argument_is_given) {
	const Ch* const ARGS[] = {
		STR("test.exe"),
		STR("123"),
		STR("str"),
		STR("custom"),
		STR("-3"),
		STR("called"),
		STR("custom function"),
		STR("extra")
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::TooManyArguments);
}

TEST_F(PREFIX(ArgumentsParsingTest), BadValue_should_be_thrown_if_non_number_is_given_to_int_argument) {
	const Ch* const ARGS[] = {
		STR("test.exe"),
		STR("num"),
		STR("str"),
		STR("custom"),
		STR("-3"),
		STR("called"),
		STR("custom function")
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< Ch >);
}

TEST_F(PREFIX(ArgumentsParsingTest), BadValue_should_be_thrown_if_non_number_is_given_to_int_function_argument) {
	const Ch* const ARGS[] = {
		STR("test.exe"),
		STR("123"),
		STR("str"),
		STR("custom"),
		STR("three"),
		STR("called"),
		STR("custom function")
	};
	const int ARGC = sizeof(ARGS) / sizeof(ARGS[0]);
	ASSERT_THROW(this->pParser->parse(ARGC, ARGS), optparse::BadValue< Ch >);
}
