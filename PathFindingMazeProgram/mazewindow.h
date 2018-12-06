#ifndef MAZEWINDOW_H
#define MAZEWINDOW_H

#include <QMainWindow>
#include "mazewindowwidget.h"

namespace Ui {
class MazeWindow;
}

class MazeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MazeWindow(QWidget *parent = nullptr);
    ~MazeWindow();

private:
    Ui::MazeWindow *ui;
    MazeWindowWidget * mMazeWidget;
};

#endif // MAZEWINDOW_H
