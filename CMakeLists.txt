# cmake_minimum_required(VERSION <specify CMake version here>)
project(cu_submitter)

set(CMAKE_CXX_STANDARD 14)

add_executable(cu_submitter
        src/cu_submitter.cc src/chgen/chgen.cpp src/chgen/chgen.h)
