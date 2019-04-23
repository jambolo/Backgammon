#include "GameState.h"

#include "Board.h"
#include "Move.h"
#include "types.h"

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
#include "StaticEvaluator.h"
#endif

#if defined(ANALYSIS_GAME_STATE)
#include <nlohmann/json.hpp>
#endif

using json = nlohmann::json;

GameState::GameState(Board const & board,
                     int           cube,
                     Color         cubeOwner,
                     Color         diceOwner,
                     Color         whoseTurn,
                     bool          doubleOffered,
                     Resignation   resignationOffered,
                     int           die0,
                     int           die1,
                     int           moveNumber,
                     Result        result,
                     bool          crawford,
                     int           length)
    : board_(board)
    , cube_(cube)
    , cubeOwner_(cubeOwner)
    , diceOwner_(diceOwner)
    , whoseTurn_(whoseTurn)
    , doubleOffered_(doubleOffered)
    , resignationOffered_(resignationOffered)
    , die0_(die0)
    , die1_(die1)
    , moveNumber_(moveNumber)
    , result_(result)
    , crawford_(crawford)
    , length_(length)
    , fingerprint_(0)
{
}

void GameState::initialize()
{
    board_.initialize();
    cube_               = 1;
    cubeOwner_          = Color::INVALID;
    diceOwner_          = Color::BLACK;
    whoseTurn_          = Color::BLACK;
    doubleOffered_      = false;
    resignationOffered_ = Resignation::NONE;
    die0_               = 0;
    die1_               = 0;
    moveNumber_         = 0;
    result_             = Result::NOT_STARTED;
    crawford_           = false;
    length_             = 0;
    fingerprint_        = 0;
}

bool GameState::initializeFromId(char const * matchId, char const * positionId)
{
    return true;
}

void GameState::generateResponses(int depth, std::vector<GamePlayer::GameState *> & responses) const
{
}

void GameState::makeMove(Move const & move)
{
}

std::string GameState::matchId() const
{
    std::string result;
    return result;
}

uint64_t GameState::initialFingerprint()
{
    return 0;
}

bool operator ==(GameState const & x, GameState const & y)
{
    // Easy quick test
    if (x.fingerprint_ != y.fingerprint_)
        return false;

    // They might be the same. Eliminate false positives.
    return true;
}

#if defined(ANALYSIS_GAME_STATE)
void GameState::AnalysisData::reset()
{
}

nlohmann::json GameState::AnalysisData::toJson() const
{
    return json();
}
#endif // defined(ANALYSIS_GAME_STATE)
