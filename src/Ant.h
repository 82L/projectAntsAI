#ifndef ANT_H_
#define ANT_H_
#pragma once
#include <stack>
#include <vector>

#include "Location.h"

enum class DIRECTION;

enum JOB
{
    UnEmployed = 0,
    CollectingFood = 1,
    Exploring = 2,
    AttackingEnemy = 3,
    AttackingHill = 4,
};
struct Ant
{
    std::vector<DIRECTION>* savedPath;
    Location currentLocation;
    Location previousLocation;
    JOB currentJob = JOB::UnEmployed;
    bool hasWaited = false;
    bool hasPlayed = false;
    Ant() = default;
};
#endif //ANT_H_