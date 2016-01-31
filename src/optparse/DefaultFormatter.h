#ifndef _OPTPARSE_OPTPARSE_DEFAULT_FORMATTER_H
#define _OPTPARSE_OPTPARSE_DEFAULT_FORMATTER_H

#include "optparse/OptionParserException.h"

#include <cerrno>
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
		 * @return
		 *     `T` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is invalid.
		 */
		T operator ()(const std::basic_string< Ch >& valueStr) const;
	};

	/** Helper utilities for `DefaultFormatter`. */
	class DefaultFormatterHelper {
	public:
		/**
		 * Converts a given `char` string into a `long long` value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::strtoll(str, end, 10)
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numberical value.
		 * @return
		 *     `long long` value equivalent to `str`.
		 */
		static inline long long strtoll(const char* str, char** end) {
#if defined(_MSC_VER) && _MSC_VER < 1800
			return static_cast< long long >(_strtoi64(str, end, 10));
#else
			return ::strtoll(str, end, 10);
#endif
		}

		/**
		 * Converts a given `wchar_t` string into a `long long` value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::wcstoll(str, end, 10)
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numerical value.
		 * @return
		 *     `long long` value equivalent to `str`.
		 */
		static inline long long strtoll(const wchar_t* str, wchar_t** end) {
#if defined(_MSC_VER) && _MSC_VER < 1800
			return static_cast< long long >(_wcstoi64(str, end, 10));
#else
			return wcstoll(str, end, 10);
#endif
		}

		/**
		 * Converts a given `char` string into an `unsigned long long` value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::strtoull(str, end, 10)
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numerical value.
		 * @return
		 *     `unsigned long long` value equivalent to `str`.
		 */
		static inline unsigned long long strtoull(const char* str, char** end) {
#if defined(_MSC_VER) && _MSC_VER < 1800
			return static_cast< unsigned long long >(_strtoui64(str, end, 10));
#else
			return ::strtoull(str, end, 10);
#endif
		}

		/**
		 * Converts a given `wchar_t` string  into an `unsigned long long`
		 * value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::wcstoull(str, end, 10)
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numerical value.
		 * @return
		 *     `unsigned long long` value equivalent to `str`.
		 */
		static inline unsigned long long
			strtoull(const wchar_t* str, wchar_t** end)
		{
#if defined(_MSC_VER) && _MSC_VER < 1800
			return static_cast< unsigned long long >(_wcstoui64(str, end, 10));
#else
			return wcstoull(str, end, 10);
#endif
		}

		/**
		 * Converts a given `char` string into a `double` value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::strtod(str, end);
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numerical value.
		 * @return
		 *     `double` value equivalent to `str`.
		 */
		static inline double strtod(const char* str, char** end) {
			return ::strtod(str, end);
		}

		/**
		 * Converts a given `wchar_t` string into a `double` value.
		 *
		 * Equivalent to the following call,
		 *
		 *     ::wcstod(str, end)
		 *
		 * @param str
		 *     C-string to be converted.
		 * @param[out] end
		 *     Set to the reference to the next character in `str` after
		 *     the numerical value.
		 * @return
		 *     `double` value equivalent to `str`.
		 */
		static inline double strtod(const wchar_t* str, wchar_t** end) {
			return ::wcstod(str, end);
		}

		/**
		 * Converts a given string into a value of a signed integer type `S`.
		 *
		 * `valueStr` will be converted into a `long long` value, and then
		 * it will be casted to `S`.
		 *
		 * @tparam S
		 *     Type which represents a value.
		 *     Must be a signed integer type.
		 * @tparam Ch
		 *     Type which represents a character.
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `S` value equivalent to `valueStr`.
		 * @throws BadValue< Ch >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `S`.
		 */
		template < typename S, typename Ch >
		static S toSigned(const std::basic_string< Ch >& valueStr) {
			if (valueStr.empty()) {
				throw BadValue< Ch >("invalid integer", valueStr);
			}
			Ch* end = 0;
			errno = 0;
			long long x = strtoll(valueStr.c_str(), &end);
			if (*end != Ch('\0')) {
				throw BadValue< Ch >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (x > std::numeric_limits< S >::max()) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (x < std::numeric_limits< S >::min()) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			return static_cast< S >(x);
		}

		/**
		 * Converts a given string into a value of an unsigned integer type `U`.
		 *
		 * `valueStr` will be converted into `unsigned long long`, and then
		 * it will be casted to `U`.
		 *
		 * @tparam U
		 *     Type which represents a value.
		 *     Must be an unsigned integer type.
		 * @tparam Ch
		 *     Type which represents a character.
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `U` value equivalent to `valueStr`.
		 * @throws BadValue< Ch >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `U`.
		 */
		template < typename U, typename Ch >
		static U toUnsigned(const std::basic_string< Ch >& valueStr) {
			if (valueStr.empty()) {
				throw BadValue< Ch >("invalid integer", valueStr);
			}
			Ch* end = 0;
			errno = 0;
			unsigned long long x = strtoull(valueStr.c_str(), &end);
			if (*end != Ch('\0')) {
				throw BadValue< Ch >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (x > std::numeric_limits< U >::max()) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (valueStr[0] == Ch('-')) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			return static_cast< U >(x);
		}

		/**
		 * Converts a given string into a floating point number.
		 *
		 * Undefined if `valueStr` causes underflow.
		 *
		 * @tparam F
		 *     Type which represents a floating point number.
		 * @tparam Ch
		 *     Type which represents a character.
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `F` value equivalent to `valueStr`.
		 * @throws BadValue< Ch >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not a number,
		 *     or if `valueStr` is out of the range representable by `F`.
		 */
		template < typename F, typename Ch >
		static F toFloat(const std::basic_string< Ch >& valueStr) {
			if (valueStr.empty()) {
				throw BadValue< Ch >("invalid number", valueStr);
			}
			Ch* end = 0;
			errno = 0;
			double x = strtod(valueStr.c_str(), &end);
			if (*end != Ch('\0')) {
				throw BadValue< Ch >("invalid number", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (x > std::numeric_limits< F >::max()) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			if (x < std::numeric_limits< F >::lowest()) {
				throw BadValue< Ch >("out of range", valueStr);
			}
			return static_cast< F >(x);
		}
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
		 * @return
		 *     `int` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `int`.
		 */
		int operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toSigned< int >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::string` into `unsigned int`.
	 */
	template <>
	class DefaultFormatter< unsigned int, char > {
	public:
		/**
		 * Converts a given string into an `unsigned int` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned int` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned int`.
		 */
		unsigned int operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toUnsigned< unsigned int >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::string` into `short`. */
	template <>
	class DefaultFormatter< short, char > {
	public:
		/**
		 * Converts a given string into a `short` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `short` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `short`.
		 */
		short operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toSigned< short >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::string` into `unsigned short`.
	 */
	template <>
	class DefaultFormatter< unsigned short, char > {
	public:
		/**
		 * Converts a given string into an `unsigned short` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned short` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned short`.
		 */
		unsigned short operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned short >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::string` into `long`. */
	template <>
	class DefaultFormatter< long, char > {
	public:
		/**
		 * Converts a given string into a `long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `long` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `long`.
		 */
		long operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toSigned< long >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::string` into `unsigned long`.
	 */
	template <>
	class DefaultFormatter< unsigned long, char > {
	public:
		/**
		 * Converts a given string into an `unsigned long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned long` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned long`.
		 */
		unsigned long operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned long >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::string` into `long long`. */
	template <>
	class DefaultFormatter< long long, char > {
	public:
		/**
		 * Converts a given string into a `long long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `long long` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `long long`.
		 */
		long long operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toSigned< long long >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::string` into
	 * `unsigned long long`.
	 */
	template <>
	class DefaultFormatter< unsigned long long, char > {
	public:
		/**
		 * Converts a given string into an `unsigned long long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned long long` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned long long`.
		 */
		unsigned long long operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned long long >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::string` into `double`. */
	template <>
	class DefaultFormatter< double, char > {
	public:
		/**
		 * Converts a given string into a `double` value.
		 *
		 * Undefined if `valueStr` causes underflow.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `double` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not a number,
		 *     or if `valueStr` is out of the range representable by `double`.
		 */
		double operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toFloat< double >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::string` into `float`. */
	template <>
	class DefaultFormatter< float, char > {
	public:
		/**
		 * Converts a given string into a `float` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `float` value equivalent to `valueStr`.
		 * @throws BadValue< char >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not a number,
		 *     or if `valueStr` is out of the range representable by `float`.
		 */
		float operator ()(const std::string& valueStr) const {
			return DefaultFormatterHelper::toFloat< float >(valueStr);
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
		 * @return
		 *     `int` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `int`.
		 */
		int operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toSigned< int >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::wstring` into `unsigned int`.
	 */
	template <>
	class DefaultFormatter< unsigned int, wchar_t > {
	public:
		/**
		 * Converts a given string into an `unsigned int` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned int` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned int`.
		 */
		unsigned int operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toUnsigned< unsigned int >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `short`. */
	template <>
	class DefaultFormatter< short, wchar_t > {
	public:
		/**
		 * Converts a given string into a `short` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `short` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `short`.
		 */
		short operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toSigned< short >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::wstring` into
	 * `unsigned short`.
	 */
	template <>
	class DefaultFormatter< unsigned short, wchar_t > {
	public:
		/**
		 * Converts a given string into an `unsigned short` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned short` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned short`.
		 */
		unsigned short operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned short >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `long`. */
	template <>
	class DefaultFormatter< long, wchar_t > {
	public:
		/**
		 * Converts a given string into a `long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `long` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by `long`.
		 */
		long operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toSigned< long >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::wstring` into `unsigned long`.
	 */
	template <>
	class DefaultFormatter< unsigned long, wchar_t > {
	public:
		/**
		 * Converts a given string into an `unsigned long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned long` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned long`.
		 */
		unsigned long operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned long >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `long long`. */
	template <>
	class DefaultFormatter< long long, wchar_t > {
	public:
		/**
		 * Converts a given string into a `long long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `long long` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `long long`.
		 */
		long long operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toSigned< long long >(valueStr);
		}
	};

	/**
	 * `DefaultFormatter` which converts an `std::wstring` into
	 * `unsigned long long`.
	 */
	template <>
	class DefaultFormatter< unsigned long long, wchar_t > {
	public:
		/**
		 * Converts a given string into an `unsigned long long` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `unsigned long long` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not an integer,
		 *     or if `valueStr` is out of the range representable by
		 *     `unsigned long long`.
		 */
		unsigned long long operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper
				::toUnsigned< unsigned long long >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `double`. */
	template <>
	class DefaultFormatter< double, wchar_t > {
	public:
		/**
		 * Converts a given string into a `double` value.
		 *
		 * Undefined if `valueStr` causes underflow.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `double` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not a number,
		 *     or if `valueStr` is out of the range representable by `double`.
		 */
		double operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toFloat< double >(valueStr);
		}
	};

	/** `DefaultFormatter` which converts an `std::wstring` into `float`. */
	template <>
	class DefaultFormatter< float, wchar_t > {
	public:
		/**
		 * Converts a given string into a `float` value.
		 *
		 * @param valueStr
		 *     String to be converted.
		 * @return
		 *     `float` value equivalent to `valueStr`.
		 * @throws BadValue< wchar_t >
		 *     If `valueStr` is empty,
		 *     or if `valueStr` is not a number,
		 *     or if `valueStr` is out of the range representable by `float`.
		 */
		float operator ()(const std::wstring& valueStr) const {
			return DefaultFormatterHelper::toFloat< float >(valueStr);
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
