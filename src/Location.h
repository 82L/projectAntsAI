#ifndef LOCATION_H_
#define LOCATION_H_
#pragma once
/**
    \brief Struct for representing locations in the grid.
**/
struct Location
{
    int row;
    int col;

    Location()
    {
        row = 0;
        col = 0;
    }

    /** 
     * \param y row
     * \param x column
     */
    Location(const int y, const int x)
    {
        row = y;
        col = x;
    }
    
    bool operator==(const Location& otherLocation) const
    {
        return col == otherLocation.col && row == otherLocation.row;
    }

};

#endif //LOCATION_H_
