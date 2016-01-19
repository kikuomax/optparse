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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid number", valueStr);
			}
			char* end = 0;
			errno = 0;
			long x = strtol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid number", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (x > std::numeric_limits< int >::max()) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (x < std::numeric_limits< int >::min()) {
				throw BadValue< char >("out of range", valueStr);
			}
			return static_cast< int >(x);
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			errno = 0;
			unsigned long x = strtoul(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (x > std::numeric_limits< unsigned int >::max()) {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			long x = strtol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (x > std::numeric_limits< short >::max()) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (x < std::numeric_limits< short >::min()) {
				throw BadValue< char >("out of range", valueStr);
			}
			return static_cast< short >(x);
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			unsigned long x = strtoul(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (x > std::numeric_limits< unsigned short >::max()) {
				throw BadValue< char >("out of range", valueStr);
			}
			return static_cast< unsigned short >(x);
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			errno = 0;
			long x = strtol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			errno = 0;
			unsigned long x = strtoul(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (valueStr[0] == '-') {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			errno = 0;
#if defined(_MSC_VER) && _MSC_VER < 1800
			long long x = static_cast< long long >(
				_strtoi64(valueStr.c_str(), &end, 10));
#else
			long long x = strtoll(valueStr.c_str(), &end, 10);
#endif
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid integer", valueStr);
			}
			char* end = 0;
			errno = 0;
#if defined(_MSC_VER) && _MSC_VER < 1800
			unsigned long long x = static_cast< unsigned long long >(
				_strtoui64(valueStr.c_str(), &end, 10));
#else
			unsigned long long x = strtoull(valueStr.c_str(), &end, 10);
#endif
			if (*end != '\0') {
				throw BadValue< char >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (valueStr[0] == '-') {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid number", valueStr);
			}
			char* end = 0;
			errno = 0;
			double x = strtod(valueStr.c_str(), &end);
			if (*end != '\0') {
				throw BadValue< char >("invalid number", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< char >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< char >("invalid number", valueStr);
			}
			char* end = 0;
			double x = strtod(valueStr.c_str(), &end);
			if (*end != '\0') {
				throw BadValue< char >("invalid number", valueStr);
			}
			if (x > std::numeric_limits< float >::max()) {
				throw BadValue< char >("out of range", valueStr);
			}
			if (x < std::numeric_limits< float >::lowest()) {
				throw BadValue< char >("out of range", valueStr);
			}
			return static_cast< float >(x);
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
			long x = wcstol(valueStr.c_str(), &end, 10);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (x > std::numeric_limits< int >::max()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (x < std::numeric_limits< int >::min()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (valueStr[0] == L'-') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
			unsigned long x = wcstoul(valueStr.c_str(), &end, 10);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (x > std::numeric_limits< unsigned int >::max()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			long x = wcstol(valueStr.c_str(), &end, 10);
			if (*end != '\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (x > std::numeric_limits< short >::max()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (x < std::numeric_limits< short >::min()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return static_cast< short >(x);
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			unsigned long x = wcstoul(valueStr.c_str(), &end, 10);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (x > std::numeric_limits< unsigned short >::max()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return static_cast< unsigned short >(x);
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
			long x = wcstol(valueStr.c_str(), &end, 10);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
			unsigned long x = wcstoul(valueStr.c_str(), &end, 10);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (valueStr[0] == L'-') {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
#if defined(_MSC_VER) && _MSC_VER < 1800
			long long x = static_cast< long long >(
				_wcstoi64(valueStr.c_str(), &end, 10));
#else
			long long x = wcstoll(valueStr.c_str(), &end, 10);
#endif
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
#if defined(_MSC_VER) && _MSC_VER < 1800
			unsigned long long x = static_cast< unsigned long long >(
				_wcstoui64(valueStr.c_str(), &end, 10));
#else
			unsigned long long x = wcstoull(valueStr.c_str(), &end, 10);
#endif
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid integer", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (valueStr[0] == L'-') {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid number", valueStr);
			}
			wchar_t* end = 0;
			errno = 0;
			double x = wcstod(valueStr.c_str(), &end);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid number", valueStr);
			}
			if (errno == ERANGE) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return x;
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
			if (valueStr.empty()) {
				throw BadValue< wchar_t >("invalid number", valueStr);
			}
			wchar_t* end = 0;
			double x = wcstod(valueStr.c_str(), &end);
			if (*end != L'\0') {
				throw BadValue< wchar_t >("invalid number", valueStr);
			}
			if (x > std::numeric_limits< float >::max()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			if (x < std::numeric_limits< float >::lowest()) {
				throw BadValue< wchar_t >("out of range", valueStr);
			}
			return static_cast< float >(x);
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
