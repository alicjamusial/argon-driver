cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(LOCAL_CFG "" CACHE FILEPATH "Path to local config")

if(LOCAL_CFG)
    message(STATUS "Using config file from ${LOCAL_CFG}")
    include(${LOCAL_CFG})
endif()

set(NAME argon-driver)

project(${NAME})

add_subdirectory(argon)