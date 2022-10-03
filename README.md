# Dusk

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)


## Description

A simple programming language built from scratch.

Uses flex as a tokenizer.

Uses bison as a parser generator.

## Installation

### Ubuntu:

Ensure you have flex and bison

`sudo apt install cmake flex libfl-dev bison libbison-dev`

### Mac OS

Ensure you have flex and bison

`brew install bison flex cmake`

`brew unlink bison`

`brew link bison --force`

`echo 'export PATH="/opt/homebrew/opt/bison/bin:$PATH"'>> ~/.zshrc`

## Building

Then run the build script

`./build.sh`

To run the binary

`./dusk`

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## License
For open source projects, say how it is licensed.

