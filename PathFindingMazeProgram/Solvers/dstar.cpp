#include "dstar.h"

bool DStarNodeLessThan(const DStarNode * q1, const DStarNode * q2)
{
    int q1_val, q2_val;
    if(q1->mPathCost == INT_MAX || q1->mHeuristicsCost == INT_MAX)
        q1_val = INT_MAX;
    else
        q1_val = (q1->mPathCost + q1->mHeuristicsCost);

    if(q2->mPathCost == INT_MAX || q2->mHeuristicsCost == INT_MAX)
            q2_val = INT_MAX;
    else
        q2_val = (q2->mPathCost + q2->mHeuristicsCost);

    return q1_val < q2_val;
}

DStar::DStar(QWidget *parent) : MazeDynamicSolver (parent)
{

}

DStar::~DStar()
{
}

std::string DStar::GetName()
{
    return "DStar";
}

void DStar::InitializeSolver()
{
    mHeapAllocations.clear();
    mOpenSet.clear();
    mNodeMap.clear();
    mBackwardPath.clear();

    // We go from end to start in D*
    MazeNode * end_node = GetNodeAt(mEndX, mEndY);
    mCurrentStartNode = GetNodeAt(mStartX, mStartY);

    DStarNode * new_node = AllocateNewNode();
    new_node->mMazeNode = end_node;
    new_node->mPathCost = 0;
    new_node->mLowestPath = 0;
    new_node->mHeuristicsCost = HeuristicsValue(end_node);
    new_node->mState = DStarNode::OPEN;
    mOpenSet.push_back(new_node);
    mNodeMap[end_node] = new_node;
}

bool DStar::SolveStep()
{
    bool return_value = false;

    int k_min = ProcessState();
    if(k_min == -1)
        return_value = true;
    else
    {
        MazeNode * start = GetNodeAt(mStartX, mStartY);
        auto find_itr = mNodeMap.find(start);
        if(find_itr != mNodeMap.end())
        {
            if(find_itr->second->mState == DStarNode::CLOSED)
            {
                return_value = true;
                ConstructSolution(find_itr->second);
            }
        }
    }

    return return_value;
}

bool DStar::SolveStepAfterChange()
{
    bool return_value = false;

    int k_min = ProcessState();
    if(k_min == -1 || k_min >= mCurrentDStar->mPathCost)
    {
        return_value = true;
        MazeNode * start = mCurrentStartNode;
        auto find_itr = mNodeMap.find(start);
        if(find_itr != mNodeMap.end())
        {
            ConstructSolution(find_itr->second);
        }
    }

    return return_value;
}

void DStar::Insert(DStarNode * node, const int & hVal)
{
    node->mState = DStarNode::OPEN;
    node->mPathCost = hVal;
    node->mLowestPath = std::min(node->mLowestPath, hVal);

    mOpenSet.push_back(node);
    //mNodeMap[node->mMazeNode] = node;
}

int DStar::ProcessState()
{
    int k_min = -1;

    if(!mOpenSet.empty())
    {
        auto node_itr = std::min_element(mOpenSet.begin(), mOpenSet.end(), &DStarNodeLessThan);
        DStarNode * next = *node_itr;
        mOpenSet.erase(node_itr);
        int k_old = next->mLowestPath;
        int actual_cost = next->mPathCost;
        std::vector<DStarNode*> neighbors = GetNeighbors(next);
        next->mState = DStarNode::CLOSED;

        // The cost is higher compared to the minimum it used to be
        if(k_old < actual_cost)
        {
            ProcessKLessThan(next, neighbors, k_old, actual_cost);
        }
        // The cost is the same as the minimum cost it was
        else if(k_old == actual_cost)
        {
            ProcessKEqual(next, neighbors, k_old, actual_cost);
        }
        // New cost is less than the minimum
        else
        {
            ProcessKGreaterThan(next, neighbors, k_old, actual_cost);
        }

        k_min = next->mLowestPath;
    }

    return k_min;
}

