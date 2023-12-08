#include "State.h"

using namespace std;

State::State()
{
    isGameOver = false;
    currentTurn = 0;
    bug.open("./debug.txt");
}

State::~State()
{
    bug.close();
}

void State::Setup()
{
    grid = vector<vector<Square>>(rowCount, vector<Square>(colCount, Square()));
}

void State::Reset()
{
    myAnts.clear();
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    foods.clear();
    for (int row = 0; row < rowCount; row++)
        for (int col = 0; col < colCount; col++)
            if (!grid[row][col].isWater)
                grid[row][col].reset();
}

void State::MakeAntMove(const Location& antLocation, DIRECTION direction)
{
    cout << "o " << antLocation.row << " " << antLocation.col << " " << direction << endl;

    Location nLoc = GetLocation(antLocation, direction);
    grid[nLoc.row][nLoc.col].ant = grid[antLocation.row][antLocation.col].ant;
    grid[antLocation.row][antLocation.col].ant = -1;
}

double State::GetWrappedDistance(const Location& location1, const Location& location2)
{
    int d1 = abs(location1.row - location2.row);
    int d2 = abs(location1.col - location2.col);
    int dr = min(d1, rowCount-d1);
    int dc = min(d2, colCount-d2);

    return sqrt(dr * dr + dc * dc);
}

Location State::GetLocation(const Location& startLocation, DIRECTION direction)
{
    DirectionData data{direction};
    return Location((startLocation.row + data.y + rowCount) % rowCount,
                    (startLocation.col + data.x + colCount) % colCount);
}

void State::UpdateVisionInformation()
{
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for (int a = 0; a < static_cast<int>(myAnts.size()); a++)
    {
        sLoc = myAnts[a];
        locQueue.push(sLoc);

        std::vector<std::vector<bool>> visited(rowCount, std::vector<bool>(colCount, false));
        grid[sLoc.row][sLoc.col].isVisible = true;
        visited[sLoc.row][sLoc.col] = true;

        while (!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for (const auto& direcion: DIRECTIONS)
            {
                nLoc = GetLocation(cLoc, direcion);

                if (!visited[nLoc.row][nLoc.col] && GetWrappedDistance(sLoc, nLoc) <= viewRadius)
                {
                    grid[nLoc.row][nLoc.col].isVisible = true;
                    locQueue.push(nLoc);
                }
                visited[nLoc.row][nLoc.col] = true;
            }
        }
    }
}

ostream& operator<<(ostream& os, const State& state)
{
    for (int row = 0; row < state.rowCount; row++)
    {
        for (int col = 0; col < state.colCount; col++)
        {
            if (state.grid[row][col].isWater)
                os << '%';
            else if (state.grid[row][col].isFood)
                os << '*';
            else if (state.grid[row][col].isHill)
                os << static_cast<char>('A' + state.grid[row][col].hillPlayer);
            else if (state.grid[row][col].ant >= 0)
                os << static_cast<char>('a' + state.grid[row][col].ant);
            else if (state.grid[row][col].isVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
}

istream& operator>>(istream& is, State& state)
{
    int row, col, player;
    string inputType, junk;

    // Finds out which turn it is
    while (is >> inputType)
    {
        if (inputType == "end")
        {
            state.isGameOver = true;
            break;
        }
        if (inputType == "turn")
        {
            is >> state.currentTurn;
            break;
        }
        // Unknown line
        getline(is, junk);
    }

    if (state.currentTurn == 0)
    {
        // Reads game parameters
        while (is >> inputType)
        {
            if (inputType == "loadtime")
                is >> state.loadTime;
            else if (inputType == "turntime")
                is >> state.turnTime;
            else if (inputType == "rows")
                is >> state.rowCount;
            else if (inputType == "cols")
                is >> state.colCount;
            else if (inputType == "turns")
                is >> state.turnsCount;
            else if (inputType == "player_seed")
                is >> state.playerSeed;
            else if (inputType == "viewradius2")
            {
                is >> state.viewRadius;
                state.viewRadius = sqrt(state.viewRadius);
            }
            else if (inputType == "attackradius2")
            {
                is >> state.attackRadius;
                state.attackRadius = sqrt(state.attackRadius);
            }
            else if (inputType == "spawnradius2")
            {
                is >> state.spawnRadius;
                state.spawnRadius = sqrt(state.spawnRadius);
            }
            else if (inputType == "ready") // End of parameter input
            {
                state.timer.Start();
                break;
            }
            else // Unknown line
                getline(is, junk);
        }
    }
    else
    {
        // Reads information about the current turn
        while (is >> inputType)
        {
            if (inputType == "w") // Water square
            {
                is >> row >> col;
                state.grid[row][col].isWater = true;
            }
            else if (inputType == "f") // Food square
            {
                is >> row >> col;
                state.grid[row][col].isFood = true;
                state.foods.push_back(Location(row, col));
            }
            else if (inputType == "a") // Live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                if (player == 0)
                    state.myAnts.push_back(Location(row, col));
                else
                    state.enemyAnts.push_back(Location(row, col));
            }
            else if (inputType == "d") // Dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].deadAnts.push_back(player);
            }
            else if (inputType == "h")
            {
                is >> row >> col >> player;
                state.grid[row][col].isHill = true;
                state.grid[row][col].hillPlayer = player;
                if (player == 0)
                    state.myHills.push_back(Location(row, col));
                else
                    state.enemyHills.push_back(Location(row, col));
            }
            else if (inputType == "players") // Player information
                is >> state.noPlayers;
            else if (inputType == "scores") // Score information
            {
                state.scores = vector<double>(state.noPlayers, 0.0);
                for (int p = 0; p < state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if (inputType == "go") // End of turn input
            {
                if (state.isGameOver)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.Start();
                break;
            }
            else // Unknown line
                getline(is, junk);
        }
    }

    return is;
}
