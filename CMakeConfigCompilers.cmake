# Check for cxx0x, cxx11, and libcxx11 options
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" HAS_CXX0x)
CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++11 -stdlib=libc++" HAS_LIBCXX11)

# switch to the right compiler
if(HAS_LIBCXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif(HAS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(HAS_CXX0x)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif()

# check for adding a reference to the pthread lib
CHECK_CXX_COMPILER_FLAG("-pthread" HAS_PTHREAD)
if(HAS_PTHREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(HAS_PTHREAD)
