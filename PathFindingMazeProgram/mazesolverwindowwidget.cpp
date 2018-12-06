#include "mazesolverwindowwidget.h"

MazeSolverWindowWidget::MazeSolverWindowWidget(QWidget *parent) : QWidget (parent),
    mMazeArea(nullptr),
    mSolver(nullptr),
    mMainLayout(new QHBoxLayout(this)),
    mRightLayoutWrapper(new QGridLayout(this)),
    mRightLayout(new QGridLayout(this)),
    mAlgoLabel(new QLabel(this)),
    mStepsHeader(new QLabel(this)),
    mStepsNum(new QLabel(this)),
    mPathCostHeader(new QLabel(this)),
    mPathCostNum(new QLabel(this)),
    mSolveStepBtn(new QPushButton(this)),
    mSolveAllBtn(new QPushButton(this)),
    mHeaderFont(),
    mNumberFont()
{

}

MazeSolverWindowWidget::~MazeSolverWindowWidget()
{
    if(mMazeArea != nullptr)
        delete mMazeArea;
    if(mSolver != nullptr)
        delete mSolver;

    delete mMainLayout;
    delete mRightLayoutWrapper;
    delete mRightLayout;
    delete mAlgoLabel;
    delete mStepsHeader;
    delete mStepsNum;
    delete mPathCostHeader;
    delete mPathCostNum;
    delete mSolveStepBtn;
    delete mSolveAllBtn;
}

void MazeSolverWindowWidget::Initialize(MazeArea * mazeArea, MazeStaticSolver * solver)
{
    mMazeArea = mazeArea->GetCopy(this);
    //mMazeArea = mazeArea;
    mSolver = solver;
    QObject::connect(mSolver, &MazeStaticSolver::StepsUpdate, this, &MazeSolverWindowWidget::StepsUpdate);

    RenderLayout();
    ConnectButtons();

    mSolver->Initialize(mMazeArea, mMazeArea->GetStartNode(), mMazeArea->GetEndNode());
}

void MazeSolverWindowWidget::RenderLayout()
{
    mHeaderFont.setBold(true);
    mHeaderFont.setPointSize(16);

    mNumberFont.setPointSize(12);

    mAlgoLabel->setText(QString(mSolver->GetName().c_str()));
    mAlgoLabel->setFont(mHeaderFont);

    mStepsHeader->setText("Num Steps");
    mStepsHeader->setFont(mHeaderFont);
    mStepsNum->setText("0");
    mStepsNum->setFont(mNumberFont);

    mPathCostHeader->setText("Path cost");
    mPathCostHeader->setFont(mHeaderFont);
    mPathCostNum->setText("0");
    mPathCostNum->setFont(mNumberFont);

    mSolveStepBtn->setText("Solve Step");
    mSolveAllBtn->setText("Solve All Steps");

    mRightLayout->addWidget(mAlgoLabel,         0, 1, Qt::AlignCenter);
    mRightLayout->addWidget(mStepsHeader, 1, 0, Qt::AlignCenter);
    mRightLayout->addWidget(mStepsNum,    2, 0, Qt::AlignCenter);
    mRightLayout->addWidget(mPathCostHeader,    1, 2, Qt::AlignCenter);
    mRightLayout->addWidget(mPathCostNum,       2, 2, Qt::AlignCenter);
    mRightLayout->addWidget(mSolveStepBtn,      3, 0, Qt::AlignCenter);
    mRightLayout->addWidget(mSolveAllBtn,       3, 2, Qt::AlignCenter);

    mRightLayout->setSpacing(35);
    mRightLayoutWrapper->addLayout(mRightLayout, 0, 0, Qt::AlignCenter);
    mRightLayoutWrapper->setSpacing(0);
    mMainLayout->addWidget(mMazeArea);
    mMainLayout->addLayout(mRightLayoutWrapper);
}

void MazeSolverWindowWidget::ConnectButtons()
{
    QObject::connect(mSolveStepBtn, SIGNAL(clicked()), this, SLOT(SolveStepBtnPushed()));
    QObject::connect(mSolveAllBtn, SIGNAL(clicked()), this, SLOT(SolveAllBtnPushed()));
}

void MazeSolverWindowWidget::SolveStepBtnPushed()
{ 
    if(!mSolutionFound)
    {
        mSolutionFound = mSolver->SolveOne();
        if(mSolutionFound)
        {
            int path_size = mSolver->GetSolution();
            mPathCostNum->setText(QString(std::to_string(path_size).c_str()));
        }
    }
}

void MazeSolverWindowWidget::SolveAllBtnPushed()
{
    if(!mSolutionFound)
    {
        mSolutionFound = mSolver->SolveAll();
        if(mSolutionFound)
        {
            int path_size = mSolver->GetSolution();
            mPathCostNum->setText(QString(std::to_string(path_size).c_str()));
        }
    }
}

void MazeSolverWindowWidget::StepsUpdate(const int & numSteps)
{
    mStepsNum->setText(std::to_string(numSteps).c_str());
    mStepsNum->update();
}







