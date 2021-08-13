cmake_minimum_required(VERSION 3.10)
project(zbytebuf_utests CXX)

include(../cmake/utils.cmake)

set(IMPORT_HEADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)
include_directories(${IMPORT_HEADERS_DIR})

set(ZBYTEBUF_UTESTS_SOURCES
        test_basic.cpp
        main.cpp)

enable_testing()

function(zbytebuf_prepare_test test_target)
    add_executable(${test_target} ${ZBYTEBUF_UTESTS_SOURCES})

    add_test(NAME ${test_target} COMMAND ${test_target})
    set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endfunction()

# The compiled library tests
if (ZBYTEBUF_TESTS OR ZBYTEBUF_BUILD_ALL)
zbytebuf_prepare_test(zbytebuf-utests esmobile::esmobile)
endif ()

