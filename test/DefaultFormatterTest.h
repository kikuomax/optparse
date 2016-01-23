// This file provides tests for DefaultFormat regardless of character type.
// You need to provide the following definitions before including this header.
//  - Ch: character type
//  - String: string type of Ch
//  - STR(str): macro which makes a string literal
//  - PREFIX(name): macro which prefixes a given test name to avoid conflict
//

#include "optparse/DefaultFormatter.h"

#include <climits>
#include "gtest/gtest.h"

/** String which represents the maximum value representable by `int`. */
#define STR_INT_MAX  \
	(sizeof(int) == 8 ? STR("9223372036854775807") : STR("2147483647"))

/** String which represents the minimum value representable by `int`. */
#define STR_INT_MIN  \
	(sizeof(int) == 8 ? STR("-9223372036854775808") : STR("-2147483648"))

/**
 * String which represents a value greater than the maximum value representable
 * by `int`.
 */
#define STR_INT_MAX_MORE  \
	(sizeof(int) == 8 ? STR("9223372036854775808") : STR("2147483648"))

/**
 * String which represents a value less than the minimum value representable
 * by `int`.
 */
#define STR_INT_MIN_LESS  \
	(sizeof(int) == 8 ? STR("-9223372036854775809") : STR("-2147483649"))

/**
 * String which represents the maximum value representable by `unsigned int`.
 */
#define STR_UINT_MAX  \
	(sizeof(unsigned int) == 8 \
		? STR("18446744073709551615") \
		: STR("4294967295"))

/**
 * String which represents a value greater than the maximum value representable
 * by `unsigned int`.
 */
#define STR_UINT_MAX_MORE  \
	(sizeof(unsigned int) == 8 \
		? STR("18446744073709551616") \
		: STR("4294967296"))

/** String which represents the maximum value representable by `short`. */
#define STR_SHORT_MAX  \
	(sizeof(short) == 4 ? STR("2147483647") : STR("32767"))

/** String which represents the minimum value representable by `short`. */
#define STR_SHORT_MIN  \
	(sizeof(short) == 4 ? STR("-2147483648") : STR("-32768"))

/**
 * String which represents a value greater than the maximum value
 * representable by `short`.
 */
#define STR_SHORT_MAX_MORE  \
	(sizeof(short) == 4 ? STR("2147483648") : STR("32768"))

/**
 * String which represents a value less than the minimum value representable
 * by `short`.
 */
#define STR_SHORT_MIN_LESS  \
	(sizeof(short) == 4 ? STR("-2147483649") : STR("-32769"))

/**
 * String which represents the maximum value representable by `unsigned short`.
 */
#define STR_USHORT_MAX  \
	(sizeof(unsigned short) == 4 ? STR("4294967295") : STR("65535"))

/**
 * String which represents a value greater than the maximum value
 * representable by `unsigned short`.
 */
#define STR_USHORT_MAX_MORE  \
	(sizeof(unsigned short) == 4 ? STR("4294967296") : STR("65536"))

/** String which represents the maximum value representable by `long`. */
#define STR_LONG_MAX  \
	(sizeof(long) == 8 ? STR("9223372036854775807") : STR("2147483647"))

/** String which represents the minimum value representable by `long`. */
#define STR_LONG_MIN  \
	(sizeof(long) == 8 ? STR("-9223372036854775808") : STR("-2147483648"))

/**
 * String which represents a value greater than the maximum value representable
 * by `long`.
 */
#define STR_LONG_MAX_MORE  \
	(sizeof(long) == 8 ? STR("9223372036854775808") : STR("2147483648"))

/**
 * String which represents a value less than the minimum value representable
 * by `long`.
 */
#define STR_LONG_MIN_LESS  \
	(sizeof(long) == 8 ? STR("-9223372036854775809") : STR("-2147483649"))

/**
 * String which represents the maximum value representable by `unsigned long`.
 */
#define STR_ULONG_MAX  \
	(sizeof(unsigned long) == 8 \
		? STR("18446744073709551615") \
		: STR("4294967295"))

/**
 * String which represents a value greater than the maximum value representable
 * by `unsigned long`.
 */
#define STR_ULONG_MAX_MORE  \
	(sizeof(unsigned long) == 8 \
		? STR("18446744073709551616") \
		: STR("4294967296"))

/** String which represents the maximum value representable by `long long`. */
#define STR_LONG_LONG_MAX  \
	(sizeof(long long) == 8 ? STR("9223372036854775807") : STR("2147483647"))

/** String which represents the minimum value representable by `long long`. */
#define STR_LONG_LONG_MIN  \
	(sizeof(long long) == 8 ? STR("-9223372036854775808") : STR("-2147483648"))

