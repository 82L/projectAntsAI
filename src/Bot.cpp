#include "Bot.h"
d#define DEBUG
using namespace std;

Bot::Bot(): aStarResolver(nullptr)
{
}

void Bot::PlayGame()
{
    // Reads the game parameters and sets up
    cin >> currentState;
    currentState.Setup();
    aStarResolver = AStar(&currentState);
    EndTurn();

    // Continues making moves while the game is not over
    while (cin >> currentState)
    {
        currentState.UpdateVisionInformation();
        MakeMoves();
        EndTurn();
    }
}

void Bot::MakeMoves()
{
    currentState.bug << "turn " << currentState.currentTurn << ":" << endl;
    currentState.bug << currentState << endl;

    // Picks out moves for each ant
    for (int ant = 0; ant < static_cast<int>(currentState.myAnts.size()); ant++)
    {
        for (const auto& direction : DIRECTIONS)
        {
             Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], direction);

            if (!currentState.grid[antCheckLocation.row][antCheckLocation.col].isWater )
            {
                currentState.MakeAntMove(currentState.myAnts[ant], DIRECTION::E);
            }
            else
            {
                currentState.MakeAntMove(currentState.myAnts[ant], DIRECTION::S);
            }
        }
    }

    currentState.bug << "time taken: " << currentState.timer.GetDuration() << "ms" << endl << endl;
}

void Bot::EndTurn()
{
    if (currentState.currentTurn > 0)
        currentState.Reset();
    currentState.currentTurn++;

    cout << "go" << endl;
}