void DStar::ProcessKLessThan(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost)
{
    int next_node_cost = (next->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (next->mMazeNode->Visit());
    int neighbor_node_cost = -1;

    for(DStarNode * neighbor : neighbors)
    {
        neighbor_node_cost = (neighbor->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (neighbor->mMazeNode->Visit());

        if(neighbor->mState != DStarNode::NEW)
        {
            int true_move_cost = neighbor->mPathCost + next_node_cost;
            if(neighbor->mPathCost <= kOld && actualCost > true_move_cost)
            {
                mBackwardPath[next->mMazeNode] = neighbor->mMazeNode;
                next->mPathCost = true_move_cost;
            }
        }
    }
}

void DStar::ProcessKEqual(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost)
{
    //int next_node_cost = (next->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (next->mMazeNode->Visit());
    int neighbor_node_cost = -1;

    for(DStarNode * neighbor : neighbors)
    {
        bool reqs_satisfied = false;
        neighbor_node_cost = (neighbor->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (neighbor->mMazeNode->Visit());

        if(neighbor->mState == DStarNode::NEW)
            reqs_satisfied = true;
        else
        {
            bool backpointer_to_next = neighbor->mBackPointer == next;
            bool y_inconsistent = neighbor->mPathCost != next->mPathCost + neighbor_node_cost;
            bool y_better_path = neighbor->mPathCost > next->mPathCost + neighbor_node_cost;

            reqs_satisfied = (backpointer_to_next && y_inconsistent) || (!backpointer_to_next && y_better_path);
        }

        if(reqs_satisfied)
        {
            neighbor->mBackPointer = next;
            Insert(neighbor, next->mPathCost + neighbor_node_cost);
        }
    }
}

void DStar::ProcessKGreaterThan(DStarNode * next, std::vector<DStarNode*> neighbors, const int & kOld, const int & actualCost)
{
    int next_node_cost = (next->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (next->mMazeNode->Visit());
    int neighbor_node_cost = -1;

    for(DStarNode * neighbor : neighbors)
    {
        neighbor_node_cost = (neighbor->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : (neighbor->mMazeNode->Visit());

        bool reqs_satisfied = false;
        bool backpointer_to_next = false;
        bool y_inconsistent = false;
        bool y_better_path = false;
        bool x_better_path = false;

        if(neighbor->mState == DStarNode::NEW)
            reqs_satisfied = true;
        else
        {
            backpointer_to_next = neighbor->mBackPointer == next;
            y_inconsistent = neighbor->mPathCost != next->mPathCost + neighbor_node_cost;
            y_better_path = neighbor->mPathCost > next->mPathCost + neighbor_node_cost;
            x_better_path = next->mPathCost > neighbor->mPathCost + next_node_cost;

            reqs_satisfied = (backpointer_to_next && y_inconsistent);
        }

        if(reqs_satisfied)
        {
            neighbor->mBackPointer = next;
            Insert(neighbor, next->mPathCost + neighbor_node_cost);
        }
        else if(!backpointer_to_next && y_better_path)
        {
            Insert(next, next->mPathCost);
        }
        else if(!backpointer_to_next && x_better_path && neighbor->mState == DStarNode::CLOSED && neighbor->mPathCost > kOld)
        {
            Insert(neighbor, neighbor->mPathCost);
        }
    }


}

void DStar::HandleChange(std::vector<MazeNode *> & nodesChanged)
{
    mCurrentStartNode = mPath.back();
    mCurrentDStar = mNodeMap.find(mCurrentStartNode)->second;
    for(DStarNode * node : mHeapAllocations)
        node->mHeuristicsCost = HeuristicsValue(node->mMazeNode);

    for(MazeNode * node : nodesChanged)
    {

        auto find_itr = mNodeMap.find(node);
        DStarNode * dstar_node;
        if(find_itr == mNodeMap.end())
        {
            dstar_node = AllocateNewNode();
            dstar_node->mHeuristicsCost = HeuristicsValue(node);
            dstar_node->mMazeNode = node;
            mNodeMap[node] = dstar_node;
        }
        else
            dstar_node = find_itr->second;

        if(dstar_node->mState != DStarNode::OPEN)
            Insert(dstar_node, dstar_node->mLowestPath);

        std::vector<DStarNode*> neighbors = GetNeighbors(dstar_node);
        int node_cost = (dstar_node->mMazeNode->IsWall()) ? (DStarNode::HIGH_INT_CONSTANT) : dstar_node->mMazeNode->Visit();
        for(DStarNode * neighbor : neighbors)
        {
            if(neighbor->mBackPointer == dstar_node)
            {
                neighbor->mPathCost = dstar_node->mPathCost + node_cost;
            }
            if(neighbor->mState != DStarNode::OPEN)
                Insert(neighbor, neighbor->mLowestPath);
        }

       //dstar_node;
       //if(dstar_node->mState == DStarNode::CLOSED)
            //Insert(dstar_node, )
    }
}

void DStar::UpdateTrackedNeighbors(MazeNode * mazeNode, const int & newCost)
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
                if(mBackwardPath[neighbor] == mazeNode)
                {
                    int neighbor_cost = newCost + neighbor->Visit();
                    mNodeMap[neighbor]->mPathCost = neighbor_cost;
                    UpdateTrackedNeighbors(neighbor, neighbor_cost);
                }
            }
        }

    }
}

void DStar::ConstructSolution(DStarNode * node)
{
    mTentativePath.clear();
    while(!node->mMazeNode->IsEnd())
    {
        if(node->mMazeNode->IsWall())
        {
            mTentativePath.clear();
            return;
        }
        else if(node->mBackPointer == nullptr)
        {
            mTentativePath.clear();
            return;
        }
        else
        {
            mTentativePath.push_back(node->mMazeNode);
            node = node->mBackPointer;
        }
    }
    mTentativePath.push_back(node->mMazeNode);
}

int DStar::HeuristicsValue(MazeNode * node)
{
    int x, y;
    node->GetMazeCoords(x, y);

    int start_x, start_y;
    mCurrentStartNode->GetMazeCoords(start_x, start_y); // This will ensure heuristics always brings the end to our current start
    return (GetManhattanDistance(x, y, mStartX, mStartY) * NODE_HEURISTICS_COST);
}

DStarNode * DStar::AllocateNewNode()
{
    DStarNode * new_node = new DStarNode;
    mHeapAllocations.push_back(new_node);
    return new_node;
}

std::vector<DStarNode *> DStar::GetNeighbors(DStarNode * node)
{
    std::vector<DStarNode *> neighbors;
    int x, y;
    node->mMazeNode->GetMazeCoords(x, y);
    std::vector<MazeNode*> neighbors_maze;

    neighbors_maze.push_back(GetNodeAt(x+1, y));
    neighbors_maze.push_back(GetNodeAt(x, y+1));
    neighbors_maze.push_back(GetNodeAt(x-1, y));
    neighbors_maze.push_back(GetNodeAt(x, y-1));

    for(MazeNode * neighbor_maze : neighbors_maze)
    {
        if(neighbor_maze != nullptr)
        {
            DStarNode * neighbor_node;
            auto find_itr = mNodeMap.find(neighbor_maze);
            if(find_itr == mNodeMap.end())
            {
                neighbor_node = AllocateNewNode();
                neighbor_node->mMazeNode = neighbor_maze;
                neighbor_node->mHeuristicsCost = HeuristicsValue(neighbor_maze);
                mNodeMap[neighbor_maze] = neighbor_node;
            }
            else
                neighbor_node = find_itr->second;

            neighbors.push_back(neighbor_node);
        }
    }

    return neighbors;
}










