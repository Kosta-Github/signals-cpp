cmake_minimum_required(VERSION 2.8.8)

project(cute CXX)
message("configure: cute")

include(CMakeConfigCompilers.cmake)

include_directories(.)

# configure unit tests
enable_testing()

add_subdirectory(cute)
add_subdirectory(test)
