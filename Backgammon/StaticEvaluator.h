#if !defined(BACKGAMMON_STATICEVALUATOR_H)
#define BACKGAMMON_STATICEVALUATOR_H

#pragma once

#include "GamePlayer/StaticEvaluator.h"

namespace GamePlayer { class GameState; }
class Move;

class StaticEvaluator : public GamePlayer::StaticEvaluator
{
public:
    static int constexpr MAX_PIP_COUNT = 375;    // Maximum possible pip count

    virtual float evaluate(GamePlayer::GameState const & state) const override;
    virtual float firstPlayerWins() const override  { return (float)MAX_PIP_COUNT; }
    virtual float secondPlayerWins() const override { return (float)-MAX_PIP_COUNT; }

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    float incremental(Move const & move)
#endif
};

#endif // !defined(BACKGAMMON_STATICEVALUATOR_H)
