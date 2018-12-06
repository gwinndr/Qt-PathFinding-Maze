#include "mazewindowwidget.h"
#include <QHBoxLayout>
#include "mazearea.h"
#include "configurationarea.h"
#include "mazesolverwindow.h"

MazeWindowWidget::MazeWindowWidget(QWidget *parent) : QWidget(parent),
    mWindowLayout(new QHBoxLayout(this)),
    mMazeArea(new MazeArea(this)),
    mConfigArea(new ConfigurationArea(this)),
    mMazeSolverWindows(),
    mDikstraSolver(new Dijkstra(this)),
    mAStarSolver(new AStar(this)),
    mDStarSolver(new DStar(this))
{
    // Solvers go here
    mStaticSolvers.push_back(mDikstraSolver);
    mStaticSolvers.push_back(mAStarSolver);

    mDynamicSolvers.push_back(mDStarSolver);

    mWindowLayout->addWidget(mMazeArea);
    mWindowLayout->addWidget(mConfigArea);

    mWindowLayout->setMargin(50);

    // Load a random maze with initial values
    mMazeArea->CreateRandomMaze(INITIAL_MAZE_X, INITIAL_MAZE_Y, INITIAL_MAZE_WALL_PERCENT);

    // Connects the Maze Area and Config Area
    mConfigArea->ConnectToMazeArea(mMazeArea);

    mConfigArea->PopulateComboBoxes(mStaticSolvers, mDynamicSolvers);

    QObject::connect(mConfigArea, &ConfigurationArea::LaunchStaticWindow, this, &MazeWindowWidget::LaunchStaticSolver);
    QObject::connect(mConfigArea, &ConfigurationArea::LaunchDynmaicWindow, this, &MazeWindowWidget::LaunchDynamicSolver);
}

MazeWindowWidget::~MazeWindowWidget()
{
    delete mWindowLayout;
    delete mMazeArea;
    delete mConfigArea;
    delete mDikstraSolver;
    delete mAStarSolver;

    for(auto itr = mMazeSolverWindows.begin(); itr != mMazeSolverWindows.end(); ++itr)
        delete *itr;
    mMazeSolverWindows.clear();
}

void MazeWindowWidget::LaunchStaticSolver(const int & staticIndex)
{
    MazeStaticSolver * static_solver = mStaticSolvers[static_cast<std::size_t>(staticIndex)];
    bool found_solver = false;

    if(static_solver->GetName() == mDikstraSolver->GetName())
    {
        static_solver = new Dijkstra();
        found_solver = true;
    }
    else if(static_solver->GetName() == mAStarSolver->GetName())
    {
        static_solver = new AStar();
        found_solver = true;
    }

    if(found_solver)
    {
        Ui::MazeSolverWindow * maze_solver_win = new Ui::MazeSolverWindow();
        maze_solver_win->Initialize(mMazeArea, static_solver);
        maze_solver_win->show();
        mMazeSolverWindows.push_back(maze_solver_win);
    }
}

void MazeWindowWidget::LaunchDynamicSolver(const int & dynamicIndex)
{
    MazeDynamicSolver * dynamic_solver = mDynamicSolvers[static_cast<std::size_t>(dynamicIndex)];
    bool found_solver = false;
    if(dynamic_solver->GetName() == mDStarSolver->GetName())
    {
        dynamic_solver = new DStar();
        found_solver = true;
    }

    if(found_solver)
    {
        Ui::MazeSolverWindow * maze_solver_win = new Ui::MazeSolverWindow();
        maze_solver_win->Initialize(mMazeArea, dynamic_solver);
        maze_solver_win->show();
        mMazeSolverWindows.push_back(maze_solver_win);
    }
}




