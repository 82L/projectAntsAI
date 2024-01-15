﻿#include "AStar.h"


AStar::AStar(State *state)
{
    _gameState = state;
}

std::stack<Location>* AStar::GetPathInstructions(Location startLocation, Location endLocation) const
{
    auto *uncheckedNodes = new std::vector<AStarNode*>();
    uncheckedNodes->push_back(new AStarNode(startLocation, nullptr, 0, DIRECTION::N));
    std::vector<AStarNode*> *checkedNodes = {};
    const AStarNode* endNode = nullptr;
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
        std::stack<Location>* path = CreatePath(endNode);
        uncheckedNodes->clear();
        checkedNodes->clear();
        return path;
    }
    return nullptr;
    //do the path
}

std::stack<DIRECTION>* AStar::GetPathInstructionsDirection( Location startLocation, Location endLocation) const
{
    // _gameState->bug << "test" << "\n";
    // _gameState->bug << _gameState << "\n";
    std::vector<AStarNode*> *uncheckedNodes = new std::vector<AStarNode *>();
    AStarNode* startNode = new AStarNode(startLocation, nullptr, 0, DIRECTION::N);
    uncheckedNodes->push_back(startNode);
    std::vector<AStarNode*> *checkedNodes = new std::vector<AStarNode *>();
    const AStarNode* endNode = nullptr;
    _gameState->bug << "test" << "\n";
    while(!uncheckedNodes->empty() && endNode == nullptr)
    {
        AStarNode* nodeToCheck = GetNextNode(uncheckedNodes, &endLocation);
        if(nodeToCheck->location == endLocation)
        {
            endNode = nodeToCheck;
            break;
        }
        AddNeighborsToList(nodeToCheck, uncheckedNodes, checkedNodes);
        // _gameState->bug << uncheckedNodes->size() << "\n";
        checkedNodes->push_back(nodeToCheck);
        // uncheckedNodes->clear();

        // AddToUncheckedList(GetNeighborList(nodeToCheck, checkedNodes), uncheckedNodes, endLocation);
    }
    // return nullptr;

    if(endNode != nullptr)
    {
        std::stack<DIRECTION>* path = CreatePathDirections(endNode);
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

std::stack<Location>* AStar::CreatePath(const AStarNode *endNode)
{
    const AStarNode* currentNode = endNode;
    auto path = new std::stack<Location>();
    do {
        path->push(currentNode->location);
        currentNode = currentNode->previousNode;
    } while(currentNode->previousNode != nullptr);
    return path;
}

std::stack<DIRECTION>* AStar::CreatePathDirections(const AStarNode *endNode)
{
    const AStarNode* currentNode = endNode;
    auto path = new std::stack<DIRECTION>();
    do {
        path->push(currentNode->direction);
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
    const auto returnNode  = std::min_element(uncheckedNodes->begin(), uncheckedNodes->end(),
                                              [&](const AStarNode* node1, const AStarNode* node2)
                                              {
                                                  return (node1->cost + HeuristicLocationCalculator(&node1->location, endLocation))
                                                      <  (node2->cost + HeuristicLocationCalculator(&node2->location, endLocation));
                                              });
    uncheckedNodes->erase(returnNode);
    return *returnNode;
}
