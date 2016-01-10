#ifndef _OPTPARSE_OPTPARSE_OPTION_PARSER_EXCEPTION_H
#define _OPTPARSE_OPTPARSE_OPTION_PARSER_EXCEPTION_H

#include <string>

namespace optparse {

	/** Base class of an exception thrown by `optparse`. */
	class Exception {
	private:
		/** Brief explanation about this exception. */
		std::string message;
	protected:
		/**
		 * Initializes with a brief explanation.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 */
		inline explicit Exception(const std::string& message)
			: message(message) {}
	public:
		/**
		 * Returns the brief explanation about this exception.
		 *
		 * @return
		 *     Brief explanation about this exception.
		 */
		inline const std::string& getMessage() const {
			return this->message;
		}
	};

	/** Exception thrown when a configuration is invalid. */
	class ConfigException : Exception {
	public:
		/**
		 * Initializes with a brief explanation.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 */
		inline explicit ConfigException(const std::string& message)
			: Exception(message) {}
	};

	/** Exception thrown when parsing has failed. */
	class ParsingException : public Exception {
	protected:
		/**
		 * Initializes with a brief explanation.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 */
		inline explicit ParsingException(const std::string& message)
			: Exception(message) {}
	};

	/** Exception thrown when too few arguments have been given. */
	class TooFewArguments : public ParsingException {
	public:
		/** Initializes with a default explanation "too few arguments". */
		inline TooFewArguments() : ParsingException("too few arguments") {}
	};

	/** Exception thrown when too many arguments have been given. */
	class TooManyArguments : public ParsingException {
	public:
		/** Initializes with a default explanation "too many arguments". */
		inline TooManyArguments() : ParsingException("too many arguments") {}
	};

	/**
	 * Exception thrown when parsing of an option fails.
	 *
	 * @tparam Ch
	 *     Type which represents an input character.
	 *     See `OptionParserBase`.
	 */
	template < typename Ch >
	class OptionParsingException : public ParsingException {
	protected:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;

		/** Label or name of the option which has an error. */
		String label;
	protected:
		/**
		 * Initializes with a brief explanation and an option label.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 * @param label
		 *     Label or name of the option which has an error.
		 */
		inline explicit OptionParsingException(
			const std::string& message, const String& label)
			: ParsingException(message), label(label) {}
	public:
		/** Returns the label or name of the option. */
		inline const String& getLabel() const {
			return this->label;
		}
	};

	/**
	 * Exception thrown when no value is given to an option which needs a value.
	 *
	 * @tparam Ch
	 *     See `OptionParsingException`.
	 */
	template < typename Ch >
	class ValueNeeded : public OptionParsingException< Ch > {
	private:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;
	public:
		/**
		 * Initializes with an option label.
		 *
		 * @param label
		 *     Label of the option which has an error.
		 */
		inline explicit ValueNeeded(const String& label)
			: OptionParsingException< Ch >("needs value", label) {}
	};

	/**
	 * Exception thrown when an invalid value is given to an option.
	 *
	 * @tparam Ch
	 *     See `OptionParsingException`.
	 */
	template < typename Ch >
	class BadValue : public OptionParsingException< Ch > {
	private:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;

		/** Invalid value given to the option. */
		String value;
	public:
		/**
		 * Initializes with a brief explanation, an option label and value.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 * @param label
		 *     Label of the option which has an error.
		 * @param value
		 *     Invalid value given to the option.
		 */
		inline BadValue(const std::string& message,
						const String& label,
						const String& value)
			: OptionParsingException< Ch >(message, label), value(value) {}

		/**
		 * Initializes with a brief explanation and an option value.
		 *
		 * @param message
		 *     Brief explanation about the exception.
		 * @param value
		 *     Invalid value given to the option.
		 */
		inline BadValue(const std::string& message, const String& value)
			: OptionParsingException< Ch >(message, String()), value(value) {}

		/** Returns the invalid value given to the option. */
		inline const String& getValue() const {
			return this->value;
		}
	};

	/**
	 * Exception thrown when an unknown option is specified.
	 *
	 * @tparam Ch
	 *     See `OptionParsingException`.
	 */
	template < typename Ch >
	class UnknownOption : public OptionParsingException< Ch > {
	private:
		/** String of `Ch`. */
		typedef std::basic_string< Ch > String;
	public:
		/**
		 * Initializes with an option label.
		 *
		 * @param label
		 *     Label of the unknown option.
		 */
		inline explicit UnknownOption(const String& label)
			: OptionParsingException< Ch >("unknown option", label) {}
	};

	/** Exception thrown when a help is needed. */
	class HelpNeeded : public ParsingException {
	public:
		/** Initializes with the default message "help needed". */
		inline HelpNeeded() : ParsingException("help needed") {}
	};

}

#endif
