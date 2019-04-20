#if !defined(BACKGAMMON_BOARD_H)
#define BACKGAMMON_BOARD_H

#pragma once

#include "Backgammon/types.h"
#include <string>

class Board
{
public:
    static int constexpr SIZE = 24; // Number of points

    Board();

    // Initialize the board to the standard starting position
    void initialize();

    int   operator [](size_t i) const { return board_[i]; }
    int & operator [](size_t i)       { return board_[i]; }

    int at(int i) const        { return board_[i - 1]; }
    int bar(Color color) const { return bar_[(int)color]; }

    // Returns the position ID with respect to the specified player
    std::string positionId(Color color) const;

private:
    friend bool operator ==(Board const & x, Board const & y);

    // Remove all pieces from the board
    void clear();

    // Each element represents a point. The index of a point is one less than its number from the point of view of Black (e.g.
    // Black's ace point has an index of 0, and White's ace point has an index of SIZE-1).
    // Each element contains the number of checkers on the point. 0 means the point is empty. Black checkers have a positive number
    // and white checkers have a negative number.
    int board_[SIZE];

    // Number of checkers on the bar of for each color.
    int bar_[NUMBER_OF_COLORS];
};

// Returns true if the other board is the same as this one
bool operator ==(Board const & x, Board const & y);

#endif // !defined(BACKGAMMON_BOARD_H)
