#include "mazesolverwindow.h"
#include "mazesolverwindowwidget.h"
#include <QDesktopWidget>

Ui::MazeSolverWindow::MazeSolverWindow(QWidget *parent) : QMainWindow(parent),
    mSolverWidget(new MazeSolverWindowWidget(this))
{
    setCentralWidget(mSolverWidget);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    setWindowTitle("Maze Solver");
}

Ui::MazeSolverWindow::~MazeSolverWindow()
{
    delete mSolverWidget;
}

void Ui::MazeSolverWindow::Initialize(MazeArea * mazeArea, MazeStaticSolver * solver)
{
    mSolverWidget->Initialize(mazeArea, solver);
}
