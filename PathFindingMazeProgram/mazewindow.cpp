#include "mazewindow.h"
#include "ui_mazewindow.h"
#include "mazearea.h"
#include <QPainter>
#include <QDesktopWidget>
#include "mazewindowwidget.h"

MazeWindow::MazeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MazeWindow),
    mMazeWidget(new MazeWindowWidget)
{
    ui->setupUi(this);
    setCentralWidget(mMazeWidget);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    setWindowTitle("PathFinding Maze");
}

MazeWindow::~MazeWindow()
{
    delete ui;
    delete mMazeWidget;
}
