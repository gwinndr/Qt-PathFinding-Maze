#include "mazearea.h"
#include "mazenode.h"
#include <QVBoxLayout>
#include <QFont>
#include <random>
#include <time.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <QMessageBox>
#include <QDebug>
#include "mazewindowwidget.h"

MazeArea::MazeArea(QWidget *parent) : QWidget(parent),
    mMazeGrid(new QGridLayout(this)),
    mMaze()
{
    setLayout(mMazeGrid);
    mMazeGrid->setAlignment(Qt::AlignCenter);
}

MazeArea::~MazeArea()
{
    delete mMazeGrid;
    DeallocateMaze();
}

void MazeArea::DeallocateMaze()
{
    for(auto itr = mMaze.begin(); itr != mMaze.end(); ++itr)
    {
        mMazeGrid->removeWidget(*itr);
        delete *itr;
    }
    mMaze.clear();
    mXDim = 0;
    mYDim = 0;
}

MazeArea * MazeArea::GetCopy(QWidget *parent)
{
    MazeArea * maze_copy = new MazeArea(parent);
    maze_copy->mXDim = mXDim;
    maze_copy->mYDim = mYDim;
    for(std::size_t y = 0; y < mYDim; ++y)
    {
        for(std::size_t x = 0; x < mXDim; ++x)
        {
            MazeNode * node_original = GetNode(x, y);
            MazeNode * node_copy = new MazeNode(maze_copy);
            node_copy->SetWidthHeight(MAZE_SQUARE_WIDTH);
            node_copy->SetLabelFontSize(MAZE_FONT_SIZE);

            if(node_original->IsEnd())
            {
               node_copy->SetEnd(true);
               node_copy->SetMoveCost(0);
               maze_copy->mEndNode = node_copy;
            }
            else if (node_original->IsStart())
            {
                node_copy->SetStart(true);
                node_copy->SetMoveCost(0);
                maze_copy->mStartNode = node_copy;
            }
            else
            {
                node_copy->SetMoveCost(node_original->Visit());
                node_original->ResetVisitStatus();
            }
            node_copy->SetPath(node_original->IsPath());
            node_copy->SetMazeCoords(x, y);

            maze_copy->mMaze.push_back(node_copy);
            maze_copy->mMazeGrid->addWidget(node_copy, y, x);
        }
    }

    return maze_copy;
}

MazeNode * MazeArea::GetNode(const std::size_t & x, const std::size_t y)
{
    MazeNode * return_value = nullptr;
    if(x < mXDim && y < mYDim)
    {
        std::size_t index = y * mXDim + x;
        return_value = mMaze[index];
    }
    return return_value;
}

void MazeArea::CreateRandomMaze(const int & xDim, const int & yDim, const std::size_t percentWall)
{
    DeallocateMaze();
    std::srand(static_cast<unsigned int>(time(nullptr)));
    std::size_t rand_output;
    mXDim = static_cast<std::size_t>(xDim);
    mYDim = static_cast<std::size_t>(yDim);

    for(int y = 0; y < yDim; ++y)
    {
        for(int x = 0; x < xDim; ++x)
        {
            MazeNode* new_node = new MazeNode(this);
            new_node->SetWidthHeight(MAZE_SQUARE_WIDTH);
            new_node->SetLabelFontSize(MAZE_FONT_SIZE);
            if(x == 0 && y == 0)
            {
                new_node->SetStart(true);
                new_node->SetMoveCost(0);
                mStartNode = new_node;
            }
            else if (x == xDim-1 && y == yDim-1)
            {
                new_node->SetEnd(true);
                new_node->SetMoveCost(0);
                mEndNode = new_node;
            }
            else
            {
                rand_output = (std::rand() % 100) + 1;
                if(rand_output > percentWall)
                {
                    // Will do move costs of 0 -> 5
                    rand_output = std::rand() % 6;
                    new_node->SetMoveCost(static_cast<int>(rand_output));
                }
            }
            new_node->SetMazeCoords(x, y);
            mMaze.push_back(new_node);
            mMazeGrid->addWidget(new_node, y, x);
        }
    }
}

void MazeArea::SaveMaze(const std::string & filePath)
{
    std::ofstream output_stream(filePath);
    if(!output_stream)
    {
        QMessageBox::critical(
          this,
          tr("Saving Maze"),
          tr(("Error: Could not save to requested path:\n" + filePath).c_str()) );
    }

    else
    {
        output_stream << mXDim << ' ' << mYDim << '\n';
        for(std::size_t j = 0; j < mYDim; ++j)
        {
            for(std::size_t i = 0; i < mXDim; ++i)
            {
                MazeNode * node = GetNode(i, j);
                std::string val;
                if(node->IsStart())
                    val = "s";
                else if(node->IsEnd())
                    val = "t";
                else if(node ->IsWall())
                    val = "w";
                else
                    val = std::to_string(node->Visit(false));

                output_stream << val << ' ';
            }
            output_stream << '\n';
        }
    }
}

