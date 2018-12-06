#ifndef CONFIGURATIONAREA_H
#define CONFIGURATIONAREA_H

#include "mazearea.h"
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include "mazedynamicsolver.h"
#include "mazestaticsolver.h"

class ConfigurationArea : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationArea(QWidget *parent = nullptr);
    ~ConfigurationArea();

    void ConnectToMazeArea(MazeArea * mazeArea);

    void PopulateComboBoxes(std::vector<MazeStaticSolver*> staticSolvers, std::vector<MazeDynamicSolver*> dynamicSolvers);

protected:
    // Wrapper is a hack to make the layout more concise and less spread out
    QGridLayout * mWrapperLayout;

    QGridLayout * mConfigLayoutStatic;
    QLabel * mAlgoStaticLabel;
    QComboBox * mAlgorithmStaticBox;
    QLabel * mAlgoDynamicLabel;
    QComboBox * mAlgorithmDynamicBox;
    QLabel * mXLabel;
    QSpinBox * mXDimBox;
    QLabel * mYLabel;
    QSpinBox * mYDimBox;
    QLabel * mPercentWallLabel;
    QSpinBox * mPercentSpinBox;
    QPushButton * mCreateRandomMazeBtn;
    QPushButton * mLoadMazeBtn;
    QPushButton * mSaveMaze;
    QFont * mLabelFont;
    QFont * mSpinBoxFont;
    QPushButton * mSolveStaticMazeBtn;
    QPushButton * mSolveDynamicMazeBtn;

    MazeArea * mMazeArea = nullptr;

    void RenderLayout();

    void ConnectButtons();


private:

signals:
    void CreateRandomMaze(const int & xDim, const int & yDim, const int & percentWall);

    void SaveMaze(const std::string & filePath);

    void LoadMaze(const std::string & filePath);

    void LaunchStaticWindow(const int & staticIndex);

    void LaunchDynmaicWindow(const int & dynamicIndex);

public slots:

protected slots:
    void CreateRandomMazeBtnPushed();

    void SaveMazeBtnPushed();

    void LoadMazeBtnPushed();

    void SolveStaticMazeBtnPushed();

    void SolveDynamicMazeBtnPushed();
};

#endif // CONFIGURATIONAREA_H






