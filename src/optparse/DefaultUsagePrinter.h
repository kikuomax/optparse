#ifndef _OPTPARSE_OPTPARSE_DEFAULT_USAGE_PRINTER_H
#define _OPTPARSE_OPTPARSE_DEFAULT_USAGE_PRINTER_H

#include "optparse/OptionParserBase.h"

#include <iostream>

namespace optparse {

	/**
	 * Default usage printer.
	 *
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename Ch >
	class DefaultUsagePrinter {
	private:
		/**
		 * Prints the usage of a given option parser.
		 *
		 * @tparam Opt
		 *     Type of an options container.
		 *     See `OptionParserBase`.
		 * @tparam MetaFormat
		 *     Template type which converts a string into a value.
		 *     See `OptionParserBase`.
		 */
		template < typename Opt,
				   template < typename, typename > class MetaFormat >
		void printUsage(
			const OptionParserBase< Opt, Ch, MetaFormat >& parser);
	};

	/** Specialization of `DefaultUsagePrinter` for `char`. */
	template <>
	class DefaultUsagePrinter< char > {
	private:
		/** Output stream. */
		std::ostream& out;
	public:
		/** Initializes an instance associated with the standard error. */
		inline DefaultUsagePrinter() : out(std::cerr) {}

		/** Prints the usage of a given option parser. */
		template < typename Opt,
				   template < typename, typename > class MetaFormat >
		void printUsage(
			const OptionParserBase< Opt, char, MetaFormat >& parser)
		{
			out << "Usage: " << parser.getProgramName() << std::endl;
			out << std::endl;
			out << parser.getDescription() << std::endl;
		}
	};

}

#endif
