#ifndef _OPTPARSE_OPTION_PARSER_BASE_H
#define _OPTPARSE_OPTION_PARSER_BASE_H

#include "OptionParserException.h"

#include <algorithm>
#include <map>
#include <string>

namespace optparse {

	/**
	 * Parser for command line options.
	 *
	 * `Opt`: Type of a container for option values.
	 *
	 * `Validator`: Type of the validator for `Opt`.
	 *
	 * `MetaFormat`: Type of the formatter from a string to a typed value.
	 *
	 * `Ch`: Type of an input character.
	 */
	template < typename Opt,
			   template < typename, typename > class Validator,
			   template < typename, typename > class MetaFormat,
			   typename Ch >
	class OptionParserBase {
	public:
		/** Type of a string. */
		typedef std::basic_string< Ch > String;
	protected:
		/** Option processor. */
		class Option {
		protected:
			/** Description of this option. */
			String description;
		public:
			/**
			 * Initializes an option with a given description.
			 *
			 * @param description
			 *     Description of the option.
			 */
			Option(const String& description) : description(description) {}

			/** Releases resources. */
			virtual ~Option() {}

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
			 * Returns the name of the value.
			 *
			 * @return
			 *     Name of the value.
			 *     An empty string if this option takes no value.
			 */
			virtual String getValueName() const = 0;

			/**
			 * Applies this option without a value.
			 *
			 * @param options
			 *     Options container to which this option is to be applied.
			 * @throws OptionParserException
			 *     If this option needs a value.
			 */
			virtual void operator ()(Opt& options) = 0;

			/**
			 * Applies this option with a given value.
			 *
			 * @param options
			 *     Options container to which this option is to be applied.
			 * @param value
			 *     Value for this option.
			 * @return
			 *     Whether `value` is valid.
			 * @throws OptionParserException
			 *     If this option does not take a value.
			 */
			virtual bool operator ()(Opt& options, const String& value) = 0;
		};

		/** `Option` that takes a value. */
		class ValueOption : public Option {
		private:
			/** Name of the value. */
			String valueName;
		public:
			/**
			 * Initializes an option that takes a value with a given name.
			 *
			 * @param description
			 *     Description of the option.
			 * @param valueName
			 *     Name of the value.
			 */
			ValueOption(const String& description, const String& valueName)
				: Option(description), valueName(valueName) {}

			/** Takes a value; i.e., returns `true`. */
			virtual bool needsValue() const {
				return true;
			}

			/** Returns the name given at the construction. */
			virtual String getValueName() const {
				return this->valueName;
			}

			/** Needs a value; i.e., throws `OptionParserException`. */
			virtual void operator ()(Opt& option) {
				throw OptionParserException("value needed");
			}
		};

		/**
		 * `Option` that substitutes a member field.
		 *
		 * `T`: Type of an option value.
		 *
		 * `SupOpt`: Type of a container for option values.
		 *           Must be `Opt` or a super type of `Opt`.
		 *
		 * `Format`: Type of a formatter from a string to a value of `T`.
		 */
		template < typename T, typename SupOpt, typename Format >
		class MemberOption : public ValueOption {
		private:
			/** Field to be substituted. */
			T (SupOpt::*field);

			/** Formats a string into a value of `T`. */
			Format format;
		public:
			/**
			 * Initializes an option that substitutes a given field.
			 *
			 * @param description
			 *     Description of the option.
			 * @param field
			 *     Pointer to the field to be substituted.
			 * @param format
			 *     Formatter for the option value.
			 */
			MemberOption(const String& description,
						 T (SupOpt::*field),
						 const Format& format = Format())
				: ValueOption(description, format.getDefaultValueName()),
				  field(field), format(format) {}

			/**
			 * Applies this option with a given value.
			 *
			 * @param options
			 *     Options container to which this options is to be applied.
			 * @param value
			 *     Value for this option.
			 * @return
			 *     Whether `value` is valid.
			 *     `false` if `Format` cannot convert `label` to the intended
			 *     type.
			 */
			virtual bool operator ()(Opt& options, const String& value) {
				return this->format(value, options.*this->field);
			}
		};

