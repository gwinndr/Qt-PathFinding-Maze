#ifndef MAZENODE_H
#define MAZENODE_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QSize>


class MazeNode : public QWidget
{
    Q_OBJECT
public:
    explicit MazeNode(QWidget *parent = nullptr);
    ~MazeNode();

    // Needed for the layout to allocate the correct amount of render space
    QSize sizeHint() const;

    // The cost to move onto this square
    void SetMoveCost(const int & moveCost);

    // Gets the move cost and gives a blue border to mark it as visited
    int Visit(bool changeVisitStatus = true);

    // Sets the visit status to false
    void ResetVisitStatus();

    // Sets the rectangle position (currently unused)
    void SetPos(const int & x, const int & y);

    // Sets the label font size displaying the move costs
    void SetLabelFontSize(const int & labelSize);

    // Sets the width and height of the rectangle (creates a square)
    void SetWidthHeight(const int & widthHeight);

    // Sets the node to be a wall (or not)
    void SetWall(const bool & isWall);

    // Checks if it is the start node
    bool IsStart();

    // Checks if it is the end node
    bool IsEnd();

    // Checks if the node is a wall
    bool IsWall();

    bool IsVisited();

    // Sets the start status
    void SetStart(bool isStart);

    // Sets the end status
    void SetEnd(bool isEnd);

    void SetPath(bool isPath);

    bool IsPath();

    void SetMazeCoords(const int & x, const int & y);

    void GetMazeCoords(int & x, int & y);

protected:
    int mMoveCost = -1; // If <0, it's a wall
    bool mIsStart = false;
    bool mIsEnd = false;
    bool mVisited = false;
    bool mIsPath = false;
    int mFontSize = 16;
    int mXCoord = -1;
    int mYCoord = -1;

    QRect mDrawingRect;
    QPainter mPainter;
    QSize mSize;

    // Needed to draw the rectangle. Draws based on start, end, move cost, and visited
    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

#endif // MAZENODE_H
