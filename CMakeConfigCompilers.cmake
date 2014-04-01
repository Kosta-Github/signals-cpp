# Check for cxx0x, cxx11, and libcxx11 options
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" HAS_CXX0x)
CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++11 -stdlib=libc++" HAS_LIBCXX11)
CHECK_CXX_COMPILER_FLAG("-pthread" HAS_PTHREAD)

# switch to the right compiler
if(HAS_LIBCXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif(HAS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(HAS_CXX0x)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(MSVC)
#    add_definitions()
endif()

# add reference to the pthread lib
if(HAS_PTHREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(HAS_PTHREAD)

# Add a workaround for the g++ bug
# c.f. http://gcc.gnu.org/bugzilla/show_bug.cgi?id=52680
#      http://gcc.gnu.org/ml/libstdc++-cvs/2012-q4/msg00045.html
if (${CMAKE_SYSTEM} MATCHES "Linux")
    # Try to use std::this_thread::yield()
    include(CheckCXXSourceCompiles)
    CHECK_CXX_SOURCE_COMPILES(
        "#include <thread> int main(int argc, const char** argv) { std::this_thread::yield(); return 0; }"
        STD_THREAD_YIELD_CAN_BE_USED
    )
    if (NOT STD_THREAD_YIELD_CAN_BE_USED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_SCHED_YIELD")
    endif()
endif()
