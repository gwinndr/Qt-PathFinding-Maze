#ifndef ASTAR_H
#define ASTAR_H
#include <mazestaticsolver.h>

struct AStarNode
{
    MazeNode* mMazeNode = nullptr;
    int mPathCost = INT_MAX;
    int mHeuristicsCost = INT_MAX;

    bool operator()(const AStarNode & n1, const AStarNode & n2)
    {
        return (n1.mPathCost + n1.mHeuristicsCost) < (n2.mPathCost + n2.mHeuristicsCost);
    }
};

class AStar : public MazeStaticSolver
{
public:
    explicit AStar(QWidget *parent = nullptr);
    ~AStar();

    virtual std::string GetName();

protected:
    std::vector<AStarNode*> mHeapAllocations;
    std::vector<AStarNode *> mOpenSet;
    std::map<MazeNode *, AStarNode *> mNodeMap;
    std::map<MazeNode*, MazeNode*> mTrackedPaths;

    virtual void InitializeSolver();

    virtual bool SolveStep();

    void UpdateTrackedNeighbors(MazeNode * mazeNode, const int & newCost);

    void ConstructSolution(MazeNode * node);

    int HeuristicsValue(MazeNode * node);

    AStarNode * AllocateNewNode();

    // The lower the value, the more like Dijkstra
    // The higher the value, the more like greedy
    // 2.5 is about the average of each node
    static const int NODE_HEURISTICS_COST = 2;
    //static const int NODE_HEURISTICS_COST = 2000;

private:

};

#endif // ASTAR_H









