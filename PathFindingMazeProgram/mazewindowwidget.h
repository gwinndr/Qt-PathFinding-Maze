#ifndef MAZEWINDOWWIDGET_H
#define MAZEWINDOWWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <vector>
#include "mazearea.h"
#include "configurationarea.h"
#include "mazestaticsolver.h"
#include "mazedynamicsolver.h"
#include "Solvers/dijkstra.h"
#include "mazesolverwindow.h"
#include "Solvers/astar.h"
#include "Solvers/dstar.h"

class MazeWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeWindowWidget(QWidget *parent = nullptr);
    ~MazeWindowWidget();

    static const int INITIAL_MAZE_X = 5;
    static const int INITIAL_MAZE_Y = 5;
    static const int INITIAL_MAZE_WALL_PERCENT = 10;

protected:
    QHBoxLayout * mWindowLayout;
    MazeArea * mMazeArea;
    ConfigurationArea * mConfigArea;

    std::vector<MazeStaticSolver*> mStaticSolvers;
    std::vector<MazeDynamicSolver*> mDynamicSolvers;

    std::vector<Ui::MazeSolverWindow *> mMazeSolverWindows;

    // Put static solvers here and update vector creation in the constructor and the LaunchStaticSolver slot
    Dijkstra * mDikstraSolver;
    AStar * mAStarSolver;

    // Put dynamic solvers here and update vector creation in the constructor and the LaunchDynamicSolver slot
    DStar * mDStarSolver;

private:

signals:

public slots:
    void LaunchStaticSolver(const int & staticIndex);

    void LaunchDynamicSolver(const int & dynamicIndex);

};

#endif // MAZEWINDOWWIDGET_H
