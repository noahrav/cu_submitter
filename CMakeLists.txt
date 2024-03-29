cmake_minimum_required(VERSION 3.14)
project(cu_submitter)

add_subdirectory(deps)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCES
        src/api/api.cpp src/api/api.h
        src/cu_submitter.cpp
        src/chgen/chgen.cpp src/chgen/chgen.h
        src/data/changelog.cpp src/data/changelog.h 
        src/utils/error.cpp src/utils/error.h 
        src/utils/log.cpp src/utils/log.h
        src/utils/print.cpp src/utils/print.h
        src/transfer/transfer.cpp src/transfer/transfer.h
        src/utils/utils.cpp src/utils/utils.h
        src/submit/submit.cpp src/submit/submit.h
)

include_directories(${CMAKE_BINARY_DIR}/_deps/rapidjson-src/include)

add_executable(cu_submitter
        ${PROJECT_SOURCES}
)

target_link_libraries(cu_submitter
        lcf
        pistache
)
