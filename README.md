# FileDiff

[![Lines of Code](https://tokei.rs/b1/github/ClayCore/fileDiff?category=code)](https://github.com/ClayCore/fileDiff)

## About
CLI and GUI drive file diff utility.

## Screenshots

<!-- TODO: Add screenshots -->

## Getting started

### Documentation

Documentation is present in the form of comments in the source code.
There are plans to add [Doxygen](https://www.doxygen.nl/index.html) but the project is very early in development and the API (if you can even call it that) changes daily.

### Prerequisites

#### Dependencies:
GCC, Clang or MSVC/VS
: For compiling

CMake version >=3.21
: for building the project

Python >=3.11.5
: optional, shorthand build scripts and tasks in `/.vscode/tasks.json`

#### Platforms:
Currently only Windows and Linux is supported.

#### Build:
GCC, Clang and MSVC are *technically* supported, however the project's only been tested with GCC and Clang.

CMake is the primary tool used to build this project. You can configure and build the project like so:
```sh
$ cmake -S . -B build
$ cmake --build build
```
This will build the project using default settings and it will assume/detect many options. You can customize it to your liking. Check out the various modules in `/cmake/` folder for more.

You can also use the following python scripts present in the `/scripts/` directory.
 - [configure.debug.py](/scripts/configure.debug.py): Configure the project using the `Debug` configuration,
 - [configure.release.py](/scripts/configure.release.py): Configure the project using the `Release` configuration,
 - [build.debug.py](/scripts/build.debug.py): Builds the project using the `Debug` configuration,
 - [build.release.py](/scripts/build.release.py): Builds the project using the `Release` configuration,

#### Testing:
For unit tests `CTest` is used. Simply run the following to run all tests:
```sh
$ ctest --test-dir build
```

## License

This project is under the GPL-3.0 license.

## Acknowledgements

 - [cmake_template.git](https://github.com/cpp-best-practices/cmake_template) for CMake build project template
 - [GetGitRevisionDescription](https://github.com/rpavlik/cmake-modules/tree/main) for fetching git revision in CMake
 - [CHEAT](https://github.com/Tuplanolla/cheat/tree/master) for the C Unit testing framework
