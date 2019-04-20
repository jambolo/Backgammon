#if !defined(BACKGAMMON_GAMESTATE_H)
#define BACKGAMMON_GAMESTATE_H

#pragma once

#include "Board.h"
#include "GamePlayer/GameState.h"
#include "Move.h"
#include "types.h"
#include <nlohmann/json.hpp>

class Move;

class GameState : public GamePlayer::GameState
{
public:

    using CastleStatus = uint32_t;

    GameState() = default;
    GameState(Board const & board,
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
              int           length);

    // Resets the game
    void initialize();

    bool initializeFromId(char const * matchId, char const * positionId);

    virtual uint64_t fingerprint() const override { return fingerprint_; }
    virtual void     generateResponses(int depth, std::vector<GamePlayer::GameState *> & responses) const override;
    virtual PlayerId whoseTurn() const override { return (whoseTurn_ == Color::WHITE) ? PlayerId::FIRST : PlayerId::SECOND; }

    // Updates the game state with the specified move
    void makeMove(Move const & move);

    // Returns the match ID for the state
    std::string matchId() const;

    // Returns the match ID for the state
    std::string positionId() const { return board_.positionId(diceOwner_); }

    // Returns the Z-hash of the game at the start
    static uint64_t initialFingerprint();

    Board board_;                       // The board
    int cube_;                          // The value of the cube
    Color cubeOwner_;                   // Who controls the cube
    Color diceOwner_;                   // Who controls the dice
    Color whoseTurn_;                   // Who must make the next action
    bool doubleOffered_;                // True if a double was offered and has not yet been taken or dropped
    Resignation resignationOffered_;    // State of resignation
    int die0_;                          // Current value or die 0 (or 0)
    int die1_;                          // Current value of die (or 0)
    int moveNumber_;                    // Current move number
    Result result_;                     // Result of the game (including not yet started and in progress)
    bool crawford_;                     // True if this is a Crawford game
    int length_;                        // Match length
    uint64_t fingerprint_;              // Zobrist hash for this state
    Move move_;                         // Move that lead to this state

#if defined(ANALYSIS_GAME_STATE)
    struct AnalysisData
    {
        void           reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;
#endif // defined(ANALYSIS_GAME_STATE)

private:
    friend bool operator ==(GameState const & x, GameState const & y);
};

bool operator ==(GameState const & x, GameState const & y);

#endif // !defined(BACKGAMMON_GAMESTATE_H)
