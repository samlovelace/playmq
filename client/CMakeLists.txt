cmake_minimum_required(VERSION 3.2)

set(CLIENT_TARGET_NAME client)

set(CLIENT_SOURCES
    client.cpp
    src/Client.cpp
)

add_executable(${CLIENT_TARGET_NAME} ${CLIENT_SOURCES})

target_include_directories(${CLIENT_TARGET_NAME} PUBLIC 
    ${COMMON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${CLIENT_TARGET_NAME} PRIVATE ${COMMON_LIBS})