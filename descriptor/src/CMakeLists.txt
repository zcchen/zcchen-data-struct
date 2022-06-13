set(targetName "${PROJECT_NAME}-descriptor")

file(GLOB src_files *.c* */*.c*)
add_library(${targetName} SHARED ${src_files})

# header config
target_include_directories(${targetName}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../headers/include/
)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_compile_definitions(${targetName} PRIVATE DLL_EXPORT)
    set_target_properties(${targetName} PROPERTIES
        SUFFIX ".dll"
        BUILD_WITH_INSTALL_RPATH ON
        LINK_FLAGS "-Wl,-rpath,$ORIGIN/"
    )
    install(TARGETS ${targetName}
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/
    )
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
else()  # unix
    set_target_properties(${targetName} PROPERTIES
        OUTPUT_NAME "${targetName}"
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION}
        BUILD_WITH_INSTALL_RPATH ON
        LINK_FLAGS "-Wl,-rpath,$ORIGIN/../lib"
    )
    install(TARGETS ${targetName}
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    )
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/../include/
        FILES_MATCHING PATTERN "*.h"
)

