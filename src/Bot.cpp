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
    while(std::cin >> currentState)
    {
        currentState.UpdateVisionInformation();
        MakeMoves();
        EndTurn();
    }
}

void Bot::MakeMoves()
{
    currentState.bug << "turn " << currentState.currentTurn << ":" << "\n";
    // currentState.bug << currentState << "\n";
    // Picks out moves for each ant
    if(currentState.myAnts.size() < 10)
    {
        StartLogic();
    }
    CleanVectors();
    currentState.bug << "time taken: " << currentState.timer.GetDuration() << "ms" << std::endl << std::endl;
}

void Bot::StartLogic()
{
    for(int ant = 0; ant < static_cast<int>(currentState.myAnts.size()); ant++)
    {
        auto trackedAnt = std::find_if(trackedAnts.begin(),
                                       trackedAnts.end(),
                                       [&](Ant *x)
                                       {
                                           return x->currentLocation == currentState.myAnts[ant];
                                       });
        Ant *currentAnt;
        //Test if we found an ant
        if(trackedAnt == trackedAnts.end())
        {
            currentAnt = new Ant();
            currentAnt->currentLocation = currentState.myAnts[ant];
            trackedAnts.push_back(currentAnt);
        }
        else
        {
            currentAnt = (*trackedAnt);
        }
        // if there is no current path
        if(currentAnt->savedPath == nullptr || currentAnt->savedPath->empty())
        {
            currentState.bug << "Finding path for number " << ant << "\n";
            std::vector<DIRECTION> *savedPath = nullptr;
            Location foodTracked = currentState.foods[0];
            for(auto food : currentState.foods)
            {
                if(std::find(foodsPursued.begin(), foodsPursued.end(), food) == foodsPursued.end())
                {
                    std::vector<DIRECTION> *path = aStarResolver->GetPathInstructionsDirection(
                        currentState.myAnts[ant],
                        food);
                    if(path != nullptr && (savedPath == nullptr || savedPath->size() > path->size()))
                    {
                        savedPath = path;
                        foodTracked = food;
                    }
                }
            }
            if(savedPath != nullptr)
            {
                currentState.bug << "Path found number " << ant << "\n";
                foodsPursued.push_back(foodTracked);
                currentAnt->currentJob = JOB::CollectingFood;
                foodGatherers++;
                currentAnt->savedPath = savedPath;
                AntMakeMove(currentAnt, ant);
            }
            else
            {
                currentState.bug << "No found path for number " << ant << "\n";
                for(const auto direction : DIRECTIONS)
                {
                    Location antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], direction);
                    if(CheckLocationValidity(antCheckLocation))
                    {
                        currentState.MakeAntMove(currentState.myAnts[ant], direction);
                        break;
                    }

                }
            }

        }
        else if(!currentAnt->savedPath->empty())
        {
            AntMakeMove(currentAnt, ant);
        }
    }
}

void Bot::AntMakeMove(Ant *currentAnt, int antNumber)
{
    currentState.bug << "Number " << antNumber << " follows path \n";
    Location antCheckLocation = currentState.GetLocation(currentState.myAnts[antNumber], currentAnt->savedPath->back());
    if(CheckLocationValidity(antCheckLocation))
    {
        currentState.MakeAntMove(currentState.myAnts[antNumber], currentAnt->savedPath->back());
        currentAnt->previousLocation = currentAnt->currentLocation;
        currentAnt->currentLocation = antCheckLocation;
        currentAnt->savedPath->pop_back();
        currentAnt->hasWaited = false;
        if(currentAnt->savedPath->empty())
        {
            UpdateStats(currentAnt);
            currentAnt->currentJob = UnEmployed;
        }
    }
    else
    {
        if(!currentAnt->hasWaited)
        {
            currentAnt->hasWaited = true;
        }
        else
        {
            currentAnt->savedPath->clear();
            UpdateStats(currentAnt);
            currentAnt->currentJob = UnEmployed;
            currentAnt->hasWaited = false;
        }

    }
}

void Bot::UpdateStats(Ant *currentAnt)
{
    switch(currentAnt->currentJob)
    {
    case JOB::UnEmployed:
        break;
    case JOB::CollectingFood:
        foodGatherers--;
        break;
    case JOB::Exploring:
        explorers--;
        break;
    case JOB::AttackingEnemy:
        attackersAnt--;
        break;
    case JOB::AttackingHill:
        attackersHill--;
        break;
    }

}

bool Bot::CheckLocationValidity(Location toCheck)
{
    return currentState.grid[toCheck.row][toCheck.col].CheckSquareIsValidForMove();
}
void Bot::CleanVectors()
{
    foodsPursued.erase(std::remove_if(foodsPursued.begin(), foodsPursued.end(), [&](Location location)
    {
        return !currentState.grid[location.row][location.col].isFood;
    }), foodsPursued.end());
}
void Bot::EndTurn()
{
    if(currentState.currentTurn > 0)
        currentState.Reset();
    currentState.currentTurn++;

    std::cout << "go" << std::endl;
}
