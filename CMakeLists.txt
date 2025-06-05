cmake_minimum_required(VERSION 3.15)
project(simple_OurScheme CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 23)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

# TODO: add lib

add_executable(OurScheme src/final_ver.cpp)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
    add_subdirectory(tests)
endif ()
