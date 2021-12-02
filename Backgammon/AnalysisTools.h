#if !defined(BACKGAMMON_ANALYSISTOOLS_H)
#define BACKGAMMON_ANALYSISTOOLS_H

#pragma once

#include "Board.h"

namespace AnalysisTools
{

// How often is each pip value rolled
struct PipsFrequency
{
    int pips;       // Value of dice roll (includes doubles)
    int frequency;  // Number of ways to roll pips
};
static int constexpr pipsFrequencyScale = 36;   // Total number of possible rolls
extern PipsFrequency pipsFrequencies[13];

// Expected number of rolls needed to bear off for each point (including OFF and BAR), assuming no points are blocked.
extern float distancesByPoint[1+Board::SIZE+1];

// Average roll including doubles
static float constexpr averageRoll = 8.16667f;

} // namespace AnalysisTools

#endif // !defined(BACKGAMMON_ANALYSISTOOLS_H)
