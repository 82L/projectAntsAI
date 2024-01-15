#include "Bot.h"


// using namespace std;

Bot::Bot()
{
}

void Bot::PlayGame()
{
    // Reads the game parameters and sets up
    std::cin >> currentState;
    currentState.Setup();
    currentState.bug << "Astar Creation" << "\n";
    aStarResolver = new AStar(&currentState);
    currentState.bug << "Astar Created" << "\n";
    EndTurn();

    // Continues making moves while the game is not over
    while (std::cin >> currentState)
    {
        currentState.UpdateVisionInformation();
        MakeMoves();
        EndTurn();
    }
}

void Bot::MakeMoves()
{
    currentState.bug << "turn " << currentState.currentTurn << ":" << "\n";
    // Picks out moves for each ant
    for (int ant = 0; ant < static_cast<int>(currentState.myAnts.size()); ant++)
    {
        auto trackedAnt = std::find_if(trackedAnts.begin(), trackedAnts.end(), [&](Ant x)
        {
            return x.currentLocation == currentState.myAnts[ant];
        });
        if(ant == 0 && trackedAnt == trackedAnts.end())
        {
            currentState.bug << "Finding path for ant" << "\n";
            Ant currentAnt = Ant();
            currentAnt.currentLocation = currentState.myAnts[ant];
            std::stack<DIRECTION> *savedPath = nullptr;
            Location foodTracked = currentState.foods[0];
            currentState.bug << "checking food" << "\n";
            for(auto food : currentState.foods)
            {
                if(std::find(foodsPursued.begin(), foodsPursued.end(), food) == foodsPursued.end())
                {
                    std::stack<DIRECTION> *path = aStarResolver->GetPathInstructionsDirection(currentState.myAnts[ant], food);
                    if(path != nullptr && (savedPath == nullptr || savedPath->size() > path->size()))
                    {
                    //     currentState.bug << "path found ant" << "\n";
                    //     
                        savedPath = path;
                        foodTracked = food;
                    }
                }
            
            }
            if(savedPath!= nullptr)
            {
                currentAnt.savedPath = savedPath;
                foodsPursued.push_back(foodTracked);
                Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], currentAnt.savedPath->top());
                if (CheckLocationValidity(antCheckLocation))
                {
                    currentState.MakeAntMove(currentState.myAnts[ant], currentAnt.savedPath->top());
                    currentAnt.currentLocation=antCheckLocation;
                    currentAnt.savedPath->pop();
                }
                trackedAnts.push_back(currentAnt);
            }
            else
            {
                for(const auto direction: DIRECTIONS)
                {
                    Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], direction);
                    if (CheckLocationValidity(antCheckLocation))
                    {
                        currentState.MakeAntMove(currentState.myAnts[ant], direction);
                        break;
                    }

                }
            }
           
           
           
    
        }
       else if(trackedAnt != trackedAnts.end() && !trackedAnt->savedPath->empty())
       {
           Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], trackedAnt->savedPath->top());
           if (CheckLocationValidity(antCheckLocation))
           {
               currentState.MakeAntMove(currentState.myAnts[ant], trackedAnt->savedPath->top());
               trackedAnt->currentLocation=antCheckLocation;
               trackedAnt->savedPath->pop();
           }
       }
    }

    currentState.bug << "time taken: " << currentState.timer.GetDuration() << "ms" << std::endl << std::endl ;
}
bool Bot::CheckLocationValidity(Location toCheck)
{
    const Square* square = &currentState.grid[toCheck.row][toCheck.col];
    return !square->isWater && !square->isFood && square->ant == -1 && (!square->isHill || square->hillPlayer != 0);
}
void Bot::EndTurn()
{
    if (currentState.currentTurn > 0)
        currentState.Reset();
    currentState.currentTurn++;

    std::cout << "go" << std::endl;
}
