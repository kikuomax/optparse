#ifndef _OPTPARSE_TEST_GTEST_HELPER_H
#define _OPTPARSE_TEST_GTEST_HELPER_H

/**
 * Expands into `INSTANTIATE_TYPED_TEST_CASE_P` indirectly.
 *
 * Equivalent to the following,
 *
 *     INSTANTIATE_TYPED_TEST_CASE_P(Prefix, CaseName, Types)
 *
 * This macro is necessary when arguments contain a macro which expands into
 * a token concatenation (`##`).
 */
#define INSTANTIATE_TYPED_TEST_CASE_P_INDIRECTLY(Prefix, CaseName, Types) \
	INSTANTIATE_TYPED_TEST_CASE_P(Prefix, CaseName, Types)

#endif
