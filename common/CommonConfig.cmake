
find_package(Threads REQUIRED)
find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)

set(COMMON_LIBS
    cppzmq    
    Threads::Threads
    nlohmann_json::nlohmann_json
)

set(COMMON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/common/include/
)

message(STATUS "Common Libs in Common.cmake: ${COMMON_LIBS}")