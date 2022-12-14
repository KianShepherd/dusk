cmake_minimum_required(VERSION 3.13)

project(dusk VERSION 0.1)

find_package(FLEX)
find_package(BISON)
add_compile_options(-Wall)
add_compile_options(-O3)

add_custom_command(
  OUTPUT src/scanner.cc src/parser.cc include/parser.hh
  COMMAND ../gen_parser.py
  VERBATIM
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message(STATUS "LLVM INCLUDE ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
message(STATUS "LLVM DEFINITIONS ${LLVM_PACKAGE_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-rtti")

set(SOURCES
    src/dusk.cc
    src/scanner.cc
    src/ast.cc
    src/expression.cc
    src/statement.cc
    src/function.cc
    src/helpers.cc
    src/struct.cc
    src/parser.cc
    src/scanner.cc
)

link_directories(/usr/local/lib)
include_directories(/usr/local/include/gmp)
if(APPLE)
    include_directories(/usr/local/include/llvm)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

include_directories(include)
set(HEADER_FILES
    include/ast.hh
    include/expression.hh
    include/function.hh
    include/statement.hh
    include/helpers.hh
    include/struct.hh
    include/parser.hh
)

add_library(stddusk stdlib/stdlib.cc)
add_library(duskvec stdlib/duskvec.cc)
add_library(duskio stdlib/duskio.cc)
add_library(duskmath stdlib/duskmath.cc)
add_library(bignum stdlib/bignum.cc)

add_executable(dusk ${SOURCES} ${HEADER_FILES} ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})

llvm_map_components_to_libnames(all)

execute_process(COMMAND llvm-config --system-libs --libs all
                OUTPUT_VARIABLE llvm_libraries)
string(STRIP ${llvm_libraries} llvm_libraries)
string(REPLACE "\n" " " llvm_libraries ${llvm_libraries})

# Link against LLVM libraries
target_link_libraries(dusk ${llvm_libraries})

install(TARGETS stddusk dusk
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