		/**
		 * `Option` that substitutes a member field with a constant.
		 */
		template < typename T, typename SupOpt, typename Fmt >
		class ConstMemberOption : public Option {
		private:
			/** Field to be substituted. */
			T (SupOpt::*field);

			/** Constant value to substitute the field. */
			T constant;
		public:
			/**
			 * Initializes an option that substitutes a given field.
			 *
			 * @param description
			 *     Description of the option.
			 * @param field
			 *     Pointer to the field to be substituted.
			 * @param constant
			 *     Constant to substitute the field.
			 */
			ConstMemberOption(const String& description,
							  T (SupOpt::*field),
							  T constant)
				: Option(description), field(field), constant(constant) {}

			/** Does not take a value; i.e., returns `false`. */
			virtual bool needsValue() const {
				return false;
			}

			/** Does not take a value; i.e, returns an empty string. */
			virtual String getValueName() const {
				return String();
			}

			/** Applies this option without a value. */
			virtual void operator ()(Opt& options) {
				options.*this->field = this->constant;
			}

			/** Does not take a value; i.e., throws OptionParserException. */
			virtual bool operator ()(Opt& options, const String& value) {
				throw OptionParserException("no value needed");
			}
		};
	private:
		/** Type of an option map. */
		typedef std::map< String, Option* > OptionMap;

		/** Value type of `OptionMap`. */
		typedef typename OptionMap::value_type OptionMapValue;

		/** Iterator type of `OptionMap`. */
		typedef typename OptionMap::iterator OptionMapItr;

		/** Validator for this parser. */
		Validator< Opt, Ch > validator;

		/** Description of the program. */
		String description;

		/** Name of the program. Empty by default. */
		String programName;

		/** Maps an option label to the corresponding `Option`. */
		OptionMap optionMap;
	public:
		/** Releases resources. */
		virtual ~OptionParserBase() {
			// deletes options
			std::for_each(this->optionMap.begin(), this->optionMap.end(),
						  &releaseOptionMapValue);
		}

		/**
		 * Adds an option that substitutes a given field.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be replaced with a new option.
		 *
		 * `T`: type of an option value.
		 *
		 * `SupOpt`: Type of a container for option values.
		 *           Must be `Opt` or a super type of `Opt`.
		 *
		 * @param label
		 *     Option label on the command line.
		 *     Must start with a dash ('-'). Like "-o" or "--option".
		 * @param description
		 *     Description of the option.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @throws OptionParserException
		 *     If `label` does not start with a dash.
		 */
		template < typename T, typename SupOpt >
		void addOption(const String& label,
					   const String& description,
					   T (SupOpt::*field))
		{
			verifyLabel(label);
			Option* option = new MemberOption
				< T, SupOpt, MetaFormat< T, Ch > >(description, field);
			this->addOption(label, option);
		}

