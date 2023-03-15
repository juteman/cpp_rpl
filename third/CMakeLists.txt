# gsl start
add_subdirectory(GSL)
add_library(external_gsl INTERFACE IMPORTED GLOBAL ../src/base/pch.h)
target_link_libraries(external_gsl INTERFACE Microsoft.GSL::GSL)

#external range
add_library(external_ranges INTERFACE IMPORTED GLOBAL)
add_subdirectory(range-v3)


target_link_libraries(external_ranges INTERFACE range-v3::range-v3)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(external_ranges
            INTERFACE
            /Zc:preprocessor # need for range-v3 see https://github.com/ericniebler/range-v3#supported-compilers
            /wd5105 # needed for `/Zc:preprocessor`, suppressing C5105 "macro expansion producing 'defined' has undefined behavior"
            )
endif()