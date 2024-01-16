#ifndef ASTAR_H_
#define ASTAR_H_
#pragma once
#include <vector>
#include "State.h"
#include "Location.h"
#include "AStarNode.h"
#include <stack>
// #include "Direction.h"

class AStar
{
    /**
     * \brief the state of the game, used to get grid in functions
     */
    State *_pGameState;
    
    /**
     * \brief Calculate estimated cost to go from one location to another
     * \param pGivenLocation current location to search heuristic from
     * \param pEndLocation End location that must be found
     * \return Estimated cost to go from one another
     */
    double HeuristicLocationCalculator(const Location *pGivenLocation, const Location *pEndLocation) const;

    /**
     * \brief Check neighbors from baseLocation, and add them to uncheckedNodes if they're not already in it or
     * in checkedNodes
     * \param pBaseLocation Location to check neighbors from
     * \param pUncheckedNodes List of nodes who were not checked
     * \param pCheckedNodes List of nodes who were checked
     */
    void AddNeighborsToList(AStarNode *pBaseLocation, std::vector<AStarNode*> *pUncheckedNodes,
                            std::vector<AStarNode*> *pCheckedNodes) const;

    /**
     * \brief Create path from linked AStarNodes
     * \param pEndNode last node used 
     * \return path to the location
     */
    static std::vector<DIRECTION>* CreatePathDirections(const AStarNode *pEndNode);

    /**
     * \brief Get NextNode from list
     * \param pUncheckedNodes list of possible next nodes
     * \param pEndLocation Location targeted
     * \return the next node to be checked
     */
    AStarNode* GetNextNode(std::vector<AStarNode *> *pUncheckedNodes, Location *pEndLocation) const;

    /**
     * \brief Get The Best Node out of the unchecked node list
     * \param pUncheckedNodes list of unchecked node
     * \param pEndLocation end location targeted
     * \return the node to be processed next
     */
    AStarNode* GetBestNodeForPath(std::vector<AStarNode*> *pUncheckedNodes, const Location *pEndLocation) const;


public:
 
    /**
     * \brief Constructor to start the Astar calculator
     * \param pState game State, needed because of grid calculations
     */
    explicit AStar(State *pState);
 
    /**
     * \brief 
     * \param startLocation Location where the path start from 
     * \param endLocation Location where we want to go
     * \return The path to go from startingLocation to endLocation
     */
    std::vector<DIRECTION>* GetPathInstructionsDirection(Location startLocation, Location endLocation) const;
};

#endif // ASTAR_H_
