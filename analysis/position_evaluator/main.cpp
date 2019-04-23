#include "Backgammon/AnalysisTools.h"
#include "Backgammon/Board.h"

#include <iostream>

static void roll(int distance, int i, double f, double & n, double & sum)
{
    ++i;
    for (auto const & t : diceResultFrequencies)
    {
        int d = distance - t.sum;
        if (d <= 0)
        {
            n   += t.frequency * f;
            sum += t.frequency * i * f;
        }
        else
        {
            roll(d, i, f * t.frequency / 36.0, n, sum);
        }
    }
}

int main(int argc, const char * argv[])
{
    for (int point = Board::POINT_1; point <= Board::BAR; ++point)
    {
        double n   = 0;
        double sum = 0;

        roll(point, 0, 1.0, n, sum);
        std::cout <<
            "point: " << point <<
            ", n = " << n <<
            ", sum = " << sum <<
            ", average = " << sum / n <<
            std::endl;
    }
    return 0;
}
