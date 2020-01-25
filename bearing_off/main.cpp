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
    long value;
    long f;
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

static void roll_again(long distance, long i, double f, double & n, double & sum)
{
    ++i;
    for (auto const & t : frequencies)
    {
        long d = distance - t.value;
        if (d <= 0)
        {
            n += t.f * f;
            sum += t.f * i * f;
        }
        else
        {
            roll_again(d, i, f * t.f/36.0, n, sum);
        }
    }
}

int main(int argc, const char * argv[])
{
    for (long point = 1; point <= 25; ++point)
    {
        double n = 0;
        double sum = 0;

        roll_again(point, 0, 1.0, n, sum);
        std::cout <<
            "point: " << point <<
            ", n = " << n <<
            ", sum = " << sum <<
            ", average = " << sum / n <<
            std::endl;
    }
    return 0;
}
