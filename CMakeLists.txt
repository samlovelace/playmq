cmake_minimum_required(VERSION 3.1)
project(playmq)

include(${CMAKE_SOURCE_DIR}/common/CommonConfig.cmake)

message(STATUS "adding client subdir")
add_subdirectory(client)

message(STATUS "adding server subdir")
add_subdirectory(server)