void MazeArea::LoadMaze(const std::string & filePath)
{
    std::ifstream input_stream(filePath);
    if(!input_stream)
    {
        QMessageBox::critical(
          this,
          tr("Opening Maze"),
          tr(("Error: Could not open requested path:\n" + filePath).c_str()) );
    }

    else
    {

        std::stringstream line_stream;
        std::string buf;
        DeallocateMaze();

        // First grabbing the header info
        std::getline(input_stream, buf);
        line_stream << buf;
        if(!ParseHeader(line_stream, buf))
        {
            CreateRandomMaze(MazeWindowWidget::INITIAL_MAZE_X, MazeWindowWidget::INITIAL_MAZE_Y, MazeWindowWidget::INITIAL_MAZE_WALL_PERCENT);
        }
        else if(!ParseLines(input_stream, line_stream, buf))
        {
            CreateRandomMaze(MazeWindowWidget::INITIAL_MAZE_X, MazeWindowWidget::INITIAL_MAZE_Y, MazeWindowWidget::INITIAL_MAZE_WALL_PERCENT);
        }
    }
}

bool MazeArea::ParseHeader(std::stringstream & lineStream, std::string & buf)
{
    int num_elems = 0;
    while(std::getline(lineStream, buf, ' '))
    {
        ++num_elems;
        if(num_elems > 2)
        {
            QMessageBox::critical(
              this,
              tr("Opening Maze"),
              tr("Error: Expected only xDim and yDim on the first line, got more"));
            return false;
        }

        int parsed_int = std::stoi(buf);
        if(parsed_int <= 0)
        {
            QMessageBox::critical(
              this,
              tr("Opening Maze"),
              tr("Error: Got dimension of less than or equal to 0"));
            return false;
        }
        else if(mXDim == 0)
            mXDim = static_cast<std::size_t>(parsed_int);
        else
            mYDim = static_cast<std::size_t>(parsed_int);
    }

    if(num_elems < 2)
    {
        QMessageBox::critical(
          this,
          tr("Opening Maze"),
          tr("Error: Expected only xDim and yDim on the first line, got less"));
        return false;
    }

    return true;
}

bool MazeArea::ParseLines(std::ifstream & inputStream, std::stringstream & lineStream, std::string & buf)
{
    std::size_t num_y = 0;
    std::size_t num_x = 0;
    while(std::getline(inputStream, buf))
    {
        ++num_y;
        if(num_y > mYDim)
        {
            QMessageBox::critical(
              this,
              tr("Opening Maze"),
              tr("Error: Got more in the y dimension than specified in the header"));
            DeallocateMaze();
            return false;
        }

        num_x = 0;
        lineStream.clear();
        lineStream << buf;
        while(std::getline(lineStream, buf, ' '))
        {
            ++num_x;
            if(num_x > mXDim)
            {
                QMessageBox::critical(
                  this,
                  tr("Opening Maze"),
                  tr(("Error: On line " + std::to_string(num_y) + ": got more elems than specified in the header").c_str()));
                DeallocateMaze();
                return false;
            }

            MazeNode * new_node = new MazeNode(this);
            new_node->SetWidthHeight(MAZE_SQUARE_WIDTH);
            new_node->SetLabelFontSize(MAZE_FONT_SIZE);
            new_node->SetMazeCoords(num_x, num_y);
            mMaze.push_back(new_node);
            mMazeGrid->addWidget(new_node);

            if(buf == "s")
            {
                new_node->SetStart(true);
                mStartNode = new_node;
            }
            else if(buf == "t")
            {
                new_node->SetEnd(true);
                mEndNode = new_node;
            }
            else if(buf == "w")
                new_node->SetWall(true);
            else if(buf == "0")
                new_node->SetMoveCost(0);
            else
            {
                int move_cost = std::stoi(buf);
                if(move_cost <= 0) // = 0 handled in else if
                {
                    QMessageBox::critical(
                      this,
                      tr("Opening Maze"),
                      tr(("Error: On line " + std::to_string(num_y) + " column " + std::to_string(num_x) +": got unknown symbol, or number less than 0").c_str()));
                    DeallocateMaze();
                    return false;
                }

                new_node->SetMoveCost(move_cost);
            }
        }

        if(num_x < mXDim)
        {
            QMessageBox::critical(
              this,
              tr("Opening Maze"),
              tr(("Error: On line " + std::to_string(num_y) + ": got less elems than specified in the header").c_str()));
            DeallocateMaze();
            return false;
        }
    }

    if(num_y < mYDim)
    {
        QMessageBox::critical(
          this,
          tr("Opening Maze"),
          tr("Error: Got less in the y dimension than specified in header"));
        DeallocateMaze();
        return false;
    }
}

std::size_t MazeArea::Size()
{
    return mXDim * mYDim;
}

std::size_t MazeArea::XDim()
{
    return mXDim;
}

std::size_t MazeArea::YDim()
{
    return mYDim;
}

MazeNode * MazeArea::GetStartNode()
{
    return mStartNode;
}

MazeNode * MazeArea::GetEndNode()
{
    return mEndNode;
}







