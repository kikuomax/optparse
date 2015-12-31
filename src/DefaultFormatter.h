#ifndef _OPT_PARSE_DEFAULT_FORMATTER_H
#define _OPT_PARSE_DEFAULT_FORMATTER_H

#include "OptionParserException.h"

#include <cstdlib>
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
		/** String of the type `Ch`. */
		typedef std::basic_string< Ch > String;
	private:
		/**
		 * Returns a default name used to describe the value.
		 *
		 * @return
		 *     Default name used to describe the value.
		 */
		String getDefaultValueName() const;

		/**
		 * Converts a given string into a value of the type `T`.
		 *
		 * @param valueStr
		 *     String to be converted into a `T` value.
		 * @throws OptionParserBase::BadValue
		 *     If `valueStr` is invalid.
		 */
		T operator ()(const String& valueStr) const;
	};

	/** `DefaultFormatter` which converts an `std::string` into for `char`. */
	template <>
	class DefaultFormatter< int, char > {
	public:
		/** Returns "N". */
		inline std::string getDefaultValueName() const {
			return "N";
		}

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
		/** Returns "STR". */
		inline std::string getDefaultValueName() const {
			return "STR";
		}

		/** Just returns a given string. */
		inline std::string operator ()(const std::string& valueStr) const {
			return valueStr;
		}
	};

}

#endif
