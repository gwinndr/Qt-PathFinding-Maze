#include "mazedynamicsolver.h"
#include <iostream>
#include <random>
#include <time.h>
#include <map>

MazeDynamicSolver::MazeDynamicSolver(QWidget *parent) : MazeStaticSolver(parent)
{

}

MazeDynamicSolver::~MazeDynamicSolver()
{

}

std::string MazeDynamicSolver::GetName()
{
    return "Maze Dynamic Solver";
}

bool MazeDynamicSolver::SolveOne()
{
    bool return_val = false;
    ++mSteps;
    StepsUpdate(mSteps);
    if(mEvent == MAZE_CHANGE_EVENT)
    {
        HandleChange(mChangedNodes);
        return_val = false;
        mEvent = NO_EVENT;
        mInChangedState = true;
    }
    else
    {
        // Steps may be different in adjustment period
        if(mInChangedState)
            return_val = SolveStepAfterChange();
        else
            return_val = SolveStep();

        if(return_val == true)
        {
            mEvent = StepThroughSolution();
            if(mEvent == MAZE_CHANGE_EVENT)
                return_val = false;
        }
    }

    return return_val;
}

bool MazeDynamicSolver::SolveAll()
{
    bool solution_found = false;
    if(SOLVE_ALL_STEP_LIM >= 0)
    {
        for(int i = 0; i < SOLVE_ALL_STEP_LIM; ++i)
        {
            solution_found = SolveOne();

            if(solution_found)
                break;
        }
    }
    else
    {
        while(!solution_found)
        {
            solution_found = SolveOne();
        }
    }

    return solution_found;
}


void MazeDynamicSolver::HandleChange(std::vector<MazeNode *> &)
{
    std::cout << "Must be overwridden!\n";
}

int MazeDynamicSolver::StepThroughSolution()
{
    int event = NO_EVENT;
    if(mTentativePath.size() > 1)
    {
        for(int i=0; i < STEPS_UNTIL_CHANGE; ++i)
        {
            MazeNode * next = mTentativePath[static_cast<std::size_t>(i)];
            mPath.push_back(next);
            next->SetPath(true);

            if(next->IsEnd())
            {
                event = SOLUTION_SUCCESS_EVENT;
                break;
            }

        }

        if(event == NO_EVENT)
        {
            event = MAZE_CHANGE_EVENT;
            MorphMaze();
            mTentativePath.clear();
        }
    }
    return event;
}

void MazeDynamicSolver::MorphMaze()
{
    mChangedNodes.clear();

    if(RANDOM_SEED < 0)
        srand(static_cast<unsigned int>(time(nullptr)));
    else
        srand(static_cast<unsigned int>(RANDOM_SEED));

    int x, y, seeder, node_val;
    bool already_changed, is_start_or_end, in_path;
    for(int i=0; i < NUM_NODES_TO_CHANGE; ++i)
    {
        x = rand() % mMazeXDim;
        y = rand() % mMazeYDim;
        MazeNode * node = GetNodeAt(x, y);
        already_changed = (std::find(mChangedNodes.begin(), mChangedNodes.end(), node) != mChangedNodes.end());
        is_start_or_end = (x == mStartX && y == mStartY) || (x == mEndX && y == mEndY);
        in_path = std::find(mPath.begin(), mPath.end(), node) != mPath.end();
        if(!already_changed && !is_start_or_end && !in_path)
        {
            mChangedNodes.push_back(node);
            seeder = rand() % 2;
            node_val = node->Visit(false);
            if(seeder == 0)
            {
                if(node_val > 1)
                    node->SetMoveCost(node_val-2);
                else if(node->IsWall())
                    node->SetMoveCost(4);
                else
                    node->SetMoveCost(0);
            }
            else
            {
                if(node->IsWall() || node_val > 3)
                    node->SetWall(true);
                else
                    node->SetMoveCost(node_val+2);
            }
        }
    }
}

bool MazeDynamicSolver::SolveStepAfterChange()
{
    std::cout << "Must be overwridden!\n";
    return false;
}



