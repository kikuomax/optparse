#ifndef _OPTPARSE_OPTPARSE_DEFAULT_FORMATTER_H
#define _OPTPARSE_OPTPARSE_DEFAULT_FORMATTER_H

#include "optparse/OptionParserException.h"

#include <cstdlib>
#include <cwchar>
#include <string>

namespace optparse {

	/**
	 * Default value formatter for `OptionParserBase`.
	 *
	 * Must be specialized.
	 *
	 * @tparam T
	 *     Type which represents a value.
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename T, typename Ch >
	class DefaultFormatter {
	public:
		/**
		 * Converts a given string into a value of the type `T`.
		 *
		 * This function must be specialized for `T` and `Ch`.
		 *
		 * @param valueStr
		 *     String to be converted into a `T` value.
		 * @throws OptionParserBase::BadValue
		 *     If `valueStr` is invalid.
		 */
		T operator ()(const std::basic_string< Ch >& valueStr) const;
	};

	/** `DefaultFormatter` which converts an `std::string` into `int`. */
	template <>
	class DefaultFormatter< int, char > {
	public:
		/**
		 * Converts a given string into an `int` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @throws OptionParserBase::BadValue
		 *     If `valueStr` is not an integer.
		 */
		int operator ()(const std::string& valueStr) const {
			char* end = 0;
			int x = strtol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			return x;
		}
	};

	/** `DefaultFormatter` from `std::string` to `std::string`. */
	template <>
	class DefaultFormatter< std::string, char > {
	public:
		/** Just returns a given string. */
		inline std::string operator ()(const std::string& valueStr) const {
			return valueStr;
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `int`. */
	template <>
	class DefaultFormatter< int, wchar_t > {
	public:
		/**
		 * Converts a given string into an `int` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @throws OptionParserBase::BadValue
		 *     If `valueStr` is not an integer.
		 */
		int operator ()(const std::wstring& valueStr) const {
			wchar_t* end = 0;
			int x = wcstol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			return x;
		}
	};

	/** `DefaultFormatter` from `std::wstring` to `std::wstring`. */
	template <>
	class DefaultFormatter< std::wstring, wchar_t > {
	public:
		/** Just returns a given string. */
		inline std::wstring operator ()(const std::wstring& valueStr) const {
			return valueStr;
		}
	};

}

#endif
