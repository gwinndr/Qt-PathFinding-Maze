#ifndef DSTAR_H
#define DSTAR_H

#include "mazedynamicsolver.h"
#include <QWidget>
#include <limits>

struct DStarNode
{
    MazeNode* mMazeNode = nullptr;
    int mPathCost = DStarNode::HIGH_INT_CONSTANT;
    int mHeuristicsCost = DStarNode::HIGH_INT_CONSTANT;
    int mLowestPath = DStarNode::HIGH_INT_CONSTANT;
    int mState = DStarNode::NEW;
    DStarNode * mBackPointer = nullptr;

    static const int HIGH_INT_CONSTANT = 100000;
    const static int NEW = 0;
    const static int OPEN = 1;
    const static int CLOSED = 2;
};

class DStar : public MazeDynamicSolver
{
public:
    explicit DStar(QWidget *parent = nullptr);
    ~DStar();

    virtual std::string GetName();

protected:
    std::vector<DStarNode*> mHeapAllocations;
    std::vector<DStarNode *> mOpenSet;
    std::vector<DStarNode*> mChangedSet;
    std::map<MazeNode *, DStarNode *> mNodeMap;
    std::map<MazeNode *, MazeNode*> mBackwardPath;
    MazeNode * mCurrentStartNode = nullptr;
    DStarNode * mCurrentDStar = nullptr;

    virtual void InitializeSolver();
    virtual bool SolveStep();
    virtual bool SolveStepAfterChange();

    int ProcessState();

    void ProcessKLessThan(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost);

    void ProcessKEqual(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost);

    void ProcessKGreaterThan(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost);

    void PrepRepair();

    void Insert(DStarNode * node, const int & hVal);

    virtual void HandleChange(std::vector<MazeNode *> & nodesChanged);

    void UpdateTrackedNeighbors(MazeNode * mazeNode, const int & newCost);

    void ConstructSolution(DStarNode * node);

    int HeuristicsValue(MazeNode * node);

    std::vector<DStarNode *> GetNeighbors(DStarNode * node);

    DStarNode * AllocateNewNode();

    // The lower the value, the more like Dijkstra
    // The higher the value, the more like greedy
    // 2.5 is about the average of each node
    static const int NODE_HEURISTICS_COST = 2;
    //static const int NODE_HEURISTICS_COST = 2000;

private:

};

#endif // DSTAR_H
