#ifndef MAZESOLVERWINDOW_H
#define MAZESOLVERWINDOW_H

#include <QMainWindow>
#include "mazearea.h"
#include "mazestaticsolver.h"
#include "mazesolverwindowwidget.h"

namespace Ui {
class MazeSolverWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MazeSolverWindow(QWidget *parent = nullptr);
    ~MazeSolverWindow();

    void Initialize(MazeArea * mazeArea, MazeStaticSolver * solver);

protected:
    MazeSolverWindowWidget * mSolverWidget;

private:

signals:

public slots:
};

}
#endif // MAZESOLVERWINDOW_H
