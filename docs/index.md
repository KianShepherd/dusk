# Dusk Language Reference

Dusk is a `C` like toy programming language, that was created as a personal project for fun. It will
likely never be stable / feature rich enough to used. But I would like to get it to the point where
it is functional enough to solve various programming problems with. I like to participate in
[Advent of Code](https://adventofcode.com/) each year and my goal is to complete as much of it as
possible with Dusk.

## Dependencies

Dusk depends on multiple third-party libraries to compile and generate optimized machine code, without
the help of these libraries Dusk would not be as performant and feature rich as it currently is as I
am only able to develop it on my free time.

Here is a list of the projects that Dusk depends on
- [LLVM](https://llvm.org/) is used to generate the final optimized executables / object files
- [Flex](https://github.com/westes/flex) is used as a tokenizer for the parser
- [Bison](https://www.gnu.org/software/bison/) is used as the parser generator
- [CMake](https://cmake.org/) is the build system that Dusk uses
- [Ninja](https://ninja-build.org/) (Optional, but I use it in the build scripts and highly reccomend it)
- [GNU Multiprecision Arithmetic](https://gmplib.org/) (Used in part of the standard library to support optimized large numbers)
