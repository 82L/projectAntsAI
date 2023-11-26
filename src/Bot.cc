#include "Bot.h"

using namespace std;

void Bot::PlayGame()
{
    // Reads the game parameters and sets up
    cin >> currentState;
    currentState.Setup();
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
        for (int d = 0; d < T_DIRECTIONS; d++)
        {
            const Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], d);

            if (!currentState.grid[antCheckLocation.row][antCheckLocation.col].isWater)
            {
                currentState.MakeAntMove(currentState.myAnts[ant], 1);
            }
            else
            {
                currentState.MakeAntMove(currentState.myAnts[ant], 2);
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
