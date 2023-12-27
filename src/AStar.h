#ifndef ASTAR_H_
#define ASTAR_H_
#include <vector>
#include "State.h"
#include "Location.h"
#include "AStarNode.h"

class AStar
{
    /**
     * \brief the state of the game, used to get grid in functions
     */
    State *_gameState;
    
    /**
     * \brief Calculate estimated cost to go from one location to another
     * \param givenLocation current location to search heuristic from
     * \param endLocation End location that must be found
     * \return Estimated cost to go from one another
     */
    double HeuristicLocationCalculator(Location *givenLocation, Location *endLocation);

    /**
     * \brief Check neighbors from baseLocation, and add them to uncheckedNodes if they're not already in it or
     * in checkedNodes
     * \param baseLocation Location to check neighbors from
     * \param uncheckedNodes List of nodes who were not checked
     * \param checkedNodes List of nodes who were checked
     */
    void AddNeighborsToList(AStarNode *baseLocation, std::vector<AStarNode*> *uncheckedNodes,
                            std::vector<AStarNode*> *checkedNodes);

    /**
     * \brief Create path from linked AStarNodes
     * \param endNode last node used 
     * \return path to the location
     */
    static std::vector<Location>* CreatePath(AStarNode* endNode);

    /**
     * \brief Get NextNode from list
     * \param uncheckedNodes list of possible next nodes
     * \param endLocation Location targeted
     * \return the next node to be checked
     */
    AStarNode* GetNextNode(std::vector<AStarNode*> *uncheckedNodes, Location *endLocation);

    /**
     * \brief Get The Best Node out of the unchecked node list
     * \param uncheckedNodes list of unchecked node
     * \param endLocation end location targeted
     * \return the node to be processed next
     */
    AStarNode* GetBestNodeForPath(std::vector<AStarNode*> *uncheckedNodes, Location *endLocation);


public:
 
    /**
     * \brief Constructor to start the Astar calculator
     * \param state game State, needed because of grid calculations
     */
    explicit AStar(State *state);
 
    /**
     * \brief 
     * \param startLocation Location where the path start from 
     * \param endLocation Location where we want to go
     * \return The path to go from startingLocation to endLocation
     */
    std::vector<Location>* GetPathInstructions(Location startLocation, Location endLocation);
};

#endif // ASTAR_H_
