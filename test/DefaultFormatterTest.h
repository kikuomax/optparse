// This file provides tests for DefaultFormat regardless of character type.
// You need to provide the following definitions before including this header.
//  - Ch: character type
//  - String: string type of Ch
//  - STR(str): macro which makes a string literal
//  - PREFIX(name): macro which prefixes a given test name to avoid conflict
//

#include "optparse/DefaultFormatter.h"

#include "gtest/gtest.h"

TEST(PREFIX(DefaultFormatter_int_Test), integer_can_be_formatted) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_EQ(0, format(STR("0")));
	EXPECT_EQ(1, format(STR("1")));
	EXPECT_EQ(-1, format(STR("-1")));
	EXPECT_EQ(2147483647, format(STR("2147483647")));
	EXPECT_EQ(-2147483648, format(STR("-2147483648")));
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_non_number) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-x")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_real_number) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1e-2")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_double_Test), real_number_can_be_formatted) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_DOUBLE_EQ(0, format(STR("0")));
	EXPECT_DOUBLE_EQ(1, format(STR("1")));
	EXPECT_DOUBLE_EQ(-1, format(STR("-1")));
	EXPECT_DOUBLE_EQ(3.14, format(STR("3.14")));
	EXPECT_DOUBLE_EQ(-1.5e-3, format(STR("-1.5e-3")));
	EXPECT_DOUBLE_EQ(6.02e+23, format(STR("6.02e+23")));
	EXPECT_DOUBLE_EQ(0.5, format(STR(".5")));
	EXPECT_DOUBLE_EQ(-0.5, format(STR("-.5")));
}

TEST(PREFIX(DefaultFormatter_double_Test), BadValue_should_be_thrown_for_non_number) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_THROW(format(STR("real")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-x")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5e*3")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_double_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_string_Test), string_can_be_copied) {
	optparse::DefaultFormatter< String, Ch > format;
	EXPECT_EQ(STR("value"), format(STR("value")));
	EXPECT_EQ(STR(""), format(STR("")));
}

