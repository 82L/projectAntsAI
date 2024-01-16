#include "Bot.h"

// using namespace std;

Bot::Bot()
{
    srand(time(nullptr));
}

void Bot::PlayGame()
{
    // Reads the game parameters and sets up
    std::cin >> currentState;
    currentState.Setup();
    currentState.bug << "Astar Creation" << "\n";
    pAStarResolver = new AStar(&currentState);
    currentState.bug << "Astar Created" << "\n";
    EndTurn();

    // Continues making moves while the game is not over
    while(std::cin >> currentState)
    {
        // Reset next moves
        nextAntsPositions.clear();
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
    // if(currentState.myAnts.size() < 10)
    // {
    StartLogic();
    // }
    CleanVectors();
    currentState.bug << "time taken: " << currentState.timer.GetDuration() << "ms" << std::endl << std::endl;
}

void Bot::StartLogic()
{
    for(int ant = 0; ant < static_cast<int>(currentState.myAnts.size()); ant++)
    {
        auto iTrackedAnt = std::find_if(trackedAnts.begin(),
                                        trackedAnts.end(),
                                        [&](Ant *pAnt)
                                        {
                                            return pAnt->currentLocation == currentState.myAnts[ant];
                                        });
        Ant *pCurrentAnt;
        //Test if we found an ant
        if(iTrackedAnt == trackedAnts.end())
        {
            pCurrentAnt = new Ant();
            pCurrentAnt->currentLocation = currentState.myAnts[ant];
            trackedAnts.push_back(pCurrentAnt);
        }
        else
        {
            pCurrentAnt = (*iTrackedAnt);

            // if(rand() % 10 == 9)
            // {
            //     if(pCurrentAnt->savedPath != nullptr && !pCurrentAnt->savedPath->empty())
            //     {
            //         DecreaseStats(pCurrentAnt);
            //         pCurrentAnt->currentJob =UnEmployed;
            //         pCurrentAnt->savedPath->clear();
            //     }
            // }
        }
        pCurrentAnt->hasPlayed = true;
        // if there is no current path
        if(pCurrentAnt->savedPath == nullptr || pCurrentAnt->savedPath->empty())
        {
            currentState.bug << "Finding path for number " << ant << "\n";
            std::vector<DIRECTION> *pSavedPath = nullptr;
            if(foodGatherers > 10)
            {
                pSavedPath = CreateAttackPathHill(ant);
                if(pSavedPath != nullptr)
                {

                    pCurrentAnt->currentJob = JOB::AttackingHill;
                    attackersHill++;
                }
                else
                {
                    pSavedPath = CreateAttackPathEnemy(ant);
                    if(pSavedPath != nullptr)
                    {

                        pCurrentAnt->currentJob = JOB::AttackingEnemy;
                        attackersAnt++;
                    }
                    else
                    {
                        pCurrentAnt->currentJob = JOB::Exploring;
                    }
                }
            }
            if(pSavedPath == nullptr)
            {
                pSavedPath = CreateFoodPath(ant);
                if(pSavedPath != nullptr)
                {
         😭           pCurrentAnt->currentJob = JOB::CollectingFood;
                    foodGatherers++;
                }
            }
            if(pSavedPath != nullptr)
            {
                currentState.bug << "Path found number " << ant << "\n";
                // foodsPursued.push_back(foodTracked);

                pCurrentAnt->savedPath = pSavedPath;
                AntMakeMove(pCurrentAnt, ant);
            }
            else
            {
                currentState.bug << "No found path for number " << ant << "\n";
                Location antCheckLocation;
                DIRECTION direction;
                int testNumber = 0;
                do
                {
                    direction = DIRECTIONS[rand() % DIRECTIONS.size()];
                    // int value = rand()%4;
                    antCheckLocation = currentState.GetLocation(currentState.myAnts[ant], direction);
                    testNumber++;
                }
                while(!CheckLocationValidity(antCheckLocation) && testNumber < 20);
                if(testNumber < 20 && CheckLocationValidity(antCheckLocation))
                {
                    currentState.bug << "Direction for ant " << ant << "\n";
                    currentState.MakeAntMove(currentState.myAnts[ant], direction);
                }

            }

        }
        else if(!pCurrentAnt->savedPath->empty())
        {
            AntMakeMove(pCurrentAnt, ant);
        }
    }
}

std::vector<DIRECTION>* Bot::CreateFoodPath(int ant)
{
    std::vector<DIRECTION> *pNewPath = nullptr;
    if(!currentState.foods.empty())
    {
        std::vector<Location> foods = currentState.foods;
        foods.erase(std::remove_if(foods.begin(),
                                   foods.end(),
                                   [&](Location foodLocation)
                                   {
                                       return std::find(foodsPursued.begin(), foodsPursued.end(), foodLocation) !=
                                           foodsPursued.end();
                                   }),
                    foods.end());
        std::sort(foods.begin(),
                  foods.end(),
                  [&](Location location1, Location location2)
                  {
                      return currentState.GetWrappedDistance(currentState.myAnts[ant], location1)
                          > currentState.GetWrappedDistance(currentState.myAnts[ant], location2);
                  });
        Location foodTracked = foods.back();
        for(int i = 0; i < 5 && !foods.empty(); i++)
        {
            Location food = foods.back();
            foods.pop_back();
            // std::vector<DIRECTION> *path =nullptr;
            std::vector<DIRECTION> *pPath = pAStarResolver->GetPathInstructionsDirection(
                currentState.myAnts[ant],
                food);
            if(pPath != nullptr && (pNewPath == nullptr || pNewPath->size() > pPath->size()))
            {
                pNewPath = pPath;
                foodTracked = food;
            }

        }
        if(pNewPath != nullptr)
        {
            foodsPursued.push_back(foodTracked);
        }
    }
    return pNewPath;
}

std::vector<DIRECTION>* Bot::CreateAttackPathHill(int ant)
{
    std::vector<DIRECTION> *pSavedPath = nullptr;
    if(!currentState.enemyHills.empty())
    {
        Location hillsTracked = currentState.enemyHills[0];
        std::pair<Location, int> *pPair = nullptr;
        for(auto hill : currentState.enemyHills)
        {
            auto iFound = std::find_if(hillsPursued.begin(),
                                       hillsPursued.end(),
                                       [&](std::pair<Location, int> *pCurrentPair)
                                       {
                                           return pCurrentPair->first == hill;
                                       });
            if(iFound == hillsPursued.end() || (*iFound)->second < 5)
            {
                std::vector<DIRECTION> *pPath = pAStarResolver->GetPathInstructionsDirection(
                    currentState.myAnts[ant],
                    hill);
                if(pPath != nullptr && (pSavedPath == nullptr || pSavedPath->size() > pPath->size()))
                {
                    pSavedPath = pPath;
                    hillsTracked = hill;
                    if(iFound != hillsPursued.end())
                    {
                        pPair = *iFound;
                    }
                    else
                    {
                        pPair = nullptr;
                    }
                }
            }
        }
        if(pSavedPath != nullptr)
        {
            if(pPair == nullptr)
            {
                hillsPursued.push_back(new std::pair<Location, int>(hillsTracked, 1));
            }
            else
            {
                pPair->second++;
            }

        }
    }
    return pSavedPath;

}

std::vector<DIRECTION>* Bot::CreateAttackPathEnemy(int ant)
{
    std::vector<DIRECTION> *pSavedPath = nullptr;
    if(!currentState.enemyAnts.empty())
    {
        Location enemyTracked = currentState.enemyAnts[0];
        for(auto enemyAnt : currentState.enemyAnts)
        {
            if(std::find(enemyPursued.begin(), enemyPursued.end(), enemyAnt) == enemyPursued.end())
            {
                std::vector<DIRECTION> *pPath = pAStarResolver->GetPathInstructionsDirection(
                    currentState.myAnts[ant],
                    enemyAnt);
                if(pPath != nullptr && (pSavedPath == nullptr || pSavedPath->size() > pPath->size()))
                {
                    pSavedPath = pPath;
                    enemyTracked = enemyAnt;
                }
            }
        }
        if(pSavedPath != nullptr)
        {
            enemyPursued.push_back(enemyTracked);
        }
    }
    return pSavedPath;

}

void Bot::AntMakeMove(Ant *pCurrentAnt, int antNumber)
{
    currentState.bug << "Number " << antNumber << " follows path \n";
    Location antCheckLocation = currentState.
        GetLocation(currentState.myAnts[antNumber], pCurrentAnt->savedPath->back());
    if(CheckLocationValidity(antCheckLocation))
    {
        currentState.MakeAntMove(currentState.myAnts[antNumber], pCurrentAnt->savedPath->back());
        pCurrentAnt->previousLocation = pCurrentAnt->currentLocation;
        pCurrentAnt->currentLocation = antCheckLocation;
        pCurrentAnt->savedPath->pop_back();
        pCurrentAnt->hasWaited = false;
        if(pCurrentAnt->savedPath->empty())
        {
            DecreaseStats(pCurrentAnt);
            pCurrentAnt->currentJob = UnEmployed;
        }
    }
    else
    {
        if(!pCurrentAnt->hasWaited)
        {
            pCurrentAnt->hasWaited = true;
        }
        else
        {
            pCurrentAnt->savedPath->clear();
            DecreaseStats(pCurrentAnt);
            pCurrentAnt->currentJob = UnEmployed;
            pCurrentAnt->hasWaited = false;
        }

    }
}

void Bot::DecreaseStats(Ant *pCurrentAnt)
{
    switch(pCurrentAnt->currentJob)
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
    bool found = false;
    for (auto it : nextAntsPositions)
    {
        if (it == toCheck)
        {
            found = true;
            break;
        }
    }
    
    if (found)
    {
        currentState.bug << "Collisions Found\n";
    }
    bool isValid = currentState.grid[toCheck.row][toCheck.col].CheckSquareIsValidForMove() && !found;

    if (isValid)
    {
        nextAntsPositions.push_back(toCheck);
    }
    return isValid;
}

void Bot::CleanVectors()
{
    foodsPursued.erase(std::remove_if(foodsPursued.begin(),
                                      foodsPursued.end(),
                                      [&](Location location)
                                      {
                                          return !currentState.grid[location.row][location.col].isFood;
                                      }),
                       foodsPursued.end());
    hillsPursued.erase(std::remove_if(hillsPursued.begin(),
                                      hillsPursued.end(),
                                      [&](std::pair<Location, int> *pPair)
                                      {
                                          return !currentState.grid[pPair->first.row][pPair->first.col].isHill;
                                      }),
                       hillsPursued.end());
    enemyPursued.erase(std::remove_if(enemyPursued.begin(),
                                      enemyPursued.end(),
                                      [&](Location location)
                                      {
                                          return currentState.grid[location.row][location.col].ant > 0;
                                      }),
                       enemyPursued.end());

    trackedAnts.erase(std::remove_if(trackedAnts.begin(),
                                     trackedAnts.end(),
                                     [&](Ant *pAnt)
                                     {
                                         if(!pAnt->hasPlayed)
                                         {
                                             DecreaseStats(pAnt);
                                             return true;
                                         }

                                         pAnt->hasPlayed = false;
                                         return false;
                                     }),
                      trackedAnts.end());
}

void Bot::EndTurn()
{
    if(currentState.currentTurn > 0)
        currentState.Reset();
    currentState.currentTurn++;

    std::cout << "go" << '\n';
}
