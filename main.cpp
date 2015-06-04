/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

/** @mainpage PIC 10C Spring 2015 Final Project: Pepe Runs Wild!
    @author Stacy Ma
    @date June 4th, 2015
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
