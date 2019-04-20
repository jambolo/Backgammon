#if !defined(BACKGAMMON_TYPES_H)
#define BACKGAMMON_TYPES_H

#pragma once

enum class Color
{
    INVALID = -1,
    BLACK   = 0,
    WHITE
};
static int constexpr NUMBER_OF_COLORS = (int)Color::WHITE - (int)Color::BLACK + 1;

enum class Result
{
    INVALID     = -1,
    NOT_STARTED = 0,
    PLAYING     = 1,
    OVER        = 2,
    RESIGNED    = 3,
    DROPPED     = 4
};

enum class Resignation
{
    INVALID    = -1,
    NONE       = 0,
    SINGLE     = 1,
    GAMMON     = 2,
    BACKGAMMON = 3
};

static int constexpr NUMBER_OF_CHECKERS = 15;

#endif // !defined(BACKGAMMON_TYPES_H)
