# Dusk

## Documentation

[The documentation for dusk can be found here](https://kian_shepherd.gitlab.io/Dusk/)

## Description

A simple programming language built from scratch.

The build scripts use CMake and Ninja to find dependencies and build the project.

Uses flex as a tokenizer.

Uses bison as a parser generator.

The AST is generated with C++.

LLVM is used as a backend to generate machine specific object files which are then compiled with a c compiler into an executable.

## Dependencies

This project requires `flex`, `bison`, `llvm`, and a `c`/`c++` compiler.

The examples for building this project also include `ninja`, but that is an optional dependency.

## Installation

Build LLVM 

```
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cd build
cmake -G Ninja -DLLVM_ENABLE_ASSERTIONS=On -DCMAKE_BUILD_TYPE=Release ../llvm
ninja
ninja install
```

### Ubuntu:

Ensure you have flex and bison and build tools 

```
sudo apt install cmake ninja-build flex libfl-dev bison libbison-dev
```

### Mac OS

Ensure you have flex and bison and build tools

```
brew install bison flex cmake ninja
```

An old version of bison is the default version so you must relink a newer version to build Dusk

```
brew unlink bison
brew link bison --force
echo 'export PATH="/opt/homebrew/opt/bison/bin:$PATH"'>> ~/.zshrc
```

## Development

To generate a `compile_commands.json` file for use with `ccls` run

```
./ccls_file_gen.sh
```

## Building

To build the project run.

```
cmake -G Ninja -S . -B CMake
ninja -C CMake
```

To install the binary run.

```
ninja -C CMake install
```

## Usage

Once it is installed you can run `dusk x`, where x is the name of the file you wish to compile.

For example

```
dusk test.ds
```

Would compile a file named `test.ds` into an executable.

The help menu can be viewed like so.

```
$ dusk -h
Usage: dusk [options] file...
Options:
-h                Display this information.
-d                Print out debug information about the AST and LLVM IR.
-da               Print out debug information about the AST.
-di               Print out debug information about the LLVM IR.
-dc               Print out debug information about the compile commands.
-O                Optimize the generated object / executable.
-c                Only compile the sources do not link into executable.

-o <file>         Set the name of the outputted object / executable.
-cc <compiler>    Set the c / c++ compiler to use (default is g++).

-l<library>       Add library to link to executable at compile time.
-L<path>          Add path to library link paths.
-I<path>          Add path to include file paths.
```

## Current Functionality

- Base types
    - Atomic types
        - i1: boolean
        - i8: char   (equivalent to C char)
        - i64: int   (equivalent to C long)
        - i128: long (equivalent to C long long)
        - f64: float (equivalent to C double)
    - Array Types
        - boolean array
        - strings
        - int array
        - float array
        - string arrays
- Functions
    - `main` as entry point
        - accepts and handles command line arguments
    - Can be self recursive
        - Optimized so tail call omptimization can be preformed to remove recursive calls where possible
    - Can pass arguments to and from
    - Can be void
    - Variables are properly scoped to be function and block specific
- `extern` keyword to load `C` functions from libraries
- `import` and `require` keywords
    - Can import other `.ds` files without including them in the CLI
    - Can require `C` libraries at link time
- Static checking
    - Ensure `main` function is present and only one exists
    - Checks for non mutable vairables being mutated
- CLI for dusk executable
    - Can print help info on how to use it
- stdlib
    - Casting functions
    - `copy` functions to allocate on the heap
    - `del` function to delete heap allocated objects
    - `print` function, takes C style format strings
- Structs
    - Member fields
    - Heap allocated
    - Constructor overloading
    - Operator overloading

## Planned Functionality

- Structs
    - Overloading for all methods
    - Automatic Reference Counting
-  stdlib improvements
    - More abstract datatypes
    - More general functionality
        - IO
        - Math functions
        - String manipulation
        - ...
    - ...
- Better compile time failure error messages
    - show line that error occured on
- Tests
- Documentation webpage

## License

This project is licensed under GNU GPLv3, as this project depends on `bison`.

