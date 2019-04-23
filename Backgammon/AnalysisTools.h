#if !defined(BACKGAMMON_ANALYSISTOOLS_H)
#define BACKGAMMON_ANALYSISTOOLS_H

#pragma once

struct DiceResultFrequency
{
    int sum;        // Includes doubles
    int frequency;  // Out of 36
};

extern DiceResultFrequency diceResultFrequencies[13];

#endif // !defined(BACKGAMMON_ANALYSISTOOLS_H)
