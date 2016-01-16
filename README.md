*optparse* is a simple command line option parser for a C++ program.

There are already a lot of libraries you can choose for parsing command line options.
To find a suitable library for you, [this conversation](http://stackoverflow.com/questions/253556/what-parameter-parser-libraries-are-there-for-c) could help you.

The main motivation of this reinvention is **separation of a container for option values from a parser.**
*optparse* will keep your option values container clean and make it easy to extend an existing option parser by inheritance.

Prerequisites
-------------

You need the following software installed,
 - [Git](https://git-scm.com)
 - [CMake](https://cmake.org) 2.8.4 or higher
 - C++ compiler

Getting Started
---------------

*optparse* consists of only header files.
The [CMake script](/CMakeLists.txt) does everything you need to install *optparse*.

Please take the following steps,

 1. Clone the repository anywhere you like and move down to it.

	```shell
	git clone https://github.com/kikuomax/optparse.git
	cd optparse
	```

 2. Create a `build` directory and move down to it.

	```shell
	mkdir build
	cd build
	```

 3. Configure the project.
    Suppose you want to install *optparse* into the directory `install-path`.

	```shell
	cmake -DCMAKE_INSTALL_PREFIX=install-path ..
	```

 4. Install header files.

	```shell
	cmake --build . --target install
	```

 5. You will find the header files in the following directory,
    - `install-path/include/optparse`

Creation of the `build` directory (step 2) is not necessary, but it prevents this directory being messy.

Examples
--------

The installation step (step 4 in [Getting Started](#getting_started)) also will compile examples.
You will find the following programs in the build directory,
 - `example`

   Example using `char` as character type.

 - `wexample`

   Example using `wchar_t` as character type.

Running Tests
-------------

If you have [googletest](https://github.com/google/googletest) installed, unittests for *optparse* will also be built at the installation step (step 4 in [Getting Started](#getting_started)).
You can run the unittests by running the following command in the build directory,

```shell
ctest -V
```

The generation of the unittests can be suppressed by turning off the `GENERATE_TESTS` option at the configuration step.

Generating Documentation
------------------------

If you have [Doxygen](http://www.stack.nl/~dimitri/doxygen/) installed, documentation of *optparse* will also be generated at the installation step (step 4 in [Getting Started](#getting_started)).
You will find the `api` directory in the build directory.
The generation of the documentation can be suppressed by turning off the `GENERATE_DOCUMENTATION` option at the configuration step.

License
-------

[MIT License](https://opensource.org/licenses/MIT).
