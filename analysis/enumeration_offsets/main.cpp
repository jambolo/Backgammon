#include "Backgammon/types.h"

#include <Misc/Probability.h>
#include <array>
#include <cstdint>
#include <numeric>
#include <stdio.h>

using Points = std::array<int, 6>;

static int constexpr NUMBER_OF_POINTS = 6 + 1; // Includes OFF

static Points  construct(int64_t i);
static int64_t id(Points const & p);

int main(int argc, const char * argv[])
{
    printf("static int64_t const offsets[NUMBER_OF_POINTS - 1][NUMBER_OF_CHECKERS] =\n{\n");
    for (int i = 0; i < NUMBER_OF_POINTS-1; ++i)
    {
        printf("    { ");
        for (int j = 0; j < NUMBER_OF_CHECKERS; ++j)
        {
            printf("%lld", multisets(NUMBER_OF_POINTS - i, j));
            if (j != NUMBER_OF_CHECKERS - 1)
                printf(", ");
        }
        if (i != NUMBER_OF_POINTS - 1)
            printf(" },\n");
        else
            printf(" }\n");
    }
    printf("};\n");

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

Points construct(int64_t i)
{
    Points v{ 0, 0, 0, 0, 0, 0 };
    int    count = 0;
    for (int p = 0; p < NUMBER_OF_POINTS; ++p)
    {
    }
    return v;
}
