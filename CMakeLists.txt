cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(NAME argon-driver)

project(${NAME})

#include(FetchContent)
#include(CheckIncludeFileCXX)

add_subdirectory(src)