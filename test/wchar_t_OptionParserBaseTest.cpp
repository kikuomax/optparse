#include <string>

#define PREFIX(name)  wchar_t_ ## name
typedef wchar_t Ch;
typedef std::wstring String;
#define STR(str) L ## str

#include "OptionParserBaseTest.h"
