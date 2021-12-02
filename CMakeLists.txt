cmake_minimum_required (VERSION 3.10)
project(BackgammonPlayer VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS       "Build libraries as DLLs"                    FALSE)

find_package(nlohmann_json REQUIRED)
find_package(Misc REQUIRED)
#find_package(GamePlayer REQUIRED)

set(BACKGAMMON_PLAYER_INCLUDE_PATHS
    .
)

set(BACKGAMMON_PLAYER_SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    main.cpp
    Player.cpp
    Player.h
)
source_group(Sources FILES ${BACKGAMMON_PLAYER_SOURCES})

add_executable(backgammon_player ${BACKGAMMON_PLAYER_SOURCES})
target_include_directories(backgammon_player PRIVATE ${BACKGAMMON_PLAYER_INCLUDE_PATHS})
target_link_libraries(backgammon_player PUBLIC
    Backgammon
    GamePlayer
    nlohmann_json::nlohmann_json
)
target_compile_definitions(backgammon_player
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)

add_subdirectory(analysis)
add_subdirectory(Backgammon)
add_subdirectory(GamePlayer)

