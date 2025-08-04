
find_package(Threads REQUIRED)
find_package(cppzmq REQUIRED)

set(COMMON_LIBS
    cppzmq    
    Threads::Threads
)

set(COMMON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/common/include/
)

message(STATUS "Common Libs in Common.cmake: ${COMMON_LIBS}")