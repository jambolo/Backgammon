#if !defined(BACKGAMMON_BOARD_H)
#define BACKGAMMON_BOARD_H

#pragma once

#include "Backgammon/types.h"
#include <string>

class Board
{
public:

    // Points are 1 - 24, OFF is 0 and BAR is 25
    static int constexpr SIZE     = 24;       // Number of points
    static int constexpr OFF      = 0;        // Position of an off checker
    static int constexpr POINT_1  = 1;        // Position of the first point
    static int constexpr POINT_24 = SIZE;     // Position of the last point
    static int constexpr BAR      = SIZE + 1; // Position of a checker on the bar

    // Constructor
    Board();

    // Constructor
    Board(Color color, char const * positionId);

    // Initialize the board to the standard starting position
    void initialize();

    // Add a checker to a point
    void add(Color color, int to);

    // Remove a checker from a point
    void remove(Color color, int from);

    // Move one checker from one point to another
    void move(Color color, int from, int to);

    // Access points directly (note that points are 0-based)
    int   operator [](size_t i) const { return board_[i]; }
    int & operator [](size_t i)       { return board_[i]; }

    // Returns the number of checkers on a point.
    int at(int i) const             { return board_[i - POINT_1]; }

    // Returns the number of checkers of the specified color that are on the bar.
    int barCount(Color color) const { return bar_[(int)color]; }

    // Returns the number of checkers of the specified color that are off.
    int offCount(Color color) const { return off_[(int)color]; }

    // Returns the GNUBG position ID with respect to the specified player
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

    int bar_[NUMBER_OF_COLORS];     // Number of checkers on the bar of for each color.
    int off_[NUMBER_OF_COLORS];     // Number of off checkers for each color
};

// Returns true if the other board is the same as this one
bool operator ==(Board const & x, Board const & y);

#endif // !defined(BACKGAMMON_BOARD_H)
