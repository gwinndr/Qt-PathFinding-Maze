#include "mazestaticsolver.h"
#include <iostream>

MazeStaticSolver::MazeStaticSolver(QWidget *parent) : QWidget (parent),
    mPath()
{

}

MazeStaticSolver::~MazeStaticSolver()
{

}

void MazeStaticSolver::Initialize(MazeArea * mazeArea, MazeNode * startNode, MazeNode * endNode)
{
    mMazeArea = mazeArea;
    startNode->GetMazeCoords(mStartX, mStartY);
    endNode->GetMazeCoords(mEndX, mEndY);
    mMazeSize = mMazeArea->Size();
    mMazeXDim = mMazeArea->XDim();
    mMazeYDim = mMazeArea->YDim();
    mPath.clear();
    mSteps = 0;
    InitializeSolver();
}

bool MazeStaticSolver::SolveOne()
{
    ++mSteps;
    StepsUpdate(mSteps);
    return SolveStep();
}

bool MazeStaticSolver::SolveAll()
{
    bool solution_found = false;
    if(SOLVE_ALL_STEP_LIM >= 0)
    {
        for(int i = 0; i < SOLVE_ALL_STEP_LIM; ++i)
        {
            ++mSteps;
            solution_found = SolveStep();

            if(solution_found)
                break;
        }
    }
    else
    {
        while(!solution_found)
        {
            ++mSteps;
            solution_found = SolveStep();
        }
    }
    StepsUpdate(mSteps);
    return solution_found;
}

int MazeStaticSolver::GetManhattanDistance(const int & x1, const int & y1, const int & x2, const int & y2)
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int MazeStaticSolver::GetSolution()
{
    int full_path_cost = 0;
    for(auto itr = mPath.begin(); itr != mPath.end(); ++itr)
    {
        (*itr)->SetPath(true);
        full_path_cost += (*itr)->Visit();
    }
    return full_path_cost;
}

MazeNode * MazeStaticSolver::GetNodeAt(const int & x, const int & y)
{
    MazeNode * return_node = nullptr;
    if(mMazeArea != nullptr && x >= 0 && x < mMazeXDim && y >= 0 && y < mMazeYDim)
        return_node = mMazeArea->GetNode(static_cast<std::size_t>(x), static_cast<std::size_t>(y));

    return return_node;
}

std::string MazeStaticSolver::GetName()
{
    return "MazeStaticSolver\n";
}

void MazeStaticSolver::InitializeSolver()
{
    std::cout << "InitializeSolver must be overwridden!\n";
}

bool MazeStaticSolver::SolveStep()
{
    std::cout << "SolveStep must be overwridden!\n";
    return false;
}






