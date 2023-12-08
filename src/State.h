#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"
#include "Direction.h"

/*
    struct to store current state of the game information
*/
struct State
{
#pragma region Members

    int rowCount;
    int colCount;

    int currentTurn;
    int turnsCount;
    int noPlayers;

    double attackRadius;
    double spawnRadius;
    double viewRadius;

    double loadTime;
    double turnTime;

    std::vector<double> scores;
    bool isGameOver;
    int64_t playerSeed;

    std::vector<std::vector<Square>> grid;

    std::vector<Location> myAnts;
    std::vector<Location> enemyAnts;
    std::vector<Location> myHills;
    std::vector<Location> enemyHills;
    std::vector<Location> foods;

    Timer timer;
    Bug bug;

#pragma endregion

#pragma region Functions

    State();
    ~State();

    void Setup();

    /**
     * \brief Resets all non-water squares to land and clears the bots ant vector
     */
    void Reset();

    /**
     * \brief Outputs move information to the engine
     * \param antLocation Location of the ant
     * \param direction Direction we want the ant to move
     */

    void MakeAntMove(const Location& antLocation, DIRECTION direction);
    
    /*
        This function will update update the lastSeen value for any squares currently
        visible by one of your live ants.
    
        BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
        THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
        A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
        IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
    */
    void UpdateVisionInformation();

    /**
     * \return The euclidean distance between two locations with the edges wrapped
     */
    double GetWrappedDistance(const Location& location1, const Location& location2);

    /**
     * \brief Get the location adjacent of an other location in a specific direction with the edges wrapped
     * \return The new location
     */
    Location GetLocation(const Location& startLocation, DIRECTION direction);

#pragma  endregion
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
std::ostream& operator<<(std::ostream& os, const State& state);

// Input function
std::istream& operator>>(std::istream& is, State& state);

#endif //STATE_H_
