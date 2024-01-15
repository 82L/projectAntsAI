#include "AStar.h"


AStar::AStar(State *state)
{
    _gameState = state;
}

std::vector<DIRECTION>* AStar::GetPathInstructionsDirection(Location startLocation, Location endLocation) const
{
    std::vector<AStarNode*> *uncheckedNodes = new std::vector<AStarNode *>();
    AStarNode* startNode = new AStarNode(startLocation, nullptr, 0, DIRECTION::N);
    uncheckedNodes->push_back(startNode);
    std::vector<AStarNode*> *checkedNodes = new std::vector<AStarNode *>();
    AStarNode* endNode = nullptr;
    while(!uncheckedNodes->empty() && endNode == nullptr)
    {
        AStarNode* nodeToCheck = GetNextNode(uncheckedNodes, &endLocation);
        if(nodeToCheck->location == endLocation)
        {
            endNode = nodeToCheck;
            break;
        }
        AddNeighborsToList(nodeToCheck, uncheckedNodes, checkedNodes);
        checkedNodes->push_back(nodeToCheck);
    }
    // return nullptr;

    if(endNode != nullptr)
    {
        std::vector<DIRECTION>* path = CreatePathDirections(endNode);
        uncheckedNodes->clear();
        checkedNodes->clear();
        return path;
    }
    return nullptr;
    //do the path
}

double AStar::HeuristicLocationCalculator(const Location* givenLocation, const Location* endLocation) const
{
   return _gameState->GetWrappedDistance(*givenLocation, *endLocation);
}

void AStar::AddNeighborsToList(AStarNode *baseLocation, std::vector<AStarNode*> *uncheckedNodes,  std::vector<AStarNode*> *checkedNodes) const
{
    for(const auto& direction : DIRECTIONS)
    {
        Location foundLocation = _gameState->GetLocation(baseLocation->location, direction);
        if(_gameState->grid[foundLocation.row][foundLocation.col].CheckSquareIsValidCalculation())
        {
            if(std::find_if(checkedNodes->begin(), checkedNodes->end(), [&](const AStarNode *node)
            {
                return node->location == foundLocation;
            }) == checkedNodes->end())
            {
                auto newNode = new AStarNode(foundLocation,baseLocation, baseLocation->cost + 1, direction);
                auto foundNode = find_if( uncheckedNodes->begin(), uncheckedNodes->end(), [&](const AStarNode *x)
                {
                    return x->location == newNode->location;
                });
                if(foundNode != uncheckedNodes->end())
                {
                    if((*foundNode)->cost > newNode->cost )
                    {
                        uncheckedNodes->erase(foundNode);
                        uncheckedNodes->push_back(newNode);
                    }
                    else
                    {
                        delete newNode;
                    }
                }else
                {
                    uncheckedNodes->push_back(newNode);
                }
            }
        }
    }
}

std::vector<DIRECTION>* AStar::CreatePathDirections(const AStarNode *endNode)
{
    const AStarNode* currentNode = endNode;
    auto path = new std::vector<DIRECTION>();
    do {
        path->push_back(currentNode->direction);
        currentNode = currentNode->previousNode;
    } while(currentNode->previousNode != nullptr);

    return path;
}

AStarNode* AStar::GetNextNode(std::vector<AStarNode*> *uncheckedNodes, Location *endLocation) const
{
    auto endNode = std::find_if(uncheckedNodes->begin(), uncheckedNodes->end(),
                                      [&](AStarNode *x)
                                      {
                                          return x->location == *endLocation;
                                      });
    if( endNode != uncheckedNodes->end())
    {
        return *endNode;
    }
    return GetBestNodeForPath(uncheckedNodes, endLocation);
}


AStarNode* AStar::GetBestNodeForPath(std::vector<AStarNode*> *uncheckedNodes, const Location *endLocation) const
{
    std::sort(uncheckedNodes->begin(), uncheckedNodes->end(),[&](const AStarNode* node1, const AStarNode* node2)
                                              {
                                                  return (node1->cost + HeuristicLocationCalculator(&node1->location, endLocation))
                                                      >  (node2->cost + HeuristicLocationCalculator(&node2->location, endLocation));
                                              } );
    const auto returnNode  = uncheckedNodes->back();
    uncheckedNodes->pop_back();
    // uncheckedNodes->erase(std::remove(uncheckedNodes->begin(), uncheckedNodes->end(), returnNode), uncheckedNodes->end());
    return returnNode;
}
