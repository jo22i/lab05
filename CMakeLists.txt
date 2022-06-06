cmake_minimum_required(VERSION 3.22)

SET(COVERAGE OFF CACHE BOOL "Coverage")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(GTest_tests)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest" "gtest")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_executable(RunExec ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

if (COVERAGE)
    target_compile_options(RunExec PRIVATE --coverage)
    target_link_libraries(RunExec PRIVATE --coverage)
endif()

target_include_directories(RunExec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/banking)

target_link_libraries(RunExec PRIVATE gtest gtest_main gmock_main banking)
