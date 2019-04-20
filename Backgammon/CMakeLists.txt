cmake_minimum_required (VERSION 3.10)

option(ANALYSIS_GAME_STATE "General GameState analysis is enabled if true" FALSE)

message(STATUS "ANALYSIS_GAME_STATE: ${ANALYSIS_GAME_STATE}")

set(SOURCES
    Board.cpp
    Board.h
    GameState.cpp
    GameState.h
    Move.cpp
    Move.h
    StaticEvaluator.cpp
    StaticEvaluator.h
    types.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(Backgammon ${SOURCES})
target_include_directories(Backgammon
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

target_link_libraries(Backgammon PUBLIC
    GamePlayer
    Misc::Misc
    nlohmann_json::nlohmann_json
)

if(ANALYSIS_GAME_STATE)
    target_compile_definitions(Backgammon PUBLIC ANALYSIS_GAME_STATE=1)
endif()

source_group(Sources FILES ${SOURCES})

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif()

