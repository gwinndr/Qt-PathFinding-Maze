#include "dijkstra.h"
#include <algorithm>

bool DijkstraNodeLessThan(const DijkstraNode & q1, const DijkstraNode & q2)
{
    return q1.mPathCost < q2.mPathCost;
}

Dijkstra::Dijkstra(QWidget *parent) : MazeStaticSolver (parent)
{

}

Dijkstra::~Dijkstra()
{

}

std::string Dijkstra::GetName()
{
    return "Dijkstra";
}

void Dijkstra::InitializeSolver()
{
    mPathCosts.clear();
    mNodeQueue.clear();
    mTrackedPaths.clear();

    MazeNode * start_node = GetNodeAt(mStartX, mStartY);
    mPathCosts[start_node] = 0;

    DijkstraNode new_node;
    new_node.mMazeNode = start_node;
    new_node.mPathCost = 0;
    mNodeQueue.push_back(new_node);
}

bool Dijkstra::SolveStep()
{
    bool return_value = false;

    if(!mNodeQueue.empty())
    {
        auto node_itr = std::min_element(mNodeQueue.begin(), mNodeQueue.end(), &DijkstraNodeLessThan);
        DijkstraNode next = *node_itr;
        mNodeQueue.erase(node_itr);

        int next_x, next_y;
        next.mMazeNode->GetMazeCoords(next_x, next_y);

        MazeNode * neighbor_array[4] = {GetNodeAt(next_x+1, next_y), GetNodeAt(next_x, next_y+1),
                                        GetNodeAt(next_x-1, next_y), GetNodeAt(next_x, next_y-1)};
        MazeNode * neighbor_temp = nullptr;
        for(int i = 0; i < 4; ++i)
        {
            neighbor_temp = neighbor_array[i];
            if(neighbor_temp != nullptr)
            {
                // Solution found!
                if(neighbor_temp->IsEnd())
                {
                    mPathCosts[neighbor_temp] = next.mPathCost + neighbor_temp->Visit();
                    mTrackedPaths[neighbor_temp] = next.mMazeNode;
                    return_value = true;
                    ConstructSolution(neighbor_temp);
                    break;
                }
                else if(!neighbor_temp->IsWall())
                {
                    int next_path_cost = next.mPathCost + neighbor_temp->Visit();

                    auto find_itr = mPathCosts.find(neighbor_temp);
                    if(find_itr == mPathCosts.end())
                    {
                        mPathCosts[neighbor_temp] = next_path_cost;
                        mTrackedPaths[neighbor_temp] = next.mMazeNode;

                        DijkstraNode new_node;
                        new_node.mMazeNode = neighbor_temp;
                        new_node.mPathCost = next_path_cost;
                        mNodeQueue.push_back(new_node);
                    }
                    else if(next_path_cost < find_itr->second)
                    {
                        mPathCosts[neighbor_temp] = next_path_cost;
                        mTrackedPaths[neighbor_temp] = next.mMazeNode;
                        UpdateQueue(neighbor_temp, next_path_cost);
                    }
                }
            }
        }
    }
    // No solution found
    else
    {
        return_value = true;
    }

    return return_value;
}

void Dijkstra::UpdateQueue(MazeNode* maze_node, int newPathCost)
{
    for(auto itr = mNodeQueue.begin(); itr != mNodeQueue.end(); ++itr)
    {
        if(itr->mMazeNode == maze_node)
        {
            itr->mPathCost = newPathCost;
            break;
        }
    }
}


void Dijkstra::ConstructSolution(MazeNode * node)
{
    if(!node->IsStart())
        ConstructSolution(mTrackedPaths.find(node)->second);

    mPath.push_back(node);
}











