# Dusk

## Description

A simple programming language built from scratch.

The build scripts use CMake and Ninja to find dependencies and build the project.

Uses flex as a tokenizer.

Uses bison as a parser generator.

The AST is generated with C++

LLVM is used as a backend to generate machine specific object files which can be compiled with a c compiler into executables.

## Dependencies

This project requires `cmake`, `flex`, `bison`, `llvm` and a `c`/`c++` compiler.

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

## License

For open source projects, say how it is licensed.

