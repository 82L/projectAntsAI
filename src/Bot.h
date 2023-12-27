#ifndef BOT_H_
#define BOT_H_

#include "AStar.h"
#include "State.h"

/**
    \brief This struct represents your bot in the game of Ants
**/
struct Bot
{
    State currentState;
    AStar aStarResolver;
    Bot();

    // Plays a single game of Ants
    void PlayGame();

    // Makes moves for a single turn
    void MakeMoves();

    // Indicates to the engine that it has made its moves
    void EndTurn();
};

#endif //BOT_H_
