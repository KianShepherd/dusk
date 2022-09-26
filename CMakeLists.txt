cmake_minimum_required(VERSION 3.10)

project(Dusk VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/dusk.cc
    src/parser.cc
    src/scanner.cc
    src/ast.cc
    src/expression.cc
    src/statement.cc
    src/function.cc
)

set(HEADER_FILES
    include/ast.hh
    include/expression.hh
    include/function.hh
    include/parser.hh
    include/statement.hh
)

add_executable(Dusk ${SOURCES} ${HEADER_FILES})
