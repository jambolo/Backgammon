//
//  main.cpp
//  position_evaluator
//
//  Created by John Bolton on 4/19/19.
//  Copyright © 2019 John Bolton. All rights reserved.
//

#include <array>
#include <iostream>

struct Entry
{
    int64_t value;
    int64_t f;
};

Entry frequencies[13] =
{
    {  3, 2 },
    {  4, 3 },
    {  5, 4 },
    {  6, 4 },
    {  7, 6 },
    {  8, 5 },
    {  9, 4 },
    { 10, 2 },
    { 11, 2 },
    { 12, 1 },
    { 16, 1 },
    { 20, 1 },
    { 24, 1 }
};

static void roll(int64_t distance, int64_t i, double f, double & n, double & sum)
{
    ++i;
    for (auto const & t : frequencies)
    {
        int64_t d = distance - t.value;
        if (d <= 0)
        {
            n += t.f * f;
            sum += t.f * i * f;
        }
        else
        {
            roll(d, i, f * t.f/36.0, n, sum);
        }
    }
}

int main(int argc, const char * argv[])
{
    for (int64_t point = 1; point <= 25; ++point)
    {
        double n = 0;
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
