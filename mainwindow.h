/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include "howtoplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void create_window();

    void easy_start();
    void medium_start();
    void hard_start();

    void play_now();
    void how_to_play();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    gameboard* board;
    howtoplay* instructions;
};

#endif // MAINWINDOW_H
