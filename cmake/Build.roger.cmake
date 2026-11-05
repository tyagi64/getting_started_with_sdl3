
if(WIN32)
    set(ROGER_PLATFORM "win")
elseif(APPLE)
    set(ROGER_PLATFORM "mac")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ROGER_PLATFORM "linux")
else()
    message(FATAL_ERROR
        "Unsupported platform: ${CMAKE_SYSTEM_NAME}"
    )
endif()

message(STATUS "Roger target platform: ${ROGER_PLATFORM}")


file(GLOB_RECURSE ROGER_SHARED_SOURCES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

set(ROGER_COMMON_SOURCES)

foreach(source IN LISTS ROGER_SHARED_SOURCES)
    file(RELATIVE_PATH source_relative
        "${PROJECT_SOURCE_DIR}"
        "${source}"
    )

    file(TO_CMAKE_PATH "${source_relative}" source_relative)

    if(source_relative MATCHES "^src/system/(win|mac|linux)/")
        continue()
    endif()

    list(APPEND ROGER_COMMON_SOURCES "${source}")
endforeach()

file(GLOB_RECURSE ROGER_PLATFORM_SOURCES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/system/${ROGER_PLATFORM}/*.cpp"
)

set(ROGER_SOURCES
    ${ROGER_COMMON_SOURCES}
    ${ROGER_PLATFORM_SOURCES}
)



set(ROGER_SOURCE_EXCLUDES "" CACHE STRING
    "Project-relative source paths to exclude"
)


set(ROGER_FILTERED_SOURCES)

foreach(source IN LISTS ROGER_SOURCES)
    file(RELATIVE_PATH source_relative
        "${PROJECT_SOURCE_DIR}"
        "${source}"
    )

    file(TO_CMAKE_PATH "${source_relative}" source_relative)

    if(source_relative IN_LIST ROGER_SOURCE_EXCLUDES)
        message(STATUS "Excluding source: ${source_relative}")
        continue()
    endif()

    list(APPEND ROGER_FILTERED_SOURCES "${source}")
endforeach()



set(ROGER_MAIN_SOURCE "${PROJECT_SOURCE_DIR}/src/main.cpp")

if(NOT EXISTS "${ROGER_MAIN_SOURCE}")
    message(FATAL_ERROR
        "Roger requires src/main.cpp as its executable entry point."
    )
endif()

if("src/main.cpp" IN_LIST ROGER_SOURCE_EXCLUDES)
    message(FATAL_ERROR
        "src/main.cpp cannot be excluded from the Roger executable."
    )
endif()


add_executable(${PROJECT_NAME}
    ${ROGER_FILTERED_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)
 
