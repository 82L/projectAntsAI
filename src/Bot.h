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
    AStar *pAStarResolver;
    std::vector<Ant*> trackedAnts {};
    std::vector<Location> foodsPursued = std::vector<Location>();
    std::vector<std::pair<Location, int>*> hillsPursued = std::vector<std::pair<Location,int>*>();
    std::vector<Location> enemyPursued = std::vector<Location>();
    int foodGatherers = 0;
    int attackersAnt = 0;
    int attackersHill = 0;
    int explorers = 0;
    
    Bot();

    // Plays a single game of Ants
    void PlayGame();

    // Makes moves for a single turn
    void MakeMoves();
    void StartLogic();
    std::vector<DIRECTION>* CreateFoodPath(int ant);
    std::vector<DIRECTION>* CreateAttackPathHill(int ant);
    std::vector<DIRECTION>* CreateAttackPathEnemy(int ant);
    void AntMakeMove(Ant *pCurrentAnt, int antNumber);
    void DecreaseStats(Ant *pCurrentAnt);
    bool CheckLocationValidity(Location toCheck);
    void CleanVectors();

    // Indicates to the engine that it has made its moves
    void EndTurn();
};

#endif //BOT_H_
