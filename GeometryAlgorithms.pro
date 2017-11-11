#-------------------------------------------------
#
# Project created by QtCreator 2017-10-22T17:29:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeometryAlgorithms
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    	renderarea.cpp \
    	algorithmbase.cpp \
    	animationthread.cpp \
        utils.cpp \
        algorithms_practice/ga00_drawpolygon.cpp \
    	algorithms_practice/ga01_sweepline.cpp \
        algorithms_practice/ga02_convexhull.cpp \
        algorithms_projects/ga00_emptyproject.cpp \
    algorithms_practice/ga03_linesegmentintersection.cpp \
    algorithms_projects/ga03_nearestpoints.cpp

	
HEADERS += \
        mainwindow.h \
    	renderarea.h \
    	algorithmbase.h \
    	animationthread.h \
        utils.h \
        algorithms_practice/ga00_drawpolygon.h \
    	algorithms_practice/ga01_sweepline.h \
        algorithms_projects/ga00_emptyproject.h \
        algorithms_practice/ga02_convexhull.h \
    algorithms_practice/ga03_linesegmentintersection.h \
    algorithms_practice/ga03_datastructures.h \
    algorithms_projects/ga03_nearestpoints.h

FORMS += \
        mainwindow.ui
