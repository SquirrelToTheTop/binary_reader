cmake_minimum_required(VERSION 3.10)
project(F90Reader)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Compiler path : ${CMAKE_CXX_COMPILER}")

# add some use-full compile option
add_compile_options(-g -O3 -march=native -std=c++17 -Wno-unused-function )

# include slicer
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)

add_executable( F90Reader.exec main_test.cpp )
target_link_libraries( F90Reader.exec PUBLIC binaryReader )