#include "mazewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MazeWindow w;
    w.show();

    return a.exec();
}
