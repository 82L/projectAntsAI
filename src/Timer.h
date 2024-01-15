#ifndef TIMER_H_
#define TIMER_H_

/*
    struct for checking how long it has been since the start of the turn.
*/
#include <ctime>
#include <windows.h>

struct Timer
{
    clock_t startTime;
    clock_t currentTime;

    Timer() = default;

    void Start()
    {
        startTime = clock();
    }

    double GetDuration()
    {
        currentTime = clock();

        return currentTime - startTime;
    }

    double GetCurrentTime()
    {
        return clock();
    }
};
#endif //TIMER_H_
