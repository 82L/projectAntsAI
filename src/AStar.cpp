#include "AStar.h"

#include "AStarNode.h"

AStar::AStar(State *state)
{
    _gameState = state;
}

std::vector<Location>* AStar::GetPathInstructions( Location startLocation, Location endLocation)
{
    auto *uncheckedNodes = new std::vector<AStarNode*>();
    uncheckedNodes->push_back(new AStarNode(startLocation, nullptr, 0));
    std::vector<AStarNode*> *checkedNodes = {};
    AStarNode* endNode = nullptr;
    while(!uncheckedNodes->empty())
    {
        AStarNode* nodeToCheck = GetNextNode(uncheckedNodes, &endLocation);
        if(nodeToCheck->location == endLocation)
        {
            endNode = nodeToCheck;
            break;
        }
        checkedNodes->insert(checkedNodes->begin(), nodeToCheck);
        // AddToUncheckedList(GetNeighborList(nodeToCheck, checkedNodes), uncheckedNodes, endLocation);
        AddNeighborsToList(nodeToCheck,uncheckedNodes, checkedNodes);

    }
    if(endNode != nullptr)
    {
        std::vector<Location>* path = CreatePath(endNode);
        uncheckedNodes->clear();
        checkedNodes->clear();
        return path;
    }
    return nullptr;
    //do the path
}

double AStar::HeuristicLocationCalculator( Location* givenLocation,  Location* endLocation) 
{
   return _gameState->GetWrappedDistance(*givenLocation, *endLocation);
}

void AStar::AddNeighborsToList(AStarNode *baseLocation, std::vector<AStarNode*> *uncheckedNodes,  std::vector<AStarNode*> *checkedNodes)
{
    for(int i = 0; i < T_DIRECTIONS; i++)
    {
        Location foundLocation = _gameState->GetLocation(baseLocation->location, i);
        if(!_gameState->grid[foundLocation.row][foundLocation.col].isWater && _gameState->grid[foundLocation.row][foundLocation.col].isVisible)
        {
            if(std::find_if(checkedNodes->begin(), checkedNodes->end(), [&](AStarNode *node)
            {
                return node->location == foundLocation;
            }) == checkedNodes->end())
            {
                auto newNode = new AStarNode(foundLocation,baseLocation, baseLocation->cost + 1);
                auto foundNode = find_if( uncheckedNodes->begin(), uncheckedNodes->end(), [&](AStarNode *x)
                {
                    return x->location == newNode->location;
                });
                if(foundNode != uncheckedNodes->end())
                {
                    if((*foundNode)->cost < newNode->cost )
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

std::vector<Location>* AStar::CreatePath(AStarNode *endNode)
{
    AStarNode* currentNode = endNode;
    auto path = new std::vector<Location>();
    do {
        path->push_back(currentNode->location);
        currentNode = currentNode->previousNode;
    } while(currentNode != nullptr);

    return path;
}

AStarNode* AStar::GetNextNode(std::vector<AStarNode*> *uncheckedNodes, Location *endLocation) 
{
    auto endNode = std::find_if(uncheckedNodes->begin(), uncheckedNodes->end(),
                                      [&]( AStarNode* x)
                                      {
                                          return x->location == *endLocation;
                                      });
    if( endNode != uncheckedNodes->end())
    {
        return *endNode;
    }
    return GetBestNodeForPath(uncheckedNodes, endLocation);
}


AStarNode* AStar::GetBestNodeForPath(std::vector<AStarNode*> *uncheckedNodes, Location *endLocation) 
{
    auto returnNode  = std::min_element(uncheckedNodes->begin(), uncheckedNodes->end(),
                                              [&](AStarNode* node1, AStarNode* node2)
                                              {
                                                  return (node1->cost + HeuristicLocationCalculator(&node1->location, endLocation))
                                                      <  (node2->cost + HeuristicLocationCalculator(&node2->location, endLocation));
                                              });
    uncheckedNodes->erase(returnNode);
    return *returnNode;
}
