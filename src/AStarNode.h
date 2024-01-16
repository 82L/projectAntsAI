#ifndef ASTARNODE_H_
#define ASTARNODE_H_
#pragma once
#include "Location.h"

enum class DIRECTION;

struct AStarNode
{
    Location location;
    AStarNode *pPreviousNode;
    int cost = 0;
    DIRECTION direction; // Direction of previous node
    
    AStarNode(Location location, AStarNode *previousNode, const int cost, const DIRECTION direction)
    {
        this->location = location;
        this->pPreviousNode = previousNode;
        this->cost = cost;
        this->direction = direction;
    }
};
#endif