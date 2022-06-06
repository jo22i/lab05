cmake_minimum_required(VERSION 3.22)

SET(COVERAGE OFF CACHE BOOL "Coverage")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(GTest_tests)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest" "gtest")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_executable(${CMAKE_CURRENT_SOURCE_DIR}/GTests.cpp)

if (COVERAGE)
    target_compile_options(GTest_tests PRIVATE --coverage)
    target_link_libraries(GTest_tests PRIVATE --coverage)
endif()

target_include_directories(GTest_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/banking)

target_include_libraries(GTest_tests PRIVATE gtest gtest_main gmock_main banking)
