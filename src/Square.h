#ifndef SQUARE_H_
#define SQUARE_H_

#include <vector>

/**
    \brief Struct for representing a square in the grid.
**/
struct Square
{
#pragma region Members

    // Square states
    bool isVisible = false;
    bool isWater = false;
    bool isHill = false;
    bool isFood = false;

    int ant = -1;
    int hillPlayer = -1;
    std::vector<int> deadAnts;

#pragma endregion

#pragma region Functions
    Square() = default;

    // Resets the information for the square except water information
    void reset()
    {
        isVisible = false;
        isHill = false;
        isFood = false;
        hillPlayer = -1;
        ant = -1;
        deadAnts.clear();
    }

    bool CheckSquareIsValidForMove()
    {
        return !this->isWater && !this->isFood && this->ant != 0 && (!this->isHill || this->hillPlayer != 0);
    }
    bool CheckSquareIsValidCalculation()
    {
        return !this->isWater && this->isVisible && (!this->isHill || this->hillPlayer != 0);
    }

#pragma endregion
};

#endif //SQUARE_H_
