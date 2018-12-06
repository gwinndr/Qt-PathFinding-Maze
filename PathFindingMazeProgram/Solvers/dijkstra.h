#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "mazestaticsolver.h"
#include <queue>
#include <map>
#include <limits>

struct DijkstraNode
{
    MazeNode* mMazeNode = nullptr;
    int mPathCost = INT_MAX;

    bool operator()(const DijkstraNode & n1, const DijkstraNode & n2)
    {
        return n1.mPathCost < n2.mPathCost;
    }
};

class Dijkstra : public MazeStaticSolver
{
public:
    explicit Dijkstra(QWidget *parent = nullptr);
    ~Dijkstra();

    virtual std::string GetName();

protected:
    std::vector<DijkstraNode> mNodeQueue;
    std::map<MazeNode *, int> mPathCosts;
    std::map<MazeNode*, MazeNode*> mTrackedPaths;

    virtual void InitializeSolver();

    virtual bool SolveStep();

    void UpdateQueue(MazeNode* maze_node, int new_val);

    void ConstructSolution(MazeNode * node);

private:

};

#endif // DIJKSTRA_H



