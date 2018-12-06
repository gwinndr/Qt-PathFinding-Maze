#ifndef MAZESOLVERWINDOWWIDGET_H
#define MAZESOLVERWINDOWWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "mazearea.h"
#include "mazestaticsolver.h"

class MazeSolverWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeSolverWindowWidget(QWidget *parent = nullptr);
    ~MazeSolverWindowWidget();

    void Initialize(MazeArea * mazeArea, MazeStaticSolver * solver);

protected:
    MazeArea * mMazeArea;
    MazeStaticSolver * mSolver;

    QHBoxLayout * mMainLayout;
    QGridLayout * mRightLayoutWrapper;
    QGridLayout * mRightLayout;

    QLabel * mAlgoLabel;

    QLabel * mStepsHeader;
    QLabel * mStepsNum;

    QLabel * mPathCostHeader;
    QLabel * mPathCostNum;

    QPushButton * mSolveStepBtn;
    QPushButton * mSolveAllBtn;

    QFont mHeaderFont;
    QFont mNumberFont;

    bool mSolutionFound = false;

    void RenderLayout();

    void ConnectButtons();

signals:

protected slots:
    void SolveStepBtnPushed();

    void SolveAllBtnPushed();

    void StepsUpdate(const int & numSteps);

};

#endif // MAZESOLVERWINDOWWIDGET_H



