#include "StaticEvaluator.h"

#include "GameState.h"
#include "Move.h"

#include <cassert>
#include <limits>

namespace
{
// Average number of rolls it takes to bear off a checker at each point. See point_evaluator for derivation.
float constexpr s_AverageBearOffRollsByPoint[Board::SIZE] =
{
    1.00000f,
    1.00000f,
    1.00000f,
    0.05556f,
    0.13889f,
    0.25000f,
    0.36420f,
    0.54012f,
    0.69830f,
    1.84045f,
    1.94616f,
    2.07180f,
    2.18916f,
    2.29081f,
    2.40521f,
    2.52558f,
    2.67453f,
    2.79049f,
    2.90420f,
    3.01838f,
    3.15878f,
    3.27080f,
    3.38290f,
    3.50208f,
};

float constexpr s_AverageBearOffRollFromBar = 3.64846f;
} // anonymous namespace

float StaticEvaluator::evaluate(GamePlayer::GameState const & state) const
{
    GameState const & s = static_cast<GameState const &>(state);

    // Find the difference (White - Black) in total average number of rolls to bear off.
    //
    // Note that a Black point count is positive and a White point count is negative. Higher is is better for Black, so that the
    // sum must be negated. Also, note that the bar count is positive for both colors. In other words, the actual calculation is
    // -(Black + bar(Black) + White - bar(White))
    float value = 0.0f;
    for (int p = 0; p < Board::SIZE; ++p)
    {
        value = s_AverageBearOffRollsByPoint[p] * s.board_[p];
    }
    value += s_AverageBearOffRollFromBar * s.board_.bar(Color::BLACK);
    value -= s_AverageBearOffRollFromBar * s.board_.bar(Color::WHITE);

    value = -value;

#if defined(PROFILING)

    // Randomize the value slightly so that equivalent moves have different
    // values and profiling results of ordering methods are more realistic

    value += ((state.GetHashCode() & 0x7) - 4) * (value * std::numeric_limits<float>::epsilon());

#endif

    return value;
}

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)

// This function returns the change in the value of the board *** BY COLOR ***.

float StaticEvaluator::incremental(Move const & move)
{
    float value = 0.0f;
    return value;
}

#endif // defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
