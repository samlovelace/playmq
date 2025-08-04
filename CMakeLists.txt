cmake_minimum_required(VERSION 3.1)
project(playmq)

include(${CMAKE_SOURCE_DIR}/common/CommonConfig.cmake)

add_subdirectory(client)
add_subdirectory(server)
