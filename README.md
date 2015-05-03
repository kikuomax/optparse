*optparse* is a simple command line option parser for a C++ program.

There are already a lot of options you can choose for parsing command line options.
 - boost: if you are using boost, this should be the best choice for you.

Motivation of this reinvention.
 - No dependency on non-standard libraries
 - Type-safety
 - Separation of a container from a parser
 - Reusability by inheritance
 - Friendliness to CMake
