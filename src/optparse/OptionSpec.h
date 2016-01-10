#ifndef _OPTPARSE_OPTPARSE_OPTION_SPEC_H
#define _OPTPARSE_OPTPARSE_OPTION_SPEC_H

#include <string>

namespace optparse {

	/**
	 * Specification of an option.
	 *
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename Ch >
	class OptionSpec {
	public:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;
	public:
		/** Releases the resources. */
		virtual ~OptionSpec() {}

		/**
		 * Returns the label of this option.
		 *
		 * @return
		 *     Label of this option.
		 */
		virtual const String& getLabel() const = 0;

		/**
		 * Returns the description of this option.
		 *
		 * @return
		 *     Description of this option.
		 */
		virtual const String& getDescription() const = 0;

		/**
		 * Returns whether this option needs a value.
		 *
		 * @return
		 *     Whether this option needs a value.
		 */
		virtual bool needsValue() const = 0;

		/**
		 * Returns the name of the value which this option takes.
		 *
		 * This name is used to explain what should be specified to this
		 * option.
		 *
		 * @return
		 *     Name of the value which this option takes.
		 *     Empty string if this option does not take a value.
		 */
		virtual const String& getValueName() const = 0;
	};

	/**
	 * Specification of an argument.
	 *
	 * @tparam Ch
	 *     Type which represents a character.
	 */
	template < typename Ch >
	class ArgumentSpec {
	public:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;
	public:
		/** Releases resources. */
		virtual ~ArgumentSpec() {}

		/**
		 * Returns the description of this argument.
		 *
		 * @return
		 *     Description of this argument.
		 */
		virtual const String& getDescription() const = 0;

		/**
		 * Returns the name of the value which this argument takes.
		 *
		 * This name is used to explain what should be specified to this
		 * argument.
		 *
		 * @return
		 *     Name of the value which this argument takes.
		 */
		virtual const String& getValueName() const = 0;
	};

}

#endif
