cmake_minimum_required(VERSION 3.14)
project(cu_submitter)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_SOURCES
        src/cu_submitter.cpp src/chgen/chgen.cpp src/chgen/chgen.h)

add_executable(cu_submitter
        ${PROJECT_SOURCES})