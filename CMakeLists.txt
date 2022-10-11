cmake_minimum_required(VERSION 3.13)

project(Dusk VERSION 0.1)

find_package(FLEX)
find_package(BISON)
add_compile_options(-Wall)

FLEX_TARGET(Scanner ${CMAKE_SOURCE_DIR}/src/tokens.l  ${CMAKE_SOURCE_DIR}/src/scanner.cc)
BISON_TARGET(Parser ${CMAKE_SOURCE_DIR}/src/grammar.y ${CMAKE_SOURCE_DIR}/src/parser.cc)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-rtti -fno-exceptions")

set(SOURCES
    src/dusk.cc
    src/scanner.cc
    src/ast.cc
    src/expression.cc
    src/statement.cc
    src/function.cc
    src/helpers.cc
)

include_directories(include)
set(HEADER_FILES
    include/ast.hh
    include/expression.hh
    include/function.hh
    include/statement.hh
    include/helpers.hh
)

add_library(dusk SHARED src/stdlib.cc)
add_executable(Dusk ${SOURCES} ${HEADER_FILES} ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})

llvm_map_components_to_libnames(all)

execute_process(COMMAND llvm-config --system-libs --libs all
                OUTPUT_VARIABLE llvm_libraries)
string(STRIP ${llvm_libraries} llvm_libraries)
string(REPLACE "\n" " " llvm_libraries ${llvm_libraries})

# Link against LLVM libraries
target_link_libraries(Dusk ${llvm_libraries})
