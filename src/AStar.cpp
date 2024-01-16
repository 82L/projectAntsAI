#include "AStar.h"


AStar::AStar(State *pState)
{
    _pGameState = pState;
}

std::vector<DIRECTION>* AStar::GetPathInstructionsDirection(Location startLocation, Location endLocation) const
{
    std::vector<AStarNode*> *pUncheckedNodes = new std::vector<AStarNode *>();
    AStarNode *pStartNode = new AStarNode(startLocation, nullptr, 0, DIRECTION::N);
    pUncheckedNodes->push_back(pStartNode);
    std::vector<AStarNode*> *pCheckedNodes = new std::vector<AStarNode *>();
    AStarNode *pEndNode = nullptr;
    while(!pUncheckedNodes->empty() && pEndNode == nullptr)
    {
        AStarNode *pNodeToCheck = GetNextNode(pUncheckedNodes, &endLocation);
        if(pNodeToCheck->location == endLocation)
        {
            pEndNode = pNodeToCheck;
            break;
        }
        AddNeighborsToList(pNodeToCheck, pUncheckedNodes, pCheckedNodes);
        pCheckedNodes->push_back(pNodeToCheck);
    }
    // return nullptr;

    if(pEndNode != nullptr)
    {
        std::vector<DIRECTION> *pPath = CreatePathDirections(pEndNode);
        pUncheckedNodes->clear();
        pCheckedNodes->clear();
        return pPath;
    }
    return nullptr;
    //do the path
}

double AStar::HeuristicLocationCalculator(const Location *pGivenLocation, const Location *pEndLocation) const
{
   return _pGameState->GetWrappedDistance(*pGivenLocation, *pEndLocation);
}

void AStar::AddNeighborsToList(AStarNode *pBaseLocation, std::vector<AStarNode*> *pUncheckedNodes,  std::vector<AStarNode*> *pCheckedNodes) const
{
    for(const auto& direction : DIRECTIONS)
    {
        Location foundLocation = _pGameState->GetLocation(pBaseLocation->location, direction);
        if(_pGameState->grid[foundLocation.row][foundLocation.col].CheckSquareIsValidCalculation())
        {
            if(std::find_if(pCheckedNodes->begin(), pCheckedNodes->end(), [&](const AStarNode *pNode)
            {
                return pNode->location == foundLocation;
            }) == pCheckedNodes->end())
            {
                AStarNode *pNewNode = new AStarNode(foundLocation,pBaseLocation, pBaseLocation->cost + 1, direction);
                auto iFoundNode = find_if( pUncheckedNodes->begin(), pUncheckedNodes->end(), [&](const AStarNode *pNode)
                {
                    return pNode->location == pNewNode->location;
                });
                if(iFoundNode != pUncheckedNodes->end())
                {
                    if((*iFoundNode)->cost > pNewNode->cost )
                    {
                        pUncheckedNodes->erase(iFoundNode);
                        pUncheckedNodes->push_back(pNewNode);
                    }
                    else
                    {
                        delete pNewNode;
                    }
                }else
                {
                    pUncheckedNodes->push_back(pNewNode);
                }
            }
        }
    }
}

std::vector<DIRECTION>* AStar::CreatePathDirections(const AStarNode *pEndNode)
{
    const AStarNode* pCurrentNode = pEndNode;
    std::vector<DIRECTION> *pPath = new std::vector<DIRECTION>();
    do {
        pPath->push_back(pCurrentNode->direction);
        pCurrentNode = pCurrentNode->pPreviousNode;
    } while(pCurrentNode->pPreviousNode != nullptr);

    return pPath;
}

AStarNode* AStar::GetNextNode(std::vector<AStarNode*> *pUncheckedNodes, Location *pEndLocation) const
{
    auto iEndNode = std::find_if(pUncheckedNodes->begin(), pUncheckedNodes->end(),
                                      [&](AStarNode *pNode)
                                      {
                                          return pNode->location == *pEndLocation;
                                      });
    if( iEndNode != pUncheckedNodes->end())
    {
        return *iEndNode;
    }
    return GetBestNodeForPath(pUncheckedNodes, pEndLocation);
}


AStarNode* AStar::GetBestNodeForPath(std::vector<AStarNode*> *pUncheckedNodes, const Location *pEndLocation) const
{
    std::sort(pUncheckedNodes->begin(), pUncheckedNodes->end(),[&](const AStarNode *pNode1, const AStarNode *pNode2)
                                              {
                                                  return (pNode1->cost + HeuristicLocationCalculator(&pNode1->location, pEndLocation))
                                                      >  (pNode2->cost + HeuristicLocationCalculator(&pNode2->location, pEndLocation));
                                              } );
    AStarNode *pReturnNode  = pUncheckedNodes->back();
    pUncheckedNodes->pop_back();
    // pUncheckedNodes->erase(std::remove(pUncheckedNodes->begin(), pUncheckedNodes->end(), returnNode), pUncheckedNodes->end());
    return pReturnNode;
}
