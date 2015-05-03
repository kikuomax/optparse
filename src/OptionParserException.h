#ifndef _OPT_PARSE_OPTION_PARSER_EXCEPTION_H
#define _OPT_PARSE_OPTION_PARSER_EXCEPTION_H

#include <string>

namespace optparse {

	/** Exception thrown when an error has occurred. */
	class OptionParserException {
	private:
		/** Short description of the error. */
		std::string message;
	public:
		/**
		 * Initializes with a short description.
		 *
		 * @param message
		 *     Short description of the error.
		 */
		inline OptionParserException(const std::string& message)
			: message(message) {}

		/**
		 * Returns the short description of the error.
		 *
		 * @return
		 *     Short description of the error.
		 */
		inline const std::string& getMessage() const {
			return this->message;
		}
	};

}

#endif
