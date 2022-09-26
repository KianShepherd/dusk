cmake_minimum_required(VERSION 3.10)

project(Dusk VERSION 0.1)

find_package(FLEX)
find_package(BISON)

FLEX_TARGET(Scanner ${CMAKE_SOURCE_DIR}/src/tokens.l  ${CMAKE_SOURCE_DIR}/src/scanner.cc)
BISON_TARGET(Parser ${CMAKE_SOURCE_DIR}/src/grammar.y ${CMAKE_SOURCE_DIR}/src/parser.cc)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/dusk.cc
    src/scanner.cc
    src/ast.cc
    src/expression.cc
    src/statement.cc
    src/function.cc
)

include_directories(include)
set(HEADER_FILES
    include/ast.hh
    include/expression.hh
    include/function.hh
    include/statement.hh
)

add_executable(Dusk ${SOURCES} ${HEADER_FILES} ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})
