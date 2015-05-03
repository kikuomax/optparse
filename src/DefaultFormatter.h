#ifndef _OPT_PARSE_DEFAULT_FORMATTER_H
#define _OPT_PARSE_DEFAULT_FORMATTER_H

#include <cstdlib>
#include <string>

namespace optparse {

	/**
	 * Default value formatter for `OptionParserBase`.
	 *
	 * Must be specialized.
	 *
	 * `T`: Type of the value.
	 *
	 * `Ch`: Type of an input character.
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
		String getDefaultValueName() const {
			return String();
		}

		/**
		 * Converts a given string into a value of the type `T`.
		 *
		 * `value` is undefined if conversion has failed.
		 *
		 * @param valueStr
		 *     String to be converted into a `T` value.
		 * @param[out] value
		 *     Converted value will be stored.
		 * @return
		 *     Whether conversion has succeeded.
		 */
		bool operator ()(const String& valueStr, T& value) const {
			return false;
		}
	};

	/** `DefaultFormatter` from `std::string` to `int`. */
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
		 * `value` won't be changed if conversion has failed.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @param[out] value
		 *     Converted `int` will be stored.
		 * @return
		 *     Whether conversion has succeeded.
		 *     `false` if `valueStr` is not a number.
		 */
		bool operator ()(const std::string& valueStr, int& value) const {
			char* end = 0;
			int x = strtol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				return false;
			}
			value = x;
			return true;
		}
	};

	/** `DefaultFormatter` from `std::string` to `std::string`. */
	template <>
	class DefaultFormatter< std::string, char > {
	public:
		/** Returns "str". */
		inline std::string getDefaultValueName() const {
			return "str";
		}

		/** Just copies a given string and always succeeds. */
		inline bool operator ()(const std::string& valueStr,
								std::string& value) const
		{
			value = valueStr;
			return true;
		}
	};

}

#endif
