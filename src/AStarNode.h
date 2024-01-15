#ifndef ASTARNODE_H_
#define ASTARNODE_H_
#pragma once
#include "Location.h"

enum class DIRECTION;

struct AStarNode
{
    Location location;
    AStarNode *previousNode;
    int cost = 0;
    DIRECTION direction;
    
    AStarNode(Location location, AStarNode *previousNode, const int cost, const DIRECTION direction)
    {
        this->location = location;
        this->previousNode = previousNode;
        this->cost = cost;
        this->direction = direction;
    }
};
#endif