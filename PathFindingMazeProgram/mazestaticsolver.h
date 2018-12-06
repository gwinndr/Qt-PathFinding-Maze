#ifndef MAZESTATICSOLVER_H
#define MAZESTATICSOLVER_H

#include <vector>
#include <map>
#include "mazearea.h"

class MazeStaticSolver : public QWidget
{
    Q_OBJECT
public:
    explicit MazeStaticSolver(QWidget *parent = nullptr);
    virtual ~MazeStaticSolver();

    void Initialize(MazeArea * mazeArea, MazeNode * startNode, MazeNode * endNode);

    virtual bool SolveOne();

    virtual bool SolveAll();

    int GetSolution();

    // Must be implemented by derived class
    virtual std::string GetName();

protected:
    std::vector<MazeNode*> mPath;
    int mStartX = -1;
    int mStartY = -1;
    int mEndX = -1;
    int mEndY = -1;

    std::size_t mMazeSize = 0;
    int mMazeXDim = 0;
    int mMazeYDim = 0;

    MazeNode * GetNodeAt(const int & x, const int & y);

    int GetManhattanDistance(const int & x1, const int & y1, const int & x2, const int & y2);

    // Must be implemented by derived class
    virtual void InitializeSolver();
    virtual bool SolveStep();

    const int SOLVE_ALL_STEP_LIM = 100; // After n steps, the automatic solver will stop to avoid inf loops, put -1 for no limit
    int mSteps = 0;

private:
    MazeArea * mMazeArea = nullptr;

signals:
    void StepsUpdate(const int & numSteps);
};

#endif // MAZESTATICSOLVER_H





