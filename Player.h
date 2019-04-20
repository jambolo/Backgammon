#if !defined(PLAYER_H)
#define PLAYER_H

#pragma once

#include "Backgammon/types.h"

class GameState;

class Player
{
public:
    Player(Color color);

    // Returns the player's next move
    virtual GameState myTurn(GameState const & s0) = 0;

protected:

    Color myColor_;
};

#endif // !defined(PLAYER_H)