		/**
		 * Adds an option that substitutes a given field with a given constant.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be replaced with a new option.
		 *
		 * `T`: type of an option value.
		 *
		 * `SupOpt`: Type of a container for option values.
		 *           Must be `Opt` or a super type of `Opt`.
		 *
		 * @param label
		 *     Option label on the command line.
		 *     Must start with a dash ('-'). Like "-o" or "--option".
		 * @param description
		 *     Description of the option.
		 * @param field
		 *     Pointer to the field of `SupOpt` to be substituted.
		 * @param constant
		 *     Constant that substitutes the field.
		 * @throws OptionParserException
		 *     If `label` does not start with a dash.
		 */
		template < typename T, typename SupOpt >
		void addOption(const String& label,
					   const String& description,
					   T (SupOpt::*field),
					   T constant)
		{
			verifyLabel(label);
			Option* option =
				new ConstMemberOption< T, SupOpt, MetaFormat< T, Ch > >
					(description, field, constant);
			this->addOption(label, option);
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
		 */
		Opt parse(int argc, Ch** argv) {
			Opt options;
			// aborts if no arguments are specified
			if (argc <= 0) {
				return this->validator.tooFewArguments(options);
			}
			// updates the program name
			this->programName = argv[0];
			// processes rest of arguments
			int argI = 1;
			while (argI < argc) {
				// checks if `argv[argI]` is an option label
				if (isLabel(argv[argI])) {
					// processes an option
					const Ch* label = argv[argI];
					Option* option = this->findOption(label);
					if (option) {
						// processes a value if necessary
						if (option->needsValue()) {
							// applies the option value
							if (argI + 1 < argc) {
								++argI;
								const Ch* value = argv[argI];
								if (!(*option)(options, value)) {
									return this->validator
										.badValue(options, label, value);
								}
							} else {
								return this->validator
									.valueNeeded(options, label);
							}
						} else {
							// applies the option without a value
							(*option)(options);
						}
					} else {
						return this->validator.unknownOption(options, label);
					}
				} else {
					// TODO: processes `argv[argI]` as a positional argument
				}
				++argI;
			}
			return this->validator.validate(options);
		}

		/**
		 * Prints the usage.
		 *
		 * If you omit `programName`, the name given to the last `parse`
		 * function call will be used as the program name.
		 *
		 * `Printer`: Printer for the usage.
		 *
		 * @param programName
		 *     Name of the program. May be omitted.
		 */
		template < template < typename > class Printer >
		void printUsage(const Ch* programName = 0,
						Printer< Ch >&& printer = Printer< Ch >())
		{
			// sets the basic information
			printer.setDescription(this->description);
			printer.setProgramName(
				programName ? programName : this->programName);
			// adds options
			std::for_each(this->optionMap.begin(), this->optionMap.end(),
				[&] (const OptionMapValue& value) {
					const String& label = value.first;
					const Option* option = value.second;
					if (option->needsValue()) {
						printer.addOption(label,
										  option->getValueName(),
										  option->getDescription());
					} else {
						printer.addOption(label, option->getDescription());
					}
				});
			// prints the information
			printer.print();
		}
	protected:
		/**
		 * Adds a given option to this parser.
		 *
		 * If an option corresponding to `label` already exists in this parser,
		 * it will be deleted an replaced by `option`.
		 *
		 * NOTE: Never checks if `label` starts with a dash ('-').
		 *
		 * @param label
		 *     Option label on the command line.
		 * @param option
		 *     Pointer to the option to be added.
		 *     Must be allocated by the standard new operator.
		 */
		void addOption(const String& label, Option* option) {
			std::pair< OptionMapItr, bool > ins =
				this->optionMap.insert(OptionMapValue(label, option));
			if (!ins.second) {
				// replaces the older one
				releaseOptionMapValue(*ins.first);
				ins.first->second = option;
			}
		}

		/**
		 * Finds the option that has to a given label.
		 *
		 * @param label
		 *     Label of the option to be searched.
		 * @return
		 *     Option that has to `label`. 0 if no option has `label`.
		 */
		Option* findOption(const String& label) {
			OptionMapItr optionItr = this->optionMap.find(label);
			return optionItr != this->optionMap.end() ? optionItr->second : 0;
		}

		/**
		 * Returns whehter a given option label is valid.
		 *
		 * @param label
		 *     Label to be tested.
		 * @return
		 *     Whether `label` starts with a dash ('-').
		 */
		static inline bool isLabel(const String& label) {
			return !label.empty() && label[0] == Ch('-');
		}

		/**
		 * Checks whether a given option label is valid.
		 *
		 * @param label
		 *     Label to be tested.
		 * @throws OptionParserException
		 *     If `label` does not start with a dash ('-').
		 */
		static void verifyLabel(const String& label) {
			if (!isLabel(label)) {
				throw OptionParserException("invalid option label");
			}
		}

		/**
		 * Releases a value in the option map.
		 *
		 * Deletes `value.second`.
		 *
		 * @param value
		 *     Value in the option map to be released.
		 */
		static void releaseOptionMapValue(OptionMapValue& value) {
			delete value.second;
		}
	private:
	};

}

#endif
