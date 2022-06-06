cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(banking)

add_library(Account STATIC Account.cpp)
add_library(Transaction STATIC Transaction.cpp)
