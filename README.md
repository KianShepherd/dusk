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

This project requires `flex`, `bison`, `llvm`, `GNU GMP`, and a `c`/`c++` compiler.

The examples for building this project also include `ninja`, but that is an optional dependency.

## Installation

Clone the repository, and run `deps.sh` to automatially install LLVM locally to this project.

```
git clone --recurse-submodules https://gitlab.com/kian_shepherd/Dusk.git
cd Dusk
./deps.sh
```

To use the `bignum.ds` library you also need the [GNU Multiple Precision Arithmetic Library](https://gmplib.org/)
You can find the [most recent version here](https://gmplib.org/#DOWNLOAD).

```
./configure --disable-shared --enable-fat --enable-cxx
make
```

 Run the tests before installing

 ```
make check
 ```

Finally install GMP

```
make install
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

To build the project run or simply run `build.sh`.

```
cmake -G Ninja -S . -B CMake
ninja -C CMake

or

./build.sh
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
-d                Print out debug information about the final AST and LLVM IR.
-df               Print out debug information about the AST at each stage and LLVM IR.
-da               Print out debug information about the final AST.
-daf              Print out debug information about the AST at each stage.
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
        - String struct for easier managing of string like objects
        - `bignum.ds` also provides these for handling of float and integer numbers of unbounded size
            - BigInt
            - BigFloat
        - `vector.ds` also provides these for handling of various vector types.
            - Vec<int>
            - Vec<float>
            - Vec<T> template type for other Structs
        - `linkedlists.ds` also provides these for handling of various linked list types
            - LinkedList<int>
            - LinkedList<T> template type for other Structs
    - Array Types
        - boolean array
        - strings
        - int array
        - float array
        - string arrays (array of char's)
    - Vector Types under `vector.ds`
        - String
        - int
        - float
    - User Defined Structs
        - Member fields
        - Heap allocated
        - Constructor overloading
        - Operator overloading
        - Method Overloading
        - Automatic Reference Counting
        - Destructors
        - `template` keyword
            - Template Structs can be created that are generic over Struct types.
            - Supports Templated member fields
            - Supports Templated methods
                - Can also override specific templated typed functions
- Functions
    - `main` as entry point
        - accepts and handles command line arguments
    - Can be self recursive
        - Optimized so tail call omptimization can be preformed to remove recursive calls where possible
    - Can pass arguments to and from
    - Can be void
    - Variables are properly scoped to be function and block specific
- `extern` keyword to load `C` functions from libraries (included them in the compilation using `requires` or `-l` on the command line)
- `import` and `require` keywords
    - Can import other `.ds` files without including them in the CLI
    - Can require `C` libraries at link time
- Static checking
    - Ensure `main` function is present and only one exists
    - Checks for non mutable vairables being mutated
- CLI for dusk executable
    - Can print help info on how to use it
- Standard Library
    - Base standard library functions
        - Casting functions
        - `copy` functions to allocate on the heap
        - `del` function to delete heap allocated objects
    - `types.ds` (Part of the base standard library)
        - `String` struct for better string datatypes support
    - `stdio.ds` (Part of the base standard library)
        - `print` function, takes C style format strings
        - `input` function
        - `readfile` function
    - `bignum.ds`
        - `BigInt` struct for integer numbers of unbounded size
        - `BigFloat` struct for floating point numbers of unbounded size
    - `vector.ds`
        - Various `Vec` types.
            - `Vec<int>`
            - `Vec<float>`
            - `Vec<String>`
    - `math.ds`
        - `pow` for int types
        - `fpow` for floating power

## Planned Functionality

- Improve Templates
    - Add supprt for multiple templated types
 - Improve Automatic Reference Counting
    - There are currently a few edge cases around calls to `__INCREF__` and `__DECREF__` that aren't currently implemented.
        - As I continue to add test caases and catch these more will be fixed.
- `stdlib` improvements
    - More abstract datatypes
        - LinkedList
            - General Improvements on functionality
        - Map
        - Set
    - More general functionality
        - IO
            - `writefile`
        - Math functions
        - String manipulation
        - ...
    - ...
- `__iter__` method function for structs for looping `for (v: vec) { ... }`
- Better compile time failure error messages
    - show line that error occured on

## License

This project is licensed under GNU GPLv3, as this project depends on `bison`.

