function(build_example)
    set(options)
    set(one_value_kw TARGET)
    set(multi_value_kw SOURCES)
    cmake_parse_arguments(build_example "${options}"
        "${one_value_kw}" "${multi_value_kw}" ${ARGN})

    if (NOT build_example_TARGET)
        message(SEND_ERROR "ERROR: build_example called without TARGET.")
        return()
    endif()
    if (NOT build_example_SOURCES)
        message(SEND_ERROR "ERROR: build_example called without SOURCES.")
        return()
    endif()

    add_executable(${build_example_TARGET} ${build_example_SOURCES})
    target_include_directories(${build_example_TARGET}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

    if (CMAKE_SYSTEM_NAME MATCHES Windows)
        set_target_properties(${build_example_TARGET} PROPERTIES
            SUFFIX ".exe"
            BUILD_WITH_INSTALL_RPATH ON
            LINK_FLAGS "-Wl,-rpath,$ORIGIN/"
        )
        install(TARGETS ${build_example_TARGET}
                RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/
        )
    elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
    else()  # unix
        set_target_properties(${build_example_TARGET} PROPERTIES
            BUILD_WITH_INSTALL_RPATH ON
            LINK_FLAGS "-Wl,-rpath,$ORIGIN/../lib"
        )
        install(TARGETS ${build_example_TARGET}
                RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
        )
    endif()
endfunction()

build_example(TARGET ${PROJECT_NAME}-descriptor-examples-simple-c SOURCES simple-c.c)
target_link_libraries(${PROJECT_NAME}-descriptor-examples-simple-c PRIVATE ${PROJECT_NAME}-descriptor)

build_example(TARGET ${PROJECT_NAME}-descriptor-examples-simple-cpp SOURCES simple-cpp.cpp)
target_link_libraries(${PROJECT_NAME}-descriptor-examples-simple-cpp PRIVATE ${PROJECT_NAME}-descriptor)

#set(targetName "")
#add_executable(${targetName} simple-c.c)
#target_link_libraries(${targetName} PRIVATE ${PROJECT_NAME}-descriptor)
#if (CMAKE_SYSTEM_NAME MATCHES Windows)
    #set_target_properties(${targetName} PROPERTIES
        #SUFFIX ".exe"
        #BUILD_WITH_INSTALL_RPATH ON
        #LINK_FLAGS "-Wl,-rpath,$ORIGIN/"
    #)
    #install(TARGETS ${targetName}
            #RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/
    #)
#elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
#else()  # unix
    #set_target_properties(${targetName} PROPERTIES
        #BUILD_WITH_INSTALL_RPATH ON
        #LINK_FLAGS "-Wl,-rpath,$ORIGIN/../lib"
    #)
    #install(TARGETS ${targetName}
            #RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    #)
#endif()
