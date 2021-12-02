cmake_minimum_required (VERSION 3.10)
project(position_evaluator CXX)

option(BUILD_SHARED_LIBS       "Build libraries as DLLs"                    FALSE)

set(INCLUDE_PATHS
    .
    ${CMAKE_SOURCE_DIR}
)

set(SOURCES
    main.cpp
)
source_group(Sources FILES ${SOURCES})

add_executable(position_evaluator ${SOURCES})
target_include_directories(position_evaluator PRIVATE ${INCLUDE_PATHS})
target_link_libraries(position_evaluator PUBLIC
    Backgammon
    nlohmann_json::nlohmann_json
)
target_compile_definitions(position_evaluator
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
