#include "Backgammon/AnalysisTools.h"
#include "Backgammon/Board.h"
#include "nlohmann/json.hpp"

#include <iostream>

// This program computes the expected number of rolls needed to bear off a piece at each position on the board, assuming
// that no positions on the board are blocked.

int main(int argc, const char * argv[])
{
    double averageRoll = 0.0;
    for (auto p : AnalysisTools::pipsFrequencies)
        averageRoll += (double)p.pips * (double)p.frequency;
    averageRoll /= double(AnalysisTools::pipsFrequencyScale);

    std::cout << "Average roll: " << averageRoll << std::endl;

    double rolls[Board::SIZE+2]; // Number of rolls for each point
    rolls[Board::OFF] = 0.0;

    // Compute the expected the number of rolls needed to go off from each point
    for (int point = Board::POINT_1; point <= Board::BAR; ++point)
    {
        // Compute the expected number of subsequent rolls needed
        double weightedSum = 0.0;
        for (auto const & t : AnalysisTools::pipsFrequencies)
        {
            if ( point > t.pips )
                weightedSum += rolls[point - t.pips] * t.frequency;
            // Note: if point <= t.pips, then it is off, s0 0
        }
        weightedSum = weightedSum / double(AnalysisTools::pipsFrequencyScale);

        // Include the first roll
        rolls[point] = weightedSum + 1.0;
    }
    std::cout << "Expected number of rolls needed to bear off a piece at each position on the board" << std::endl;
    std::cout << nlohmann::json(rolls).dump(2) << std::endl;
}
