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
)
	 
target_link_libraries(
	signals-cpp_unittests
	signals-cpp
)

add_test(
	NAME signals-cpp_unittests
	COMMAND signals-cpp_unittests
)
