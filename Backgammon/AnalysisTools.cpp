#include "AnalysisTools.h"

#include "Board.h"

namespace AnalysisTools
{

PipsFrequency pipsFrequencies[] =
{
    {  3, 2 },  // (1, 2), (2, 1)
    {  4, 3 },  // (1, 3), (3, 1), (1, 1) -- excludes double 2s, includes double 1s
    {  5, 4 },  // (1, 4), (2, 3), (3, 2), (4, 1)
    {  6, 4 },  // (1, 5), (2, 4), (4, 2), (5, 1) -- excludes double 3s
    {  7, 6 },  // (1, 6), (2, 5), (3, 4), (4, 3), (5, 2), (6, 1)
    {  8, 5 },  // (2, 6), (3, 5), (5, 3), (6, 2), (2, 2) -- excludes double 4s, includes double 2s
    {  9, 4 },  // (3, 6), (4, 5), (5, 4), (6, 3)
    { 10, 2 },  // (4, 6), (6, 4) -- excludes double 5s
    { 11, 2 },  // (5, 6), (6, 5)
    { 12, 1 },  // (3, 3) -- excludes double 6s, includes double 3s
    { 16, 1 },  // (4, 4) -- includes double 4s
    { 20, 1 },  // (5, 5) -- includes double 5s
    { 24, 1 }   // (6, 6) -- includes double 6s
};

// Results from position_evaluator
extern float distancesByPoint[1 + Board::SIZE + 1] =
{
    0.0000000f,      // OFF
    1.0000000f,
    1.0000000f,
    1.0000000f,
    1.0555556f,
    1.1388889f,
    1.2500000f,
    1.3641975f,
    1.5401235f,
    1.6983025f,
    1.8404492f,
    1.9461591f,
    2.0718021f,
    2.1891599f,
    2.2908117f,
    2.4052140f,
    2.5255754f,
    2.6745259f,
    2.7904934f,
    2.9041982f,
    3.0183797f,
    3.1587780f,
    3.2707955f,
    3.3829023f,
    3.5020771f,
    3.6484592f       // Bar
};

} // namespace AnalysisTools
