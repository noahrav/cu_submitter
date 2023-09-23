cmake_minimum_required(VERSION 3.14)
project(cu_submitter)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCES
        src/cu_submitter.cpp
        src/chgen/chgen.cpp
        src/chgen/chgen.h
        src/data/changelog.cpp
        src/data/changelog.h)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt5")

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(liblcf REQUIRED)

add_executable(cu_submitter
        ${PROJECT_SOURCES})

target_link_libraries(cu_submitter
        Qt5::Widgets)

target_link_libraries(cu_submitter
        lcf)