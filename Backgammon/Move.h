#if !defined(BACKGAMMON_MOVE_H)
#define BACKGAMMON_MOVE_H

#pragma once

#include <string>
class Move
{
public:
    Move() = default;

    // Returns the move in notation form
    std::string notation() const;

private:
};

#endif // !defined(BACKGAMMON_MOVE_H)
