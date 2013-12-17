cmake_minimum_required(VERSION 2.8)

project(signals-cpp CXX)
message("configure: signals-cpp library")

add_library(
	signals-cpp SHARED
	connection.hpp
	connections.hpp
	signals-cpp.hpp
	signal.hpp
	signal.cpp
)
