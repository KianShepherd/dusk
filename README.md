# Dusk

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)


## Description

A simple programming language built from scratch.

Uses flex as a tokenizer.

Uses bison as a parser generator.

The AST is generated with C++

LLVM is used as a backend

## Installation

### Ubuntu:

Ensure you have flex and bison

`sudo apt install cmake ninja-build flex libfl-dev bison libbison-dev`

Build LLVM 

`git clone https://github.com/llvm/llvm-project.git`

`cd llvm-project`

`mkdir build`

`cd build`

`cmake -G Ninja -DLLVM_ENABLE_ASSERTIONS=On -DCMAKE_BUILD_TYPE=Release ../llvm`

`ninja`

`ninja install`

### Mac OS

Ensure you have flex and bison

`brew install bison flex cmake`

An old version of bison is the default version so you must relink a newer version to build Dusk

`brew unlink bison`

`brew link bison --force`

`echo 'export PATH="/opt/homebrew/opt/bison/bin:$PATH"'>> ~/.zshrc`

## Building

To generate a `compile_commands.json` file for use with `ccls` run

`./ccls_file_gen.sh`

Then run the build script

`./build.sh`

To run the binary

`./dusk`

## Usage

To use Dusk simply run the built executable and pass in the name of the file you would like to compile.

`./dusk test.ds`

## License
For open source projects, say how it is licensed.

