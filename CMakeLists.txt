cmake_minimum_required(VERSION 2.8)

project(signals-cpp CXX)
message("configure: signals-cpp")

include(CMakeConfigCompilers.cmake)

set(SIGNALS_CPP_3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)

SET(EXECUTABLE_OUTPUT_PATH         ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(.)

# configure unit tests
enable_testing()

add_subdirectory(signals-cpp)
add_subdirectory(test)
