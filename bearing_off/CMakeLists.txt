cmake_minimum_required (VERSION 3.10)
project(bearing_off CXX)

option(BUILD_SHARED_LIBS       "Build libraries as DLLs"                    FALSE)

set(INCLUDE_PATHS
    .
)

set(SOURCES
    main.cpp
)
source_group(Sources FILES ${SOURCES})

add_executable(bearing_off ${SOURCES})
target_include_directories(bearing_off PRIVATE ${INCLUDE_PATHS})
target_compile_definitions(bearing_off
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
