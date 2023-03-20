cmake_minimum_required(VERSION 3.14)
project(cpp_rpl)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0091 NEW)

include(cmake/nice_target_sources.cmake)

add_subdirectory(third)
add_subdirectory(src/rpl)
add_subdirectory(src/base)

add_library(cpp_rpl INTERFACE)

target_link_libraries(cpp_rpl
        INTERFACE
        lib_rpl
        lib_base
        )
#if (CPP_RPL_BUILD_EXAMPLE)
    add_subdirectory(example)
#endif ()