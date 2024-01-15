#ifndef ANT_H_
#define ANT_H_
#pragma once
#include <stack>

#include "Direction.h"
#include "Location.h"

struct Ant
{
    std::stack<DIRECTION>* savedPath;
    Location currentLocation;
    Location previousLocation;
};
#endif //ANT_H_