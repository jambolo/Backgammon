#include "Backgammon/types.h"

#include <Misc/Probability.h>
#include <stdio.h>

static int constexpr NUMBER_OF_POINTS = 6 + 1; // Includes OFF

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
