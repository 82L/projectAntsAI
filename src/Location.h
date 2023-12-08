#ifndef LOCATION_H_
#define LOCATION_H_

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
};

#endif //LOCATION_H_
