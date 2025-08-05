cmake_minimum_required(VERSION 3.2)

set(SERVER_TARGET_NAME server)

set(SERVER_SOURCES
    server.cpp
    src/Server.cpp
    src/Player.cpp
)

add_executable(${SERVER_TARGET_NAME} ${SERVER_SOURCES})

target_include_directories(${SERVER_TARGET_NAME} PUBLIC
    ${COMMON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/include 
)

target_link_libraries(${SERVER_TARGET_NAME} PRIVATE ${COMMON_LIBS})