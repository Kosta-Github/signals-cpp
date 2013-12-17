cmake_minimum_required(VERSION 2.8)

project(signals-cpp CXX)
message("configure: signals-cpp unittests")

# configure catch
include_directories("${SIGNALS_CPP_3RD_PARTY_DIR}/catch/include")
add_definitions(-DCATCH_CONFIG_PREFIX_ALL)

add_executable(
	signals-cpp_unittests
	main.cpp
	signals-cpp_unittests.cpp
	../signals-cpp/connection.hpp
	../signals-cpp/connections.hpp
	../signals-cpp/signal.hpp
	../signals-cpp/signals-cpp.hpp
)
	 
add_test(
	NAME signals-cpp_unittests
	COMMAND signals-cpp_unittests
)
