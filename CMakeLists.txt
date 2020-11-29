cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(LOCAL_CFG "" CACHE FILEPATH "Path to local config")

if(NOT LOCAL_CFG)
    message(FATAL_ERROR "LOCAL_CFG variable not set. Use -DLOCAL_CFG=<file> to set local config. See local-example.cmake for available options")
endif()
include(${LOCAL_CFG})

set(NAME argon-driver)

project(${NAME})

add_subdirectory(src)