/**
 * String which represents a value greater than the maximum value representable
 * by `long long`.
 */
#define STR_LONG_LONG_MAX_MORE  \
	(sizeof(long long) == 8 ? STR("9223372036854775808") : STR("2147483647"))

/**
 * String which represents a value less than the minimum value representable
 * by `long long`.
 */
#define STR_LONG_LONG_MIN_LESS  \
	(sizeof(long long) == 8 ? STR("-9223372036854775809") : STR("-2147483649"))

/**
 * String which represents the maximum value representable by
 * `unsigned long long`.
 */
#define STR_ULONG_LONG_MAX  \
	(sizeof(unsigned long long) == 8 \
		? STR("18446744073709551615") \
		: STR("4294967295"))

/**
 * String which represents a value greater than the maximum value representable
 * by `unsigned long long`.
 */
#define STR_ULONG_LONG_MAX_MORE  \
	(sizeof(unsigned long long) == 8 \
		? STR("18446744073709551616") \
		: STR("4294967296"))

TEST(PREFIX(DefaultFormatter_int_Test), int_can_be_formatted) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_EQ(0, format(STR("0")));
	EXPECT_EQ(1, format(STR("1")));
	EXPECT_EQ(-1, format(STR("-1")));
	EXPECT_EQ(INT_MAX, format(STR_INT_MAX));
	EXPECT_EQ(INT_MIN, format(STR_INT_MIN));
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_int_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< int, Ch > format;
	EXPECT_THROW(format(STR_INT_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR_INT_MIN_LESS), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_int_Test), unsigned_int_can_be_formatted) {
	optparse::DefaultFormatter< unsigned int, Ch > format;
	EXPECT_EQ(0, format(STR("0")));
	EXPECT_EQ(1, format(STR("1")));
	EXPECT_EQ(UINT_MAX, format(STR_UINT_MAX));
}

