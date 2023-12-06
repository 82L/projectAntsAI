#pragma once
#include <vector>

#include "Location.h"

class AStar
{
public:
    std::vector<int> GetPathInstruction(Location startingPoint, Location endPoint);
};
