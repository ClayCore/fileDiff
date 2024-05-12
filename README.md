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

<dl>
  <dt>GCC, clang or MSVC/VS</dt>
  <dd>for compilation</dd>
  <dt>CMake version >= 3.21</dt>
  <dd>for building and testing all targets</dd>
  <dt>Python >= 3.11.5</dt>
  <dd>optional, utility scripts and tasks in `/.vscode/tasks.json`</dd>
</dl>

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

You can also use the following batch scripts present in the `/scripts/` directory.
Note that these call into a python script, that in turn configures/builds the project with the `Ninja` generator
 - [configure.debug.cmd](/scripts/configure.debug.cmd): Configure the project using the `Debug` configuration,
 - [configure.release.cmd](/scripts/configure.release.cmd): Configure the project using the `Release` configuration,
 - [build.debug.cmd](/scripts/build.debug.cmd): Builds the project using the `Debug` configuration,
 - [build.release.cmd](/scripts/build.release.cmd): Builds the project using the `Release` configuration,

#### Testing:
For unit tests `CTest` is used. Simply run the following to run all tests:
```sh
$ ctest --test-dir build
```

You can also use the batch scripts below
 - [test.cli.cmd]('/scripts/test.cli.cmd'): Runs `CTest` against the CLI testing suite,
 - [test.gui.cmd]('/scripts/test.gui.cmd'): Runs `CTest` against the GUI testing suite,
 - [test.lib.cmd]('/scripts/test.lib.cmd'): Runs `CTest` against the common library,

## License

This project is under the GPL-3.0 license.

## Acknowledgements

 - [cmake_template.git](https://github.com/cpp-best-practices/cmake_template) for CMake build project template
 - [GetGitRevisionDescription](https://github.com/rpavlik/cmake-modules/tree/main) for fetching git revision in CMake