TEST(PREFIX(DefaultFormatter_unsigned_int_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< unsigned int, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_int_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< unsigned int, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_int_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< unsigned int, Ch > format;
	EXPECT_THROW(format(STR_UINT_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-1")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_short_Test), short_can_be_formatted) {
	optparse::DefaultFormatter< short, Ch > format;
	EXPECT_EQ(0, format(STR("0")));
	EXPECT_EQ(1, format(STR("1")));
	EXPECT_EQ(-1, format(STR("-1")));
	EXPECT_EQ(std::numeric_limits< short >::max(), format(STR_SHORT_MAX));
	EXPECT_EQ(std::numeric_limits< short >::min(), format(STR_SHORT_MIN));
}

TEST(PREFIX(DefaultFormatter_short_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< short, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_short_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< short, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_short_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< short, Ch > format;
	EXPECT_THROW(format(STR_SHORT_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR_SHORT_MIN_LESS), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_short_Test), unsigned_short_can_be_formatted) {
	optparse::DefaultFormatter< unsigned short, Ch > format;
	EXPECT_EQ(0, format(STR("0")));
	EXPECT_EQ(1, format(STR("1")));
	EXPECT_EQ(std::numeric_limits< unsigned short >::max(),
			  format(STR_USHORT_MAX));
}

TEST(PREFIX(DefaultFormatter_unsigned_short_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< unsigned short, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_short_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< unsigned short, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_short_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< unsigned short, Ch > format;
	EXPECT_THROW(format(STR_USHORT_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-1")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_Test), long_can_be_formatted) {
	optparse::DefaultFormatter< long, Ch > format;
	EXPECT_EQ(0L, format(STR("0")));
	EXPECT_EQ(1L, format(STR("1")));
	EXPECT_EQ(-1L, format(STR("-1")));
	EXPECT_EQ(LONG_MAX, format(STR_LONG_MAX));
	EXPECT_EQ(LONG_MIN, format(STR_LONG_MIN));
}

TEST(PREFIX(DefaultFormatter_long_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< long, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< long, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< long, Ch > format;
	EXPECT_THROW(format(STR_LONG_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR_LONG_MIN_LESS), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_Test), unsigned_long_can_be_formatted) {
	optparse::DefaultFormatter< unsigned long, Ch > format;
	EXPECT_EQ(0UL, format(STR("0")));
	EXPECT_EQ(1UL, format(STR("1")));
	EXPECT_EQ(ULONG_MAX, format(STR_ULONG_MAX));
}

TEST(PREFIX(DefaultFormatter_unsigned_long_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< unsigned long, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< unsigned long, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< unsigned long, Ch > format;
	EXPECT_THROW(format(STR_ULONG_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-1")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_long_Test), long_long_can_be_formatted) {
	optparse::DefaultFormatter< long long, Ch > format;
	EXPECT_EQ(0LL, format(STR("0")));
	EXPECT_EQ(1LL, format(STR("1")));
	EXPECT_EQ(-1LL, format(STR("-1")));
	EXPECT_EQ(std::numeric_limits< long long >::max(),
			  format(STR_LONG_LONG_MAX));
	EXPECT_EQ(std::numeric_limits< long long >::min(),
			  format(STR_LONG_LONG_MIN));
}

TEST(PREFIX(DefaultFormatter_long_long_Test), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< long long, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_long_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< long long, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_long_long_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< long long, Ch > format;
	EXPECT_THROW(format(STR_LONG_LONG_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR_LONG_LONG_MIN_LESS), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_long_TEST), unsigned_long_long_can_be_formatted) {
	optparse::DefaultFormatter< unsigned long long, Ch > format;
	EXPECT_EQ(0ULL, format(STR("0")));
	EXPECT_EQ(1ULL, format(STR("1")));
	EXPECT_EQ(std::numeric_limits< unsigned long long >::max(),
			  format(STR_ULONG_LONG_MAX));
}

TEST(PREFIX(DefaultFormatter_unsigned_long_long_TEST), BadValue_should_be_thrown_for_non_integer) {
	optparse::DefaultFormatter< unsigned long long, Ch > format;
	EXPECT_THROW(format(STR("num")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_long_TEST), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< unsigned long long, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_unsigned_long_long_TEST), BadValue_shoudl_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< unsigned long long, Ch > format;
	EXPECT_THROW(format(STR_ULONG_LONG_MAX_MORE), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-1")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_double_Test), double_can_be_formatted) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_DOUBLE_EQ(0, format(STR("0")));
	EXPECT_DOUBLE_EQ(1, format(STR("1")));
	EXPECT_DOUBLE_EQ(-1, format(STR("-1")));
	EXPECT_DOUBLE_EQ(3.14, format(STR("3.14")));
	EXPECT_DOUBLE_EQ(-1.5e-3, format(STR("-1.5e-3")));
	EXPECT_DOUBLE_EQ(1.0e+308, format(STR("1.0e+308")));
	EXPECT_DOUBLE_EQ(-1.0e+308, format(STR("-1.0e+308")));
	EXPECT_DOUBLE_EQ(.5, format(STR(".5")));
	EXPECT_DOUBLE_EQ(-.5, format(STR("-.5")));
}

TEST(PREFIX(DefaultFormatter_double_Test), BadValue_should_be_thrown_for_non_number) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_THROW(format(STR("real")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5e")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_double_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_double_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< double, Ch > format;
	EXPECT_THROW(format(STR("1.0e+309")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_float_Test), float_can_be_formatted) {
	optparse::DefaultFormatter< float, Ch > format;
	EXPECT_FLOAT_EQ(0.0f, format(STR("0")));
	EXPECT_FLOAT_EQ(1.0f, format(STR("1")));
	EXPECT_FLOAT_EQ(-1.0f, format(STR("-1")));
	EXPECT_FLOAT_EQ(3.14f, format(STR("3.14")));
	EXPECT_FLOAT_EQ(-1.5e-3f, format(STR("-1.5e-3")));
	EXPECT_FLOAT_EQ(6.02e+23f, format(STR("6.02e+23")));
	EXPECT_FLOAT_EQ(1.0e+38f, format(STR("1.0e+38")));
	EXPECT_FLOAT_EQ(-1.0e+38f, format(STR("-1.0e+38")));
	EXPECT_FLOAT_EQ(.5f, format(STR(".5")));
	EXPECT_FLOAT_EQ(-.5f, format(STR("-.5")));
}

TEST(PREFIX(DefaultFormatter_float_Test), BadValue_should_be_thrown_for_non_number) {
	optparse::DefaultFormatter< float, Ch > format;
	EXPECT_THROW(format(STR("real")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("1.5e")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_float_Test), BadValue_should_be_thrown_for_empty_string) {
	optparse::DefaultFormatter< float, Ch > format;
	EXPECT_THROW(format(STR("")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_float_Test), BadValue_should_be_thrown_for_out_of_range_value) {
	optparse::DefaultFormatter< float, Ch > format;
	EXPECT_THROW(format(STR("1.0e+39")), optparse::BadValue< Ch >);
	EXPECT_THROW(format(STR("-1.0e+39")), optparse::BadValue< Ch >);
}

TEST(PREFIX(DefaultFormatter_string_Test), string_can_be_copied) {
	optparse::DefaultFormatter< String, Ch > format;
	EXPECT_EQ(STR("value"), format(STR("value")));
	EXPECT_EQ(STR(""), format(STR("")));
}

