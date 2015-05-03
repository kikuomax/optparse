cmake_minimum_required (VERSION 2.8.4)

project ("optparse")

set (PROJECT_MAJOR 0)
set (PROJECT_MINOR 1)
set (PROJECT_PATCH 0-SNAPSHOT)
set (PROJECT_VERSION "${PROJECT_MAJOR}.${PROJECT_MINOR}.${PROJECT_PATCH}")

# enables c++11
if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

configure_file (src/OptionParserVersion.h.in src/OptionParserVersion.h)

include_directories (
	src
	${PROJECT_BINARY_DIR}/src)

add_executable (optparse_test
	src/test.cpp)
