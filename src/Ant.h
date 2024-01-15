#ifndef ANT_H_
#define ANT_H_
#pragma once
#include <stack>

#include "Location.h"

enum class DIRECTION;

struct Ant
{
    std::stack<DIRECTION>* savedPath;
    Location currentLocation;
    Location previousLocation;
    Ant()
    {
        
    }
};
#endif //ANT_H_