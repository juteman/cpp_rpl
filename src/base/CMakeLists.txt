add_library(lib_base INTERFACE)
get_filename_component(src_loc .. REALPATH)

target_precompile_headers(lib_base INTERFACE $<$<COMPILE_LANGUAGE:CXX,OBJCXX>:${src_loc}/base/pch.h>)

nice_target_sources(lib_base ${src_loc}
        INTERFACE
        base/assertion.h
        base/variant.h
        base/optional.h
        base/build_config.h
        base/unique_function.h
        base/index_based_iterator.h
        base/match_method.h
        base/basic_types.h
        base/pch.h
        )


target_include_directories(lib_base
        INTERFACE
        ${src_loc}
        )

target_link_libraries(lib_base
        INTERFACE
        lib_rpl
        external_gsl
        external_ranges
        )