#ifndef _OPTPARSE_OPTPARSE_DEFAULT_USAGE_PRINTER_H
#define _OPTPARSE_OPTPARSE_DEFAULT_USAGE_PRINTER_H

#include "optparse/OptionParserBase.h"

#include <iostream>

namespace optparse {

	/**
	 * Traits of `DefaultUsagePrinter`.
	 *
	 * Must be specialized.
	 *
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename Ch >
	class DefaultUsagePrinterTraits {
	public:
		/**
		 * Returns the default output stream.
		 *
		 * This function must be specialized.
		 *
		 * @return
		 *     Default output stream.
		 */
		static std::basic_ostream< Ch >& defaultOut();

		/**
		 * Converts a given `char` value into an equivalent `Ch` value.
		 *
		 * This function must be specialized.
		 *
		 * @param ch
		 *     Character to be converted.
		 * @return
		 *     `Ch` value equivalent to `ch`.
		 */
		static char fromChar(char ch);

		/**
		 * Converts a given `char` string into an equivalent `Ch` string.
		 *
		 * This function must be specialized.
		 *
		 * @param str
		 *     String to be converted.
		 * @return
		 *     `Ch` string equivalent to `str`.
		 */
		static std::basic_string< Ch > fromChar(const char* str);
	};

	/** Specialization of `DefaultUsagePrinterTraits` for `char`. */
	template <>
	class DefaultUsagePrinterTraits< char > {
	public:
		/** Returns the default output stream. Standard error. */
		inline static std::basic_ostream< char >& defaultOut() {
			return std::cerr;
		}

		/** Just returns a given character. */
		inline static char fromChar(char ch) {
			return ch;
		}

		/** Just returns a given string. */
		inline static const char* fromChar(const char* str) {
			return str;
		}
	};

	/** Specialization of `DefaultUsagePrinterTraits` for `wchar_t`. */
	template <>
	class DefaultUsagePrinterTraits< wchar_t > {
	public:
		/** Returns the default output stream. */
		inline static std::basic_ostream< wchar_t >& defaultOut() {
			return std::wcerr;
		}

		/**
		 * Converts a given `char` into an equivalent `wchar_t`.
		 *
		 * @param ch
		 *     Character to be converted into `wchar_t`.
		 * @return
		 *     `wchar_t` character equivalent to `ch`.
		 */
		inline static char fromChar(char ch) {
			return static_cast< wchar_t >(ch);
		}

		/**
		 * Converts a given `char` string into an equivalent `wchar_t` string.
		 *
		 * Undefined if `str` contains other than ASCII character(s).
		 *
		 * @param str
		 *     `char` string to be converted into an equivalent `wchar_t`
		 *     string.
		 * @return
		 *     `wchar_t` string equivalent to `str`.
		 */
		static std::wstring fromChar(const std::string& str) {
			return std::wstring(str.begin(), str.end());
		}
	};

	/**
	 * Default usage printer.
	 *
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename Ch >
	class DefaultUsagePrinter {
	private:
		/** `DefaultUsagePrinterTraits` for `Ch`. */
		typedef DefaultUsagePrinterTraits< Ch > Traits;

		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;

		/** Output stream where usage is written. */
		std::basic_ostream< Ch >& out;
	public:
		/**
		 * Initializes an instance associated with a given output stream.
		 *
		 * @param out
		 *     Output stream where usage is written.
		 *     Standard error by default.
		 */
		inline DefaultUsagePrinter(
			std::basic_ostream< Ch >& out = Traits::defaultOut())
			: out(out) {}

		/**
		 * Prints the usage of a given option parser.
		 *
		 * @tparam Opt
		 *     See `OptionParserBase`.
		 * @tparam MetaFormat
		 *     See `OptionParserBase`.
		 */
		template < typename Opt,
				   template < typename, typename > class MetaFormat >
		void printUsage(const OptionParserBase< Opt, Ch, MetaFormat >& parser) {
			this->out << "usage: " << parser.getProgramName();
			// appends optional arguments if exist
			if (parser.getOptionCount() > 0) {
				const size_t N = parser.getOptionCount();
				for (size_t i = 0; i < N; ++i) {
					const OptionSpec< Ch >& spec = parser.getOption(i);
					this->out << Traits::fromChar(" [") << spec.getLabel();
					if (spec.needsValue()) {
						this->out
							<< Traits::fromChar(" ") << spec.getValueName();
					}
					this->out << Traits::fromChar("]");
				}
			}
			// appends positional arguments if exist
			if (parser.getArgumentCount() > 0) {
				const size_t N = parser.getArgumentCount();
				for (size_t i = 0; i < N; ++i) {
					const ArgumentSpec< Ch >& spec = parser.getArgument(i);
					this->out << Traits::fromChar(" ") << spec.getValueName();
				}
			}
			this->out << std::endl;
			this->out << std::endl;
			this->out << parser.getDescription() << std::endl;
			// prints descriptions of positional arguments if exist
			if (parser.getArgumentCount() > 0) {
				// measures the maximum length of arguments
				size_t maxLen = measureMaximumArgumentLength(parser);
				this->out << std::endl;
				this->out
					<< Traits::fromChar("positional arguments:")
					<< std::endl;
				const size_t N = parser.getArgumentCount();
				for (size_t i = 0; i < N; ++i) {
					const ArgumentSpec< Ch >& arg = parser.getArgument(i);
					this->out
						<< Traits::fromChar("  ")
						<< padString(arg.getValueName(), maxLen)
						<< Traits::fromChar("  ")
						<< arg.getDescription()
						<< std::endl;
				}
			}
			// prints descriptions of optional arguments if exist
			if (parser.getOptionCount() > 0) {
				// measures the maximum length of options
				size_t maxLen = measureMaximumOptionLength(parser);
				this->out << std::endl;
				this->out
					<< Traits::fromChar("optional arguments:") << std::endl;
				const size_t N = parser.getOptionCount();
				for (size_t i = 0; i < N; ++i) {
					const OptionSpec< Ch >& option = parser.getOption(i);
					this->out
						<< Traits::fromChar("  ")
						<< padString(toString(option), maxLen)
						<< Traits::fromChar("  ")
						<< option.getDescription()
						<< std::endl;
				}
			}
			this->out << std::endl;
		}
	private:
		/**
		 * Measures the maximum length of the positional arguments which
		 * a given parser takes.
		 *
		 * @tparam Opt
		 *     See `OptionParserBase`.
		 * @tparam MetaFormat
		 *     See `OptionParserBase`.
		 * @param parser
		 *     Parser of which the maximum argument length is to be measured.
		 * @return
		 *     Maximum length of the positional arguments.
		 *     0 if `parser` takes no positional arguments.
		 */
		template < typename Opt,
				   template < typename, typename > class MetaFormat >
		static size_t measureMaximumArgumentLength(
			const OptionParserBase< Opt, Ch, MetaFormat >& parser)
		{
			size_t maxLen = 0;
			const size_t N = parser.getArgumentCount();
			for (size_t i = 0; i < N; ++i) {
				const ArgumentSpec< Ch >& spec = parser.getArgument(i);
				maxLen = std::max(spec.getValueName().size(), maxLen);
			}
			return maxLen;
		}

		/**
		 * Measures the maximum length of the optional arguments which
		 * a given parser takes.
		 *
		 * @tparam Opt
		 *     See `OptionParserBase`.
		 * @tparam MetaFormat
		 *     See `OptionParserBase`.
		 * @param parser
		 *     Parser of which the maximum option length is to be measured.
		 * @return
		 *     Maximum length of the optional arguments.
		 *     0 if `parser` takes no optional arguments.
		 */
		template < typename Opt,
				   template < typename, typename > class MetaFormat >
		static size_t measureMaximumOptionLength(
			const OptionParserBase< Opt, Ch, MetaFormat >& parser)
		{
			size_t maxLen = 0;
			const size_t N = parser.getOptionCount();
			for (size_t i = 0; i < N; ++i) {
				const OptionSpec< Ch >& option = parser.getOption(i);
				maxLen = std::max(toString(option).size(), maxLen);
			}
			return maxLen;
		}

		/**
		 * Returns the string form of a given option.
		 *
		 * @param option
		 *     Option to be formatted as a string.
		 * @return
		 *     String form of `option`.
		 */
		static String toString(const OptionSpec< Ch >& option) {
			String str(option.getLabel());
			if (option.needsValue()) {
				str += Traits::fromChar(" ") + option.getValueName();
			}
			return str;
		}

		/**
		 * Pads a given string with whitespace so that it will have the fixed
		 * length.
		 *
		 * Just returns `str` if `str.size() >= len`.
		 *
		 * @param str
		 *     String to be padded.
		 * @param len
		 *     Target length.
		 * @return
		 *     Padded string.
		 */
		static String padString(const String& str, size_t len) {
			// immediately returns `str`
			// if its length is longer than or equal to `len`
			if (str.size() >= len) {
				return str;
			}
			size_t n = len - str.size();
			String padded(str);
			padded.insert(str.size(), n, Traits::fromChar(' '));
			return padded;
		}
	};

}

#endif
