function(babylon_add_library TARGET)
    message("babylon_add_library ${TARGET}")

    add_library(${TARGET} ${ARGN})
    babylon_target_clang_tidy(${TARGET})

    # Create namespaced alias
    add_library(${META_PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

    # Create API export header
    generate_export_header(${TARGET}
        EXPORT_FILE_NAME  ${EXPORT_FILE}
        EXPORT_MACRO_NAME ${EXPORT_MACRO}
        )

    # Set library ouput name
    set_target_properties(${TARGET}
        PROPERTIES  PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}"
        OUTPUT_NAME $<LOWER_CASE:${TARGET}>
        VERSION ${META_VERSION}
        SOVERSION ${META_VERSION}
        )

    # Compile definitions
    target_compile_definitions(${TARGET}
        PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${BABYLON_UPPER}_STATIC_DEFINE>
        )

    # group sources
    get_target_property(sources ${TARGET} SOURCES)
    source_group_by_path_all(${CMAKE_CURRENT_SOURCE_DIR} ${sources})

endfunction()


function(babylon_add_executable TARGET)
    message("babylon_add_executable ${TARGET}")

    add_executable(${TARGET} ${ARGN})
    babylon_target_clang_tidy(${TARGET})
endfunction()
