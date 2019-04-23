#include "Backgammon/Board.h"
#include "Backgammon/types.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <map>
#include <numeric>
#include <stdio.h>
#include <vector>
#include <Misc/Probability.h>

using Points = std::array<int, 6>;
using Dice   = std::array<int, 4>;

static int constexpr POINT_1          = 0;
static int constexpr POINT_6          = 5;
static int constexpr MIN_DIE          = 1;
static int constexpr MAX_DIE          = 6;
static int constexpr NUMBER_OF_POINTS = 6 + 1; // Includes OFF

struct Entry
{
    Points points;
    int count;
    int pips;
    double value;

    constexpr Entry()
        : points({ 0, 0, 0, 0, 0, 0 })
        , count(0)
        , pips(0)
        , value(0.0)
    {
    }
    Entry(Points & p);
};

static Points  construct(int64_t i);
static int64_t id(Points const & p);
static int     pip_count(Points const & p);
static double  roll(Points points, int count);
static double  apply_dice(Points points, int count, Dice const & dice, int d);
static double  move(Points points, int count, Dice const & dice, int d, int from);

Entry::Entry(Points & p)
    : points(p)
    , count(std::accumulate(p.begin(), p.end(), 0))
    , pips(pip_count(p))
    , value(0.0)
{
}

std::map<int64_t, Entry> states;

int main(int argc, const char * argv[])
{
    // Very helpful: http://www.bkgm.com/articles/BenjaminRoss/EnumeratingBackgammonPositions.pdf

    // Number of possible states of up to 15 checkers bearing off.
    int64_t const NUMBER_OF_STATES = multisets(NUMBER_OF_POINTS, NUMBER_OF_CHECKERS);

    for (int64_t i = 0; i < NUMBER_OF_STATES; ++i)
    {
        Points p(construct(i));
        if (id(p) != i)
        {
            fprintf(stderr, "construct() and id() don't match for state %lld.\n", i);
            return 1;
        }
        states.emplace(i, p);
    }

    // Evaluate every state
    for (auto & e : states)
    {
        e.second.value = roll(e.second.points, e.second.count);
    }

    // Dump the values 
    printf("static float const sBearOffValuesById[] =\n{\n    ");
    int64_t i = 0;
    for (auto const & e : states)
    {
        if (i != e.first)
        {
            fprintf(stderr, "Out of order: entry %lld has id %lld.\n", i, e.first);
            return 1;
        }

        printf("%f, ", e.second.value);
        if (i % 8 == 7)
            printf("\n    ");
    }
    printf("};\nstatic_assert(sizeof(sBearOffValuesById) / sizeof(*sBearOffValuesById) == 54264);\n");

    return 0;
}

int64_t id(Points const & points)
{
    int     k  = std::accumulate(points.begin(), points.end(), 0);
    int64_t rv = 0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        rv += multisets(NUMBER_OF_POINTS - i, k - 1);
        k  -= points[i];
    }
    return rv;
}

Points construct(int64_t id)
{
    static int64_t const offsets[NUMBER_OF_POINTS - 1][NUMBER_OF_CHECKERS] =
    {
        { 1, 7, 28, 84, 210, 462, 924, 1716, 3003, 5005, 8008, 12376, 18564, 27132, 38760 },
        { 1, 6, 21, 56, 126, 252, 462, 792, 1287, 2002, 3003, 4368, 6188, 8568, 11628 },
        { 1, 5, 15, 35, 70, 126, 210, 330, 495, 715, 1001, 1365, 1820, 2380, 3060 },
        { 1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680 },
        { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 },
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    };

    int v[NUMBER_OF_POINTS] = { 0, 0, 0, 0, 0, 0, 0 };
    int count = NUMBER_OF_CHECKERS;
    for (int i = 0; i < NUMBER_OF_POINTS - 1; ++i)
    {
        int64_t const * p = std::upper_bound(std::begin(offsets[i]), std::end(offsets[i]), id) - 1;
        int n = count - int(p - offsets[i]) - 1;
        v[i]   = n;
        id    -= *p;
        count -= n;
    }
    v[NUMBER_OF_POINTS - 1] = count;
    Points points;
    std::copy(&v[1], &v[NUMBER_OF_POINTS], &points[0]);
    return points;
}

static int pip_count(Points const & points)
{
    int pips = 0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        pips += points[i] * int(i + 1);
    }
    return pips;
}

//! Returns the average number of rolls to bear off for the given board.
//!
//! @param  points      Board
//! @param  count       Number of checkers on the board
//!
//! @note   This function is called recursively.

static double roll(Points points, int count)
{
    // If there is already a value for this state, then just return it rather than computing it again.
    assert(states.find(id(points)) != states.end());
    double v = states[id(points)].value;
    if (v > 0.0)
        return v;

    // Try each possible dice roll outcome. Return the average.
    double sum = 0.0;
    for (int d0 = MIN_DIE; d0 <= MAX_DIE; ++d0)
    {
        for (int d1 = MIN_DIE; d1 <= MAX_DIE; ++d1)
        {
            int  d23  = (d0 == d1) ? d0 : 0;
            Dice dice = { d0, d1, d23, d23 };
            sum += apply_dice(points, count, dice, 0);
        }
    }
    assert(sum != 0.0);
    assert(v == 0.0 || sum / ((MAX_DIE - MIN_DIE + 1) * (MAX_DIE - MIN_DIE + 1)) == v);

    // Return average
    return sum / ((MAX_DIE - MIN_DIE + 1) * (MAX_DIE - MIN_DIE + 1));
}

//! Returns the minimum number of rolls to bear off for the given board and dice roll.
//!
//! @param  points      Board
//! @param  count       Number of checkers on the board
//! @param  dice        Dice roll to apply
//! @param  d           Which die to apply
//!
//! @note   This function is called recursively.

double apply_dice(Points points, int count, Dice const & dice, int d)
{
    assert(std::accumulate(points.begin(), points.end(), 0) > 0);

    // For each checker, return the minimum number of rolls to bear off with these dice and the current board if the checker is
    // moved with the specified die. The minimum is returned because the checker to move is chosen, and the one resulting in the
    // minimum is always the best choice.
    double minimum = std::numeric_limits<double>::max();
    for (int from = POINT_1; from <= POINT_6; ++from)
    {
        if (points[from] > 0)
        {
            double x = move(points, count, dice, d, from);
            minimum = std::min(minimum, x);
        }
    }
    return minimum;
}

//! Moves the given checker and returns the remaining number of rolls to bear off.
//!
//! @param  points      Board
//! @param  count       Number of checkers on the board
//! @param  dice        Dice roll to apply
//! @param  d           Which die to apply
//! @param  from        Checker to move
//!
//! @note   This function is called recursively.

double move(Points points, int count, Dice const & dice, int d, int from)
{
    assert(std::accumulate(points.begin(), points.end(), 0) == count);
    assert(count > 0);
    assert(d >= 0 && d < dice.size());
    assert(dice[d] >= MIN_DIE && dice[d] <= MAX_DIE);
    assert(from >= POINT_1 && from <= POINT_6);

    int to = from - dice[d];

    // Move/remove the checker
    --points[from];
    if (to >= 0)
        ++points[to];
    else
        --count;    // Removed

    // If there are still checkers to bear off, then continue. Otherwise, the value is 1.
    double value;
    if (count > 0)
    {
        // If there are more dice to apply, then apply them. Otherwise, roll again.
        ++d;
        if (d < 4 && dice[d] > 0)
            value = apply_dice(points, count, dice, d);
        else
            value = 1.0 + roll(points, count);
    }
    else
    {
        value = 1.0;
    }

    return value;
}
