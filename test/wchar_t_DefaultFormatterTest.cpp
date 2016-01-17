#include <string>

typedef wchar_t Ch;
typedef std::wstring String;
#define STR(str)  L ## str
#define PREFIX(name)  wchar_t_ ## name

#include "DefaultFormatterTest.h"
