# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.10)

# ---------------------------------------------------------------------------------------
# Start spdlog project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

zbytebuf_extract_version()

project(zbytebuf VERSION ${ZBYTEBUF_VERSION} LANGUAGES CXX)
message(STATUS "Build zbytebuf: ${ZBYTEBUF_VERSION}")


# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)


option(ZBYTEBUF_BUILD_ALL "Build all artifacts" ON)

# example options
option(ZBYTEBUF_BUILD_EXAMPLE "Build example" OFF)

# testing options
option(ZBYTEBUF_BUILD_TESTS "Build tests" ON)

# clang-tidy
if(${CMAKE_VERSION} VERSION_GREATER "3.5")
    option(ZBYTEBUF_TIDY "run clang-tidy" OFF)
endif()

if(ZBYTEBUF_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Enabled clang-tidy")
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
# if (ESMOBILE_BUILD_EXAMPLE OR ESMOBILE_BUILD_ALL)
#     message(STATUS "Generating example(s)")
#     add_subdirectory(example)
# endif ()

 if(ZBYTEBUF_BUILD_TESTS OR ZBYTEBUF_BUILD_ALL)
     message(STATUS "Generating tests")
     enable_testing()
     add_subdirectory(tests)
 endif()

