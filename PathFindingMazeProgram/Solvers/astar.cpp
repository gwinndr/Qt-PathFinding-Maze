#include "astar.h"

bool AStarNodeLessThan(const AStarNode * q1, const AStarNode * q2)
{
    return (q1->mPathCost + q1->mHeuristicsCost) < (q2->mPathCost + q2->mHeuristicsCost);
}

AStar::AStar(QWidget *parent) : MazeStaticSolver (parent)
{

}

AStar::~AStar()
{
    for(auto itr = mHeapAllocations.begin(); itr != mHeapAllocations.end(); ++itr)
        delete *itr;
    mHeapAllocations.clear();
}

std::string AStar::GetName()
{
    return "AStar";
}

void AStar::InitializeSolver()
{
    mHeapAllocations.clear();
    mOpenSet.clear();
    mNodeMap.clear();
    mTrackedPaths.clear();

    MazeNode * start_node = GetNodeAt(mStartX, mStartY);

    AStarNode * new_node = AllocateNewNode();
    new_node->mMazeNode = start_node;
    new_node->mPathCost = 0;
    new_node->mHeuristicsCost = HeuristicsValue(start_node);
    mOpenSet.push_back(new_node);
    mNodeMap[start_node] = new_node;
}

bool AStar::SolveStep()
{
    bool return_value = false;

    if(!mOpenSet.empty())
    {
        auto node_itr = std::min_element(mOpenSet.begin(), mOpenSet.end(), &AStarNodeLessThan);
        AStarNode * next = *node_itr;
        mOpenSet.erase(node_itr);

        int next_x, next_y;
        next->mMazeNode->GetMazeCoords(next_x, next_y);

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
                    mTrackedPaths[neighbor_temp] = next->mMazeNode;
                    return_value = true;
                    ConstructSolution(neighbor_temp);
                    break;
                }
                else if(!neighbor_temp->IsWall())
                {
                    int next_path_cost = next->mPathCost + neighbor_temp->Visit();

                    auto find_itr = mNodeMap.find(neighbor_temp);
                    if(find_itr == mNodeMap.end())
                    {
                        mTrackedPaths[neighbor_temp] = next->mMazeNode;

                        AStarNode * new_node = AllocateNewNode();
                        new_node->mMazeNode = neighbor_temp;
                        new_node->mPathCost = next_path_cost;
                        new_node->mHeuristicsCost = HeuristicsValue(neighbor_temp);
                        mOpenSet.push_back(new_node);
                        mNodeMap[neighbor_temp] = new_node;
                    }
                    else if(next_path_cost < find_itr->second->mPathCost)
                    {
                        mTrackedPaths[neighbor_temp] = next->mMazeNode;
                        mNodeMap[neighbor_temp]->mPathCost = next_path_cost;
                        UpdateTrackedNeighbors(neighbor_temp, next_path_cost);
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

void AStar::UpdateTrackedNeighbors(MazeNode * mazeNode, const int & newCost)
{
    int x, y;
    mazeNode->GetMazeCoords(x, y);

    MazeNode * neighbor_array[4] = {GetNodeAt(x+1, y), GetNodeAt(x, y+1),
                                    GetNodeAt(x-1, y), GetNodeAt(x, y-1)};
    for(int i = 0; i < 4; ++i)
    {
        MazeNode * neighbor = neighbor_array[i];
        if(neighbor != nullptr)
        {
            if(neighbor->IsVisited())
            {
                if(mTrackedPaths[neighbor] == mazeNode)
                {
                    int neighbor_cost = newCost + neighbor->Visit();
                    mNodeMap[neighbor]->mPathCost = neighbor_cost;
                    UpdateTrackedNeighbors(neighbor, neighbor_cost);
                }
            }
        }

    }
}

void AStar::ConstructSolution(MazeNode * node)
{
    if(!node->IsStart())
        ConstructSolution(mTrackedPaths.find(node)->second);

    mPath.push_back(node);
}

int AStar::HeuristicsValue(MazeNode * node)
{
    int x, y;
    node->GetMazeCoords(x, y);
    return (GetManhattanDistance(x, y, mEndX, mEndY) * NODE_HEURISTICS_COST);
}

AStarNode * AStar::AllocateNewNode()
{
    AStarNode * new_node = new AStarNode;
    mHeapAllocations.push_back(new_node);
    return new_node;
}









