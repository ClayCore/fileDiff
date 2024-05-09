# FileDiff

## About

[![Lines of Code](https://tokei.rs/b1/github/ClayCore/fileDiff?category=code)](https://github.com/ClayCore/fileDiff)

CLI and GUI drive file diff utility.

## Screenshots

<!-- TODO: Add screenshots -->

## Getting started

### Documentation

Documentation is present in the form of comments in the source code.
There are plans to add [Doxygen](https://www.doxygen.nl/index.html) but the project is very early in development and the API (if you can even call it that) changes daily.

### Prerequisites

#### Build:
GCC, Clang and MSVC are *technically* supported, however the project's only been tested with GCC and Clang.

CMake is the primary tool used to build this project. You can configure and build the project like so:
```sh
$ cmake -S . -B build
$ cmake --build build
```
This will build the project using default settings and it will assume/detect many options. You can customize it to your liking. Check out the various modules in `/cmake/` folder for more.

You can also use the batch scripts present in the `/scripts/` directory.
 - [configure.debug.cmd](/scripts/configure.debug.cmd): Configure the project using the `Debug` configuration,
 - [configure.release.cmd](/scripts/configure.release.cmd): Configure the project using the `Release` configuration,
 - [build.debug.cmd](/scripts/build.debug.cmd): Builds the project using the `Debug` configuration,
 - [build.release.cmd](/scripts/build.release.cmd): Builds the project using the `Release` configuration,

#### Platforms:
Currently only Windows and Linux is supported.

## License

This project is under the GPL-3.0 license.

## Acknowledgements

 - [cmake_template.git](https://github.com/cpp-best-practices/cmake_template) for CMake build project template
 - [GetGitRevisionDescription](https://github.com/rpavlik/cmake-modules/tree/main) for fetching git revision in CMake
 - [CHEAT](https://github.com/Tuplanolla/cheat/tree/master) for the C Unit testing framework
