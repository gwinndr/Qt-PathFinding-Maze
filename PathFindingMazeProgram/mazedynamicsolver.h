#ifndef MAZEDYNAMICSOLVER_H
#define MAZEDYNAMICSOLVER_H

#include "mazestaticsolver.h"
#include <vector>
#include "mazenode.h"

class MazeDynamicSolver : public MazeStaticSolver
{
public:
    explicit MazeDynamicSolver(QWidget *parent = nullptr);
    virtual ~MazeDynamicSolver();

    //void SetChangePercent(const int & percentMaze);

    virtual bool SolveOne();

    virtual bool SolveAll();

    int GetSolution();

    // Must be implemented by derived class
    virtual std::string GetName();

protected:
    const static int RANDOM_SEED            = -1; // For reproducibility. Use -1 for time(0)
    const static int NUM_NODES_TO_CHANGE    = 10;
    const static int STEPS_UNTIL_CHANGE     = 3;

    //int mChangePercent = 0;
    //bool mInChangeState = false;
    std::vector<MazeNode *> mTentativePath;

    virtual void HandleChange(std::vector<MazeNode *> & changedNodes);

    virtual bool SolveStepAfterChange();

private:
    int StepThroughSolution();

    void MorphMaze();

    static const int NO_EVENT = 0;
    static const int MAZE_CHANGE_EVENT = 1;
    static const int SOLUTION_SUCCESS_EVENT = 2;
    int mEvent = NO_EVENT;
    std::vector<MazeNode *> mChangedNodes;
    bool mInChangedState = false;

};

#endif // MAZEDYNAMICSOLVER_H
