#ifndef ANT_H_
#define ANT_H_
#pragma once
#include <stack>

#include "Location.h"

enum class DIRECTION;

enum JOB
{
    CollectingFood = "Collecting Food",
    Exploring = "Exploring",
    AttackingEnemy = "Attacking Enemy",
    AttackingHill = "Attacking Hill",
};
struct Ant
{
    std::stack<DIRECTION>* savedPath;
    Location currentLocation;
    Location previousLocation;
    JOB currentJob;
    Ant()
    {
        
    }
};
#endif //ANT_H_