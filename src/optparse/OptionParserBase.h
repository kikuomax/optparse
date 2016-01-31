#ifndef _OPTPARSE_OPTPARSE_OPTION_PARSER_BASE_H
#define _OPTPARSE_OPTPARSE_OPTION_PARSER_BASE_H

#include "optparse/OptionParserException.h"
#include "optparse/OptionSpec.h"

#include <algorithm>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace optparse {

	/**
	 * Parser for command line options.
	 *
	 * ## MetaFormat
	 *
	 * `MetaFormat` is a template class which takes the following type
	 * parameters.
	 *  1. Type of a value `T`
	 *  2. Type which represents a character; i.e., `Ch`
	 *
	 * `MetaFormat` must support a default constructor, and an instance of
	 * `MetaFormat` must support a function call similar to the following,
	 *
	 *     T operator ()(const std::basic_string< Ch >& str) const
	 *
	 * It must take a string representation of the value `str` and return the
	 * formatted value.
	 * If formatting fails, it must throw `BadValue< Ch >`.
	 *
	 * ## Other Type Parameters
	 *
	 * Throughout this class, the following type parameters are also used,
	 *  - `T`
	 *  - `SupOpt`
	 *  - `Format`
	 *
	 * ### T
	 *
	 * A type of a value which an option can take.
	 * It must support a default constructor and assignment.
	 *
	 * ### SupOpt
	 *
	 * A type of an options container, which is either `Opt` and a super type
	 * of `Opt`.
	 *
	 * ### Format
	 *
	 * A type of a formatter which converts a string representation into
	 * a value of the type `T`.
	 * A specialization of `MetaFormat` by default.
	 * `Format` must support a copy constructor and an instance of `Format`
	 * must support a function call similar to the following,
	 *
	 *     T operator ()(const std::basic_string< Ch >& str) const
	 *
	 * It must take a string representation of a value and return the formatted
	 * value.
	 * If formatting fails, it must throw `BadValue< Ch >`.
	 *
	 * @tparam Opt
	 *     Type of a container for option values.
	 *     Must support a default constructor.
	 * @tparam Ch
	 *     Type which represents a character.
	 * @tparam MetaFormat
	 *     Template type which converts a string into a typed value.
	 */
	template < typename Opt,
			   typename Ch,
			   template < typename, typename > class MetaFormat >
	class OptionParserBase {
	public:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;
	protected:
		/** Processor for an optional argument. */
		class Option : public OptionSpec< Ch > {
		protected:
			/** Label of this option. */
			String label;

			/** Description of this option. */
			String description;
		public:
			/**
			 * Initializes an option with a given description.
			 *
			 * @param label
			 *     Label of the option.
			 * @param description
			 *     Description of the option.
			 */
			inline Option(const String& label, const String& description)
				: label(label), description(description) {}

			/**
			 * Returns the label of this option.
			 *
			 * @return
			 *     Label of this option.
			 */
			virtual const String& getLabel() const {
				return this->label;
			}

			/**
			 * Returns the description of this option.
			 *
			 * @return
			 *     Description of this option.
			 */
			virtual const String& getDescription() const {
				return this->description;
			}

			/**
			 * Returns whether this option needs a value.
			 *
			 * @return
			 *     Whether this option needs a value.
			 */
			virtual bool needsValue() const = 0;

			/**
			 * Returns the name of the option value.
			 *
			 * This name is used to explain what should be specified to
			 * this option.
			 *
			 * @return
			 *     Name of the option value.
			 *     Empty string if this option does not take a value.
			 */
			virtual const String& getValueName() const = 0;

			/**
			 * Applies this option without a value.
			 *
			 * @param options
			 *     Options container to which this option is to be applied.
			 * @throws ValueNeeded
			 *     If this option needs a value.
			 */
			virtual void operator ()(Opt& options) = 0;

			/**
			 * Applies this option with a given value.
			 *
			 * @param options
			 *     Options container to which this option is to be applied.
			 * @param value
			 *     Value given to this option.
			 * @throws BadValue
			 *     If this option does not take a value,
			 *     or if `value` is invalid.
			 */
			virtual void operator ()(Opt& options, const String& value) = 0;
		};

		/** Processor for a positional argument. */
		class Argument : public ArgumentSpec< Ch > {
		protected:
			/** Name of this argument. */
			String name;

			/** Description of this argument. */
			String description;
		public:
			/**
			 * Initializes with a description and a value name.
			 *
			 * @param name
			 *     Name of this argument.
			 * @param description
			 *     Description of the argument.
			 */
			inline Argument(const String& name, const String& description)
				: name(name), description(description) {}

			/**
			 * Returns the name of this argument.
			 *
			 * @return
			 *     Name of this argument.
			 */
			inline const String& getValueName() const {
				return this->name;
			}

			/**
			 * Returns the description of this argument.
			 *
			 * @return
			 *     Description of this option.
			 */
			inline const String& getDescription() const {
				return this->description;
			}

			/**
			 * Applies this argument with a given value.
			 *
			 * @param options
			 *     Options container to which this argument is to be applied.
			 * @param value
			 *     Value given to this argument.
			 * @throws BadValue
			 *     If this argument does not take a value,
			 *     or if `value` is invalid.
			 */
			virtual void operator ()(Opt& options, const String& value) = 0;
		};

		/** `Option` which takes a value. */
		class ValueOption : public Option {
		protected:
			/** Name of the option value. */
			String valueName;
		public:
			/**
			 * Initializes an option that takes a value with a given name.
			 *
			 * @param label
			 *     Label of the option.
			 * @param description
			 *     Description of the option.
			 * @param valueName
			 *     Name of the value.
			 */
			inline ValueOption(const String& label,
							   const String& valueName,
							   const String& description)
				: Option(label, description), valueName(valueName) {}

			/** Takes a value; i.e., returns `true`. */
			virtual bool needsValue() const {
				return true;
			}

			/** Returns the name given at the construction. */
			virtual const String& getValueName() const {
				return this->valueName;
			}

			/** Needs a value; i.e., throws `ValueNeeded`. */
			virtual void operator ()(Opt&) {
				throw ValueNeeded< Ch >(this->label);
			}
		};

		/** `Option` which does not take values. */
		class NoValueOption : public Option {
		public:
			/**
			 * Initializes an option that does not take values.
			 *
			 * @param label
			 *     Label of the option.
			 * @param description
			 *     Description of the option.
			 */
			inline NoValueOption(const String& label, const String& description)
				: Option(label, description) {}

			/** Does not take values; i.e., returns `false`. */
			virtual bool needsValue() const {
				return false;
			}

			/** Does not take values; i.e., returns an empty string. */
			virtual const String& getValueName() const {
				static String empty;
				return empty;
			}

			/** Does not take values; i.e., throws `BadValue`. */
			virtual void operator ()(Opt&, const String&) {
				throw BadValue< Ch >("no value needed", this->label);
			}
		};

		/**
		 * `Option` which substitutes a member field.
		 *
		 * @tparam T
		 *     See `OptionParserBase`
		 * @tparam SupOpt
		 *     See `OptionParserBase`
		 * @tparam Format
		 *     See `OptionParserBase`
		 */
		template < typename T, typename SupOpt, typename Format >
		class MemberOption : public ValueOption {
		private:
			/** Field to be substituted. */
			T (SupOpt::*field);

			/** Formats a string as a value of `T`. */
			Format format;
		public:
			/**
			 * Initializes an option which substitutes a given field.
			 *
			 * @param label
			 *     Label of the option.
			 * @param name
			 *     Name of the value which the option takes.
			 *     Used to explain what should be specified to the option.
			 * @param description
			 *     Description of the option.
			 * @param field
			 *     Pointer to the field to be substituted.
			 * @param format
			 *     Formatter for the option value.
			 */
			inline MemberOption(const String& label,
								const String& name,
								const String& description,
								T (SupOpt::*field),
								const Format& format)
				: ValueOption(label, name, description),
				  field(field),
				  format(format) {}

			/**
			 * Formats a given string and sets the field of a given options
			 * container to the formatted value.
			 */
			virtual void operator ()(Opt& options, const String& value) {
				try {
					options.*(this->field) = this->format(value);
				} catch (BadValue< Ch >& ex) {
					// augments the exception with the label
					throw BadValue< Ch >(ex.getMessage(), this->label, value);
				}
			}
		};

		/**
		 * `Option` that substitutes a member field with a constant.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 */
		template < typename T, typename SupOpt >
		class ConstMemberOption : public NoValueOption {
		private:
			/** Field to be substituted. */
			T (SupOpt::*field);

			/** Constant value to substitute the field. */
			T constant;
		public:
			/**
			 * Initializes an option that substitutes a given field.
			 *
			 * @param label
			 *     Label of the option.
			 * @param description
			 *     Description of the option.
			 * @param field
			 *     Pointer to the field to be substituted.
			 * @param constant
			 *     Constant to substitute the field.
			 */
			inline ConstMemberOption(const String& label,
									 const String& description,
									 T (SupOpt::*field),
									 T constant)
				: NoValueOption(label, description),
				  field(field),
				  constant(constant) {}

			/** Applies this option without a value. */
			virtual void operator ()(Opt& options) {
				options.*(this->field) = this->constant;
			}
		};

		/**
		 * `Option` that calls a given function.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 */
		template < typename T, typename SupOpt, typename Format >
		class FunctionOption : public ValueOption {
		private:
			/** Function to be called. */
			void (*f)(SupOpt&, const T&);

			/** Formats a string as a value of the type `T`. */
			Format format;
		public:
			/**
			 * Initializes an option which calls a given function.
			 *
			 * @param label
			 *     Label of the option.
			 * @param name
			 *     Name of the value which the option takes.
			 *     Used to explain what should be specified to the option.
			 * @param description
			 *     Description of the option.
			 * @param f
			 *     Function to be called when the option is specified.
			 * @param format
			 *     Formatter for the option.
			 */
			inline FunctionOption(const String& label,
								  const String& name,
								  const String& description,
								  void (*f)(SupOpt&, const T&),
								  const Format& format = Format())
				: ValueOption(label, name, description),
				  f(f),
				  format(format) {}

			/**
			 * Formats a given string and passes the formatted value to the
			 * function specified at the construction.
			 */
			virtual void operator ()(Opt& options, const String& value) {
				try {
					this->f(options, this->format(value));
				} catch (BadValue< Ch >& ex) {
					// augments the exception with the label
					throw BadValue< Ch >(ex.getMessage(), this->label, value);
				}
			}
		};

		/**
		 * `Option` that calls a given function without values.
		 *
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 */
		template < typename SupOpt >
		class ConstFunctionOption : public NoValueOption {
		private:
			/** Function to be called when this option is specified. */
			void (*f)(SupOpt&);
		public:
			/**
			 * Initializes an option that calls a given function.
			 *
			 * @param label
			 *     Label of the option.
			 * @param description
			 *     Description of the option.
			 * @param f
			 *     Function to be called when the option is specified.
			 */
			inline ConstFunctionOption(const String& label,
									   const String& description,
									   void (*f)(SupOpt&))
				: NoValueOption(label, description), f(f) {}

			/** Calls the function given at the construction. */
			virtual void operator ()(Opt& options) {
				this->f(options);
			}
		};

		/**
		 * `Argument` that substitutes a member field.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 */
		template < typename T, typename SupOpt, typename Format >
		class MemberArgument : public Argument {
		private:
			/** Pointer to the field of `SupOpt` to be substituted. */
			T (SupOpt::*field);

			/** Formats a string as a value of type `T`. */
			Format format;
		public:
			/**
			 * Initialies an argument that substitues a given field.
			 *
			 * @param name
			 *     Name of the argument.
			 *     Used to explain what should be specified to the argument.
			 * @param description
			 *     Description of the argument.
			 * @param field
			 *     Pointer to the field of `SupOpt` to be substituted.
			 * @param format
			 *     Formatter for the argument value.
			 */
			inline MemberArgument(const String& name,
								  const String& description,
								  T (SupOpt::*field),
								  const Format& format)
				: Argument(name, description),
				  field(field),
				  format(format) {}

			/**
			 * Formats a given string and sets the field of a given options
			 * container to the formatted value.
			 */
			virtual void operator ()(Opt& options, const String& value) {
				try {
					options.*(this->field) = this->format(value);
				} catch (BadValue< Ch >& ex) {
					// augments the exception with the name
					throw BadValue< Ch >(ex.getMessage(), this->name, value);
				}
			}
		};

		/**
		 * Argument which calls a function.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 */
		template < typename T, typename SupOpt, typename Format >
		class FunctionArgument : public Argument {
		private:
			/** Function to be called when this argument is specified. */
			void (*f)(SupOpt&, const T&);

			/** Formats a string as a value of the type `T`. */
			Format format;
		public:
			/**
			 * Initializes an argument which calls a given function.
			 *
			 * @param name
			 *     Name of the argument.
			 *     Used to explain what should be specified to the argument.
			 * @param description
			 *     Description of the argument.
			 * @param f
			 *     Function to be called when the argument is specified.
			 * @param format
			 *     Function object which formats a string as a value of
			 *     the type `T`.
			 */
			FunctionArgument(const String& name,
							 const String& description,
							 void (*f)(SupOpt&, const T&),
							 const Format& format = Format())
				: Argument(name, description), f(f), format(format) {}

			/**
			 * Formats a given string and calls the function specified
			 * at the construction with the formatted value.
			 */
			virtual void operator ()(SupOpt& options, const String& value) {
				try {
					this->f(options, this->format(value));
				} catch (BadValue< Ch >& ex) {
					throw BadValue< Ch >(ex.getMessage(), this->name, value);
				}
			}
		};
	private:
		/** Pointer to an option definition. */
		typedef std::shared_ptr< Option > OptionPtr;

		/** Pointer to an argument. */
		typedef std::shared_ptr< Argument > ArgumentPtr;

		/** Index and option. */
		typedef std::pair< int, OptionPtr > IndexedOptionPtr;

		/** Type of an option map. */
		typedef std::map< String, IndexedOptionPtr > OptionMap;

		/** Value type of `OptionMap`. */
		typedef typename OptionMap::value_type OptionMapValue;

		/** Iterator type of `OptionMap`. */
		typedef typename OptionMap::iterator OptionMapItr;

		/** Description of the program. */
		String description;

		/** Name of the program. Empty by default. */
		String programName;

		/** List of options. */
		std::vector< OptionPtr > optionList;

		/** Maps an option label to the corresponding `Option`. */
		OptionMap optionMap;

		/** List of positional arguments. */
		std::vector< ArgumentPtr > arguments;
	public:
		/**
		 * Initializes with the description of the program.
		 *
		 * @param description
		 *     Description of the program.
		 */
		explicit OptionParserBase(const String& description)
			: description(description)
		{
			this->optionList.reserve(10);
		}

		/** Releases resources. */
		virtual ~OptionParserBase() {}

		/**
		 * Returns the description of the program.
		 *
		 * @return
		 *     Description of the program.
		 */
		inline const String& getDescription() const {
			return this->description;
		}

		/**
		 * Returns the program name.
		 *
		 * @return
		 *     Name of the program.
		 *     An empty string if `parse` has not yet been called.
		 */
		inline const String& getProgramName() const {
			return this->programName;
		}

		/**
		 * Returns the number of the registered options.
		 *
		 * @return
		 *     Number of the registered options.
		 */
		inline size_t getOptionCount() const {
			return this->optionList.size();
		}

		/**
		 * Returns the specification of the option at a given index.
		 *
		 * Undefined if `i >= this->getOptionCount()`.
		 *
		 * @param i
		 *     Index of the option of which the specification is to be obtained.
		 * @return
		 *     Specification of the option at `i`.
		 */
		const OptionSpec< Ch >& getOption(size_t i) const {
			return *this->optionList[i];
		}

		/**
		 * Adds an option which substitutes a given field.
		 *
		 * Equivalent to the following call,
		 *
		 *     this->addOption(label, name, description, field, MetaFormat< T, Ch >())
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param name
		 *     Name of the value which the option takes.
		 *     Used to explain what should be specified to the option.
		 * @param description
		 *     Description of the option.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename T, typename SupOpt >
		inline void addOption(const String& label,
							  const String& name,
							  const String& description,
							  T (SupOpt::*field))
		{
			this->addOption(
				label, name, description, field, MetaFormat< T, Ch >());
		}

		/**
		 * Adds an option which substitutes a given field with a value
		 * formatted by a given function.
		 *
		 * If an option associated with `label` already exists in this parser,
		 * it will be replaced with the new option.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param name
		 *     Name of the value which the option takes.
		 *     Used to explain what should be specified to the option.
		 * @param description
		 *     Description of the option.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @param format
		 *     Function object which converts a string into a value of the
		 *     type `T`.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename T, typename SupOpt, typename Format >
		void addOption(const String& label,
					   const String& name,
					   const String& description,
					   T (SupOpt::*field),
					   const Format& format)
		{
			OptionPtr pOption(new MemberOption< T, SupOpt, Format >(
				label, name, description, field, format));
			this->addOption(label, pOption);
		}

		/**
		 * Adds an option which substitutes a given field with a given constant.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be replaced with the new option.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param description
		 *     Description of the option.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @param constant
		 *     Constant that substitutes the field.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename T, typename SupOpt >
		void addOption(const String& label,
					   const String& description,
					   T (SupOpt::*field),
					   const T& constant)
		{
			OptionPtr pOption(new ConstMemberOption
				< T, SupOpt >(label, description, field, constant));
			this->addOption(label, pOption);
		}

		/**
		 * Adds an option which calls a given function.
		 *
		 * Equivalent to the following call,
		 *
		 *     this->addOption(label, name, description, f, MetaFormat< T, Ch >())
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param name
		 *     Name of the value which the option takes.
		 *     Used to explain what should be specified to the option.
		 * @param description
		 *     Description of the option.
		 * @param f
		 *     Function to be called when the option is specified.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename T, typename SupOpt >
		inline void addOption(const String& label,
							  const String& name,
							  const String& description,
							  void (*f)(SupOpt&, const T&))
		{
			this->addOption(label, name, description, f, MetaFormat< T, Ch >());
		}

		/**
		 * Adds an option which calls a given function with a value formatted
		 * by a given formatter function.
		 *
		 * If an option corresponding to `label` already exists in the parser,
		 * it will be replaced with the new option.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param name
		 *     Name of the value which the option takes.
		 *     Used to explain what should be specified to the option.
		 * @param description
		 *     Description of the option.
		 * @param f
		 *     Function to be called when the option is specified.
		 * @param format
		 *     Function object which converts a string into a value of the
		 *     type `T`.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename T, typename SupOpt, typename Format >
		void addOption(const String& label,
					   const String& name,
					   const String& description,
					   void (*f)(SupOpt&, const T&),
					   const Format& format)
		{
			OptionPtr pOption(new FunctionOption< T, SupOpt, Format >(
				label, name, description, f, format));
			this->addOption(label, pOption);
		}

		/**
		 * Adds an option which calls a given function without argument.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be replaced with the new option.
		 *
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param label
		 *     Option label on the command line.
		 * @param description
		 *     Description of the option.
		 * @param f
		 *     Function to be called when the option is specified.
		 * @throws ConfigException
		 *     If `label` cannot be an option label
		 *     (see `OptionParserBase::isLabel`).
		 */
		template < typename SupOpt >
		void addOption(const String& label,
					   const String& description,
					   void (*f)(SupOpt&))
		{
			OptionPtr pOption(
				new ConstFunctionOption< SupOpt >(label, description, f));
			this->addOption(label, pOption);
		}

		/**
		 * Returns the number of the registered arguments.
		 *
		 * @return
		 *     Number of the registered arguments.
		 */
		inline size_t getArgumentCount() const {
			return this->arguments.size();
		}

		/**
		 * Returns the specification of the argument at a given index.
		 *
		 * Undefined if `i >= this->getArgumentCount()`.
		 *
		 * @param i
		 *     Index of the argument of which the specification is to be
		 *     obtained.
		 * @return
		 *     Specification of the argument at the index `i`.
		 */
		inline const ArgumentSpec< Ch >& getArgument(size_t i) const {
			return *this->arguments[i];
		}

		/**
		 * Appends an argument which subsitutes a given field.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param name
		 *     Name of the argument.
		 *     Used to explain what should be specified to the argument.
		 * @param description
		 *     Description of the argument.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 */
		template < typename T, typename SupOpt >
		void appendArgument(const String& name,
							const String& description,
							T (SupOpt::*field))
		{
			this->appendArgument(
				name, description, field, MetaFormat< T, Ch >());
		}

		/**
		 * Appends an argument which substitutes a given field with a value
		 * formatted by a given formatter.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 * @param name
		 *     Name of the argument.
		 *     Used to explain what should be specified to the argument.
		 * @param description
		 *     Description of the argument.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @param format
		 *     Function object which converts a string into a value of the
		 *     type `T`.
		 */
		template < typename T, typename SupOpt, typename Format >
		void appendArgument(const String& name,
							const String& description,
							T (SupOpt::*field),
							const Format& format)
		{
			ArgumentPtr pArg(new MemberArgument
				< T, SupOpt, Format >(name, description, field, format));
			this->arguments.push_back(pArg);
		}

		/**
		 * Appends an argument which calls a given function.
		 *
		 * Equivalent to the following call,
		 *
		 *     this->appendArgument(name, description, f, MetaFormat< T, Ch >())
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @param name
		 *     Name of the argument.
		 *     Used to explain what should be specified to the argument.
		 * @param description
		 *     Description of the argument.
		 * @param f
		 *     Function to be called when the argument is specified.
		 */
		template < typename T, typename SupOpt >
		void appendArgument(const String& name,
							const String& description,
							void (*f)(SupOpt&, const T&))
		{
			this->appendArgument(name, description, f, MetaFormat< T, Ch >());
		}

		/**
		 * Appends an argument which calls a given function with a value
		 * formatted by a given formatter.
		 *
		 * @tparam T
		 *     See `OptionParserBase`.
		 * @tparam SupOpt
		 *     See `OptionParserBase`.
		 * @tparam Format
		 *     See `OptionParserBase`.
		 * @param name
		 *     Name of the argument.
		 *     Used to explain what should be specified to the argument.
		 * @param description
		 *     Description of the argument.
		 * @param f
		 *     Function to be called when the argument is specified.
		 * @param format
		 *     Function object which converts a string into a value of the
		 *     type `T`.
		 */
		template < typename T, typename SupOpt, typename Format >
		void appendArgument(const String& name,
							const String& description,
							void (*f)(SupOpt&, const T&),
							const Format& format)
		{
			ArgumentPtr pArg(new FunctionArgument
				< T, SupOpt, Format >(name, description, f, format));
			this->arguments.push_back(pArg);
		}

		/**
		 * Parses given command line arguments.
		 *
		 * @param argc
		 *     Number of the command line arguments including the program name.
		 * @param argv
		 *     Command line arguments. First element must be the program name.
		 * @return
		 *     Parsed option values.
		 * @throws TooFewArguments
		 *     Thrown when too few arguments are given.
		 * @throws TooManyArguments
		 *     Thrown when too many arguments are given.
		 * @throws ValueNeeded
		 *     Thrown when no value is given to some option which needs a value.
		 * @throws BadValue
		 *     Thrown when a bad value is given to some option.
		 * @throws UnknownOption
		 *     Thrown when an unknown option is given.
		 */
		Opt parse(int argc, const Ch* const* argv) {
			Opt options;
			// aborts if no arguments are specified
			if (argc <= 0) {
				throw TooFewArguments();
			}
			// updates the program name
			this->programName = argv[0];
			// processes rest of arguments
			int argI = 1;
			size_t nextPos = 0;  // index of the next positional argument
			while (argI < argc) {
				// checks if `argv[argI]` is an option label
				if (isLabel(argv[argI])) {
					// processes an option
					const Ch* label = argv[argI];
					OptionPtr pOption = this->findOption(label);
					if (pOption) {
						// processes a value if necessary
						if (pOption->needsValue()) {
							// applies the option value
							if (argI + 1 < argc) {
								++argI;
								const Ch* value = argv[argI];
								(*pOption)(options, value);
							} else {
								throw ValueNeeded< Ch >(label);
							}
						} else {
							// applies the option without a value
							(*pOption)(options);
						}
					} else {
						throw UnknownOption< Ch >(label);
					}
				} else {
					// processes the next positional argument
					// aborts if too many arguments are given
					if (nextPos == this->arguments.size()) {
						throw TooManyArguments();
					}
					Argument& posArg = *this->arguments[nextPos++];
					posArg(options, argv[argI]);
				}
				++argI;
			}
			// makes sure that all of the positional arguments were substituted
			if (nextPos != this->arguments.size()) {
				throw TooFewArguments();
			}
			return options;
		}

		/**
		 * Returns whehter a given string is an option label.
		 *
		 * An option label satisfies all of the following conditions,
		 *  - Starts with a dash (`-`)
		 *  - The first dash is not followed by digits (`0-9`) or a dot (`.`)
		 *
		 * @param label
		 *     String to be tested.
		 * @return
		 *     Whether `label` is an option label.
		 */
		static bool isLabel(const String& label) {
			if (label.empty()) {
				return false;
			}
			if (label[0] != Ch('-')) {
				return false;
			}
			if (label.size() == 1) {
				return true;
			}
			const Ch c = label[1];
			return c != Ch('.') && !(Ch('0') <= c && c <= Ch('9'));
		}
	protected:
		/**
		 * Adds a given option to this parser.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be replaced with `pOption`.
		 *
		 * @param label
		 *     Option label on the command line.
		 *     Must start with a dash (`-`); e.g., "-o", "--option".
		 * @param pOption
		 *     Pointer to the option to be added.
		 *     Must be allocated by the standard new operator.
		 * @throws ConfigException
		 *     If `label` does not start with a dash.
		 */
		void addOption(const String& label, OptionPtr pOption) {
			verifyLabel(label);
			std::pair< OptionMapItr, bool > insertion = this->optionMap.insert(
				OptionMapValue(label, IndexedOptionPtr(-1, pOption)));
			if (!insertion.second) {
				// replaces an existing option
				int i = insertion.first->second.first;
				insertion.first->second.second = pOption;
				this->optionList[i] = pOption;
			} else {
				// new otpion
				insertion.first->second.first =
					static_cast< int >(this->optionList.size());
				this->optionList.push_back(pOption);
			}
		}

		/**
		 * Finds the option that has to a given label.
		 *
		 * @param label
		 *     Label of the option to be searched.
		 * @return
		 *     Option that has `label`. 0 if no option has `label`.
		 */
		OptionPtr findOption(const String& label) {
			OptionMapItr optionItr = this->optionMap.find(label);
			return optionItr != this->optionMap.end()
				? optionItr->second.second : 0;
		}

		/**
		 * Checks whether a given option label is valid.
		 *
		 * `label` is tested by the `OptionParserBase::isLabel` function.
		 *
		 * @param label
		 *     Label to be tested.
		 * @throws ConfigException
		 *     If `label` cannot be an option label.
		 * @see  isLabel
		 */
		static void verifyLabel(const String& label) {
			if (!isLabel(label)) {
				throw ConfigException("option label must start with dash (-)");
			}
		}
	private:
	};

}

#endif
