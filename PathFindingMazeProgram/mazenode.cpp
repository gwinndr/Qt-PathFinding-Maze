#include "mazenode.h"
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QLabel>

MazeNode::MazeNode(QWidget *parent) : QWidget(parent),
    mDrawingRect(),
    mPainter(),
    mSize()
{
}

MazeNode::~MazeNode()
{
}

void MazeNode::paintEvent(QPaintEvent *)
{

    QPen pen;
    QString label;
    QColor color;

    label = QString::fromStdString(std::to_string(mMoveCost));

    //mIsStart = true;
    //mIsEnd = true;
    if(mIsStart)
    {
        color.setRgb(255,215,0);
        label = QString("s");
        pen.setColor(Qt::black);
        pen.setWidth(5);
    }
    else if(mIsEnd)
    {
        color.setRgb(255,215,0);
        label = QString("t");
        pen.setColor(Qt::black);
        pen.setWidth(5);
    }
    else
    {
        //mVisited = true;
        //mIsPath = false;
        if(mVisited && !mIsPath)
        {
            pen.setColor(Qt::blue);
            pen.setWidth(10);
        }
        else
        {
            pen.setColor(Qt::black);
            pen.setWidth(5);
        }

        //mMoveCost = 1;
        if(mIsPath)
        {
            color.setRgb(255,215,0);
        }
        else if(mMoveCost < 0)
        {
            color.setRgb(0, 0, 0);
        }
        else if(mMoveCost == 0)
        {
            color.setRgb(255, 255, 255);
        }
        else if(mMoveCost == 1)
        {
            color.setRgb(255, 200, 200);
        }
        else if(mMoveCost == 2)
        {
            color.setRgb(255, 150, 150);
        }
        else if(mMoveCost == 3)
        {
            color.setRgb(255, 100, 100);
        }
        else if(mMoveCost == 4)
        {
            color.setRgb(255, 50, 50);
        }
        else
        {
            color.setRgb(255, 0, 0);
        }
    }

    mPainter.begin(this);
    mPainter.setPen(pen);
    mPainter.fillRect(mDrawingRect, color);
    mPainter.drawRect(mDrawingRect);

    if(mMoveCost >= 0 || mIsStart || mIsEnd)
    {
        QFont font;
        font.setPointSize(mFontSize);
        mPainter.setFont(font);
        mPainter.drawText(mDrawingRect, Qt::AlignCenter, label);
    }

    mPainter.end();
}

QSize MazeNode::sizeHint() const
{
    return mSize;
}

void MazeNode::SetMoveCost(const int & moveCost)
{
    mMoveCost = moveCost;
    update();
}

int MazeNode::Visit(bool changeVisitStatus)
{
    if(changeVisitStatus)
    {
        mVisited = true;
        update();
    }

    return mMoveCost;
}

void MazeNode::ResetVisitStatus()
{
    mVisited = false;
    update();
}

void MazeNode::SetPos(const int & x, const int & y)
{
    mDrawingRect.setX(x);
    mDrawingRect.setY(y);
    update();
}

void MazeNode::SetLabelFontSize(const int & labelSize)
{
    mFontSize = labelSize;
    update();
}

void MazeNode::SetWidthHeight(const int & widthHeight)
{
    mDrawingRect.setWidth(widthHeight);
    mDrawingRect.setHeight(widthHeight);
    mSize.setHeight(widthHeight+1);
    mSize.setWidth(widthHeight+1);
    update();
}

void MazeNode::SetWall(const bool & isWall)
{
    if(isWall)
        mMoveCost = -1;
    else
        mMoveCost = 1;

    update();
}

bool MazeNode::IsStart()
{
    return mIsStart;
}

bool MazeNode::IsEnd()
{
    return mIsEnd;
}

bool MazeNode::IsWall()
{
    return mMoveCost < 0;
}

bool MazeNode::IsVisited()
{
    return mVisited;
}

void MazeNode::SetStart(bool isStart)
{
    mIsStart = isStart;
    if(isStart)
        mMoveCost = 0;
    update();
}

void MazeNode::SetEnd(bool isEnd)
{
    mIsEnd = isEnd;
    if(isEnd)
        mMoveCost = 0;
    update();
}

void MazeNode::SetPath(bool isPath)
{
    mIsPath = isPath;
    update();
}

bool MazeNode::IsPath()
{
    return mIsPath;
}

void MazeNode::SetMazeCoords(const int & x, const int & y)
{
    mXCoord = x;
    mYCoord = y;
}

void MazeNode::GetMazeCoords(int & x, int & y)
{
    x = mXCoord;
    y = mYCoord;
}







