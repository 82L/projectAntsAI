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
     * \param r row
     * \param c column
     */
    Location(const int r, const int c)
    {
        row = r;
        col = c;
    }
};

#endif //LOCATION_H_
