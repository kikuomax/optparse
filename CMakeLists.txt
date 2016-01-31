cmake_minimum_required (VERSION 2.8.4)

project ("optparse")

set (PROJECT_MAJOR 0)
set (PROJECT_MINOR 1)
set (PROJECT_PATCH 0-SNAPSHOT)
set (PROJECT_VERSION "${PROJECT_MAJOR}.${PROJECT_MINOR}.${PROJECT_PATCH}")

# turn off if you want to dynamically link the MSVC runtime
option (MSVC_LINK_STATIC_RUNTIME "Statically link the MSVC runtime" on)

# replaces /MD with /MT if the static MSVC runtime is needed
if (MSVC AND MSVC_LINK_STATIC_RUNTIME)
	foreach (flag_var
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if (${flag_var} MATCHES "/MD")
			string (REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif ()
	endforeach ()
	# finds googletest built for the static runtime
	set (GTEST_MSVC_SEARCH "MT")
endif ()

# sets the warning level to the maximum
if (MSVC)
	foreach (flag_var
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if (${flag_var} MATCHES "/W[0-3]")
			string (REGEX REPLACE "/W[0-3]" "/W4" ${flag_var} "${${flag_var}}")
		else ()
			set (${flag_var} "${${flag_var}} /W4")
		endif ()
	endforeach ()
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-long-long")
endif ()

# enables c++11 on GNU compilers
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

# finds Doxygen
find_package (Doxygen)

# turn on if you want to generate documentation
# turned on by default as long as Doxygen is found
option (GENERATE_DOCUMENTATION "Generate Doxygen documentation" ${DOXYGEN_FOUND})

# finds GTest
find_package (GTest)

# turn on if you want to generate tests
# turned on by default as long as googletest is found
option (GENERATE_TESTS "Generate tests" ${GTEST_FOUND})

configure_file (src/optparse/optparse.h.in src/optparse/optparse.h @ONLY)

include_directories (
	src
	example
	test
	${PROJECT_BINARY_DIR}/src)

add_executable (example example/example.cpp)

add_executable (wexample example/example.cpp)

set_target_properties (wexample
	PROPERTIES COMPILE_FLAGS "-DWCHAR_EXAMPLE")

# generates tests if necessary
if (GENERATE_TESTS)
	enable_testing ()
	# builds a test binary
	include_directories (${GTEST_INCLUDE_DIRS})
	add_executable (optparse-test
		test/char_DefaultFormatterTest.cpp
		test/wchar_t_DefaultFormatterTest.cpp
		test/char_OptionParserBaseTest.cpp
		test/wchar_t_OptionParserBaseTest.cpp)
	# old Visual Studio needs a tweak
	if (MSVC AND MSVC_VERSION LESS 1800)
		set_target_properties (optparse-test
			PROPERTIES COMPILE_FLAGS "-D_VARIADIC_MAX=10")
	endif ()
	target_link_libraries (optparse-test ${GTEST_BOTH_LIBRARIES})
	add_test (optparse-test optparse-test)
endif ()

# generates documentation if necessary
if (GENERATE_DOCUMENTATION)
	# configure Doxygen
	configure_file (Doxyfile.in Doxyfile @ONLY)
	# runs Doxygen
	add_custom_target (apis ALL
		${DOXYGEN_EXECUTABLE} Doxyfile
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		COMMENT "Generating Doxygen documentation")
	# installs the docs
	install (DIRECTORY ${PROJECT_BINARY_DIR}/apis/html
		DESTINATION apis/optparse)
endif ()

# installs headers
install (FILES
	src/optparse/DefaultFormatter.h
	src/optparse/DefaultUsagePrinter.h
	src/optparse/OptionParserBase.h
	src/optparse/OptionParserException.h
	src/optparse/OptionSpec.h
	${PROJECT_BINARY_DIR}/src/optparse/optparse.h
	DESTINATION include/optparse)
