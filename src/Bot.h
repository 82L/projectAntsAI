#ifndef BOT_H_
#define BOT_H_
#pragma once
#include "Ant.h"
#include "AStar.h"
#include "State.h"
#include <iostream>

/**
    \brief This struct represents your bot in the game of Ants
**/
struct Bot
{
    State currentState;
    AStar aStarResolver;
    std::vector<Ant> trackedAnts;
    std::vector<Location> foodsPursued;
    Bot();

    // Plays a single game of Ants
    void PlayGame();

    // Makes moves for a single turn
    void MakeMoves();
    bool CheckLocationValidity(Location toCheck);

    // Indicates to the engine that it has made its moves
    void EndTurn();
};

#endif //BOT_H_
