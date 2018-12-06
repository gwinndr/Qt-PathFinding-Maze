#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T18:58:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PathFindingMaze
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mazewindow.cpp \
    mazenode.cpp \
    mazearea.cpp \
    mazewindowwidget.cpp \
    configurationarea.cpp \
    mazestaticsolver.cpp \
    mazedynamicsolver.cpp \
    mazesolverwindow.cpp \
    mazesolverwindowwidget.cpp \
    Solvers/dijkstra.cpp \
    Solvers/astar.cpp \
    Solvers/dstar.cpp

HEADERS += \
    mazewindow.h \
    mazenode.h \
    mazearea.h \
    mazewindowwidget.h \
    configurationarea.h \
    mazestaticsolver.h \
    mazedynamicsolver.h \
    mazesolverwindow.h \
    mazesolverwindowwidget.h \
    Solvers/dijkstra.h \
    Solvers/astar.h \
    Solvers/dstar.h

FORMS += \
        mazewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
