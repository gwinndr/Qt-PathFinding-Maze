#ifndef MAZEAREA_H
#define MAZEAREA_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <sstream>

#include "mazenode.h"

class MazeArea : public QWidget
{
    Q_OBJECT
public:
    explicit MazeArea(QWidget *parent = nullptr);
    ~MazeArea();

    MazeArea * GetCopy(QWidget *parent = nullptr);

    MazeNode * GetNode(const std::size_t & x, const std::size_t y);

    void CreateRandomMaze(const int & xDim, const int & yDim, const std::size_t percentWall);

    void SaveMaze(const std::string & filePath);

    void LoadMaze(const std::string & file);

    std::size_t Size();

    std::size_t XDim();

    std::size_t YDim();

    MazeNode * GetStartNode();

    MazeNode * GetEndNode();

protected:
    QGridLayout * mMazeGrid;

    std::vector<MazeNode *> mMaze;
    std::size_t mXDim = 0;
    std::size_t mYDim = 0;

    MazeNode * mStartNode = nullptr;
    MazeNode * mEndNode = nullptr;

    static const int MAZE_SQUARE_WIDTH = 50;
    static const int MAZE_FONT_SIZE = 16;

    void DeallocateMaze();

    bool ParseHeader(std::stringstream & lineStream, std::string & buf);

    bool ParseLines(std::ifstream & inputStream, std::stringstream & lineStream, std::string & buf);

private:

signals:

public slots:
};

#endif // MAZEAREA_H
