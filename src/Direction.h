#pragma once
#include <array>

enum class DIRECTION
{
    N = 'N',
    E = 'E',
    S = 'S',
    W = 'W'
};

inline std::ostream& operator<<(std::ostream& os, const DIRECTION& e) {
    return os << static_cast<char>(e);
}

struct DirectionData
{
    int x;
    int y;
    DirectionData(DIRECTION direction)
    {
        /*
         *
         *  Direction 
         *       N
         *       |
         *   W---x---E
         *       |
         *       S
         *
         *   are translated to coordinates of type
         * 
         *   X-------------> X
         *   |
         *   |
         *   |
         *   |
         *   Y
         *
         */
        
        switch (direction) {
        case DIRECTION::N:
            x = 0;
            y = -1;
            break;
        case DIRECTION::E:
            x = 1;
            y = 0;
            break;
        case DIRECTION::S:
            x = 0;
            y = 1;
            break;
        case DIRECTION::W:
            x = -1;
            y = 1;
            break;
        }
    }
};

constexpr std::array<DIRECTION, 4> DIRECTIONS = {DIRECTION::N,DIRECTION::E,DIRECTION::S,DIRECTION::W};
