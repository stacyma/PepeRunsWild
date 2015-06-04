#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T22:02:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PandaGame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    howtoplay.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    howtoplay.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    howtoplay.ui

RESOURCES += \
    images.qrc
