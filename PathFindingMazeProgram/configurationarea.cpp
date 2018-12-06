#include "configurationarea.h"
#include "mazewindowwidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFont>
#include <QFileDialog>
#include "mazesolverwindow.h"

ConfigurationArea::ConfigurationArea(QWidget *parent) : QWidget(parent),
    mWrapperLayout(new QGridLayout(this)),
    mConfigLayoutStatic(new QGridLayout(this)),
    mAlgoStaticLabel(new QLabel(this)),
    mAlgorithmStaticBox(new QComboBox(this)),
    mAlgoDynamicLabel(new QLabel(this)),
    mAlgorithmDynamicBox(new QComboBox(this)),
    mXLabel(new QLabel(this)),
    mXDimBox(new QSpinBox(this)),
    mYLabel(new QLabel(this)),
    mYDimBox(new QSpinBox(this)),
    mPercentWallLabel(new QLabel(this)),
    mPercentSpinBox(new QSpinBox(this)),
    mCreateRandomMazeBtn(new QPushButton(this)),
    mLoadMazeBtn(new QPushButton(this)),
    mSaveMaze(new QPushButton(this)),
    mLabelFont(new QFont),
    mSpinBoxFont(new QFont),
    mSolveStaticMazeBtn(new QPushButton(this)),
    mSolveDynamicMazeBtn(new QPushButton(this))
{
    RenderLayout();
    ConnectButtons();
}

ConfigurationArea::~ConfigurationArea()
{
    delete mWrapperLayout;
    delete mConfigLayoutStatic;
    delete mAlgorithmStaticBox;
    delete mAlgorithmDynamicBox;
    delete mXDimBox;
    delete mYDimBox;
    delete mPercentSpinBox;
    delete mCreateRandomMazeBtn;
    delete mLoadMazeBtn;
    delete mAlgoStaticLabel;
    delete mAlgoDynamicLabel;
    delete mXLabel;
    delete mYLabel;
    delete mPercentWallLabel;
    delete mLabelFont;
    delete mSaveMaze;
    delete mSolveStaticMazeBtn;
    delete mSolveDynamicMazeBtn;
    delete mSpinBoxFont;

}

void ConfigurationArea::ConnectToMazeArea(MazeArea * mazeArea)
{
    QObject::connect(this, &ConfigurationArea::CreateRandomMaze,
                     mazeArea, &MazeArea::CreateRandomMaze);
    mMazeArea = mazeArea;
}

void ConfigurationArea::PopulateComboBoxes(std::vector<MazeStaticSolver*> staticSolvers, std::vector<MazeDynamicSolver*> dynamicSolvers)
{
    for(auto itr = staticSolvers.begin(); itr != staticSolvers.end(); ++itr)
    {
        mAlgorithmStaticBox->addItem((*itr)->GetName().c_str());
    }

    for(auto itr = dynamicSolvers.begin(); itr != dynamicSolvers.end(); ++itr)
    {
        mAlgorithmDynamicBox->addItem((*itr)->GetName().c_str());
    }
}

void ConfigurationArea::RenderLayout()
{
    mLabelFont->setBold(true);
    mLabelFont->setPointSize(14);

    mSpinBoxFont->setPointSize(11);

    mAlgoStaticLabel->setText("Static Algorithm");
    mAlgoStaticLabel->setFont(*mLabelFont);
    mAlgoDynamicLabel->setText("Dynamic Algorithm");
    mAlgoDynamicLabel->setFont(*mLabelFont);
    mXLabel->setText("XDim");
    mXLabel->setFont(*mLabelFont);
    mYLabel->setText("YDim");
    mYLabel->setFont(*mLabelFont);
    mPercentWallLabel->setText("Percent Wall");
    mPercentWallLabel->setFont(*mLabelFont);

    mCreateRandomMazeBtn->setText("Create Random Maze");
    mSaveMaze->setText("Save Maze");
    mLoadMazeBtn->setText("Load Maze");
    mSolveStaticMazeBtn->setText("Solve Static Maze");
    mSolveDynamicMazeBtn->setText("Solve Dynamic Maze");

    mXDimBox->setMaximum(25);
    mXDimBox->setMinimum(1);
    mXDimBox->setFont(*mSpinBoxFont);
    mXDimBox->setValue(MazeWindowWidget::INITIAL_MAZE_X);

    mYDimBox->setMaximum(25);
    mYDimBox->setMinimum(1);
    mYDimBox->setFont(*mSpinBoxFont);
    mYDimBox->setValue(MazeWindowWidget::INITIAL_MAZE_Y);

    mPercentSpinBox->setMaximum(100);
    mPercentSpinBox->setMinimum(0);
    mPercentSpinBox->setFont(*mSpinBoxFont);
    mPercentSpinBox->setValue(MazeWindowWidget::INITIAL_MAZE_WALL_PERCENT);

    mWrapperLayout->addLayout(mConfigLayoutStatic, 0, 0, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mAlgoStaticLabel, 0, 0, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mAlgorithmStaticBox, 1, 0, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mAlgoDynamicLabel, 0, 2, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mAlgorithmDynamicBox, 1, 2, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mXLabel, 2, 0, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mYLabel, 2, 1, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mPercentWallLabel, 2, 2, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mXDimBox, 3, 0, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mYDimBox, 3, 1, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mPercentSpinBox, 3, 2, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mCreateRandomMazeBtn, 4, 0, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mSaveMaze, 4, 1, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mLoadMazeBtn, 4, 2, Qt::AlignCenter);

    mConfigLayoutStatic->addWidget(mSolveStaticMazeBtn, 5, 0, Qt::AlignCenter);
    mConfigLayoutStatic->addWidget(mSolveDynamicMazeBtn, 5, 1, Qt::AlignCenter);

    mConfigLayoutStatic->setVerticalSpacing(25);
}

void ConfigurationArea::ConnectButtons()
{
    QObject::connect(mCreateRandomMazeBtn, SIGNAL(clicked()), this, SLOT(CreateRandomMazeBtnPushed()));
    QObject::connect(mLoadMazeBtn, SIGNAL(clicked()), this, SLOT(LoadMazeBtnPushed()));
    QObject::connect(mSaveMaze, SIGNAL(clicked()), this, SLOT(SaveMazeBtnPushed()));
    QObject::connect(mSolveStaticMazeBtn, SIGNAL(clicked()), this, SLOT(SolveStaticMazeBtnPushed()));
    QObject::connect(mSolveDynamicMazeBtn, SIGNAL(clicked()), this, SLOT(SolveDynamicMazeBtnPushed()));
}

void ConfigurationArea::CreateRandomMazeBtnPushed()
{
    CreateRandomMaze(mXDimBox->value(), mYDimBox->value(), mPercentSpinBox->value());
}

void ConfigurationArea::SaveMazeBtnPushed()
{
    if(mMazeArea != nullptr)
    {
        QString output = QFileDialog::getSaveFileName(this, "Save Maze", QString(), tr("text files (*.txt)"));
        mMazeArea->SaveMaze(output.toStdString());
    }
}

void ConfigurationArea::LoadMazeBtnPushed()
{
    QString output = QFileDialog::getOpenFileName(this, "Open Maze", QString(), tr("text files (*.txt)"));
    mMazeArea->LoadMaze(output.toStdString());
}

void ConfigurationArea::SolveStaticMazeBtnPushed()
{
    LaunchStaticWindow(mAlgorithmStaticBox->currentIndex());
}

void ConfigurationArea::SolveDynamicMazeBtnPushed()
{
    LaunchDynmaicWindow(mAlgorithmDynamicBox->currentIndex());
}








