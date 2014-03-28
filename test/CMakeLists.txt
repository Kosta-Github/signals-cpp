cmake_minimum_required(VERSION 2.8)

project(signals-cpp CXX)
message("configure: signals-cpp unittests")

# configure cute
include_directories("${SIGNALS_CPP_3RD_PARTY_DIR}/cute/")

add_executable(
	signals_unittests
	main.cpp
	stepper.hpp
	signals_unittests.cpp
	../signals-cpp/config.hpp
	../signals-cpp/connection.hpp
	../signals-cpp/connections.hpp
	../signals-cpp/signal.hpp
)
	 
add_test(
	NAME signals_unittests
	COMMAND signals_unittests
)
