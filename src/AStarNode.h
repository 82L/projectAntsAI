#pragma once
#include "Location.h"

struct AStarNode
{
    Location location;
    AStarNode *previousNode;
    int cost = 0;
    AStarNode(Location location, AStarNode *previousNode, const int cost)
    {
        this->location = location;
        this->previousNode = previousNode;
        this-> cost = cost;
    }
};
