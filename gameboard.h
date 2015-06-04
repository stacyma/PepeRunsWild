/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <QSpacerItem>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>

namespace Ui {
class gameboard;
}

class gameboard : public QMainWindow
{
    Q_OBJECT

public slots:
   void updateHunters();
   void addATrap(size_t px, size_t py);
   void updateTimer();
   void updateScore(int inc);


public:
    explicit gameboard(QWidget *parent = 0, size_t board_size = 10, QString difficulty = "");

    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

    void updatePanda(int px, int py, int nx, int ny);
    bool freeSpace(int x, int y);
    bool hitHunter(int x, int y);
    bool hitTrap(int x, int y);
    bool hitHunterOrTrap(int x, int y);
    bool hitBamboo(int x, int y);

    void lose();
    void win();
    void specialBamboo();
    ~gameboard();


private:
    Ui::gameboard *ui;

    //panda variables
    const QPixmap* panda_image;
    QPoint *panda_position;

    //bamboo variables
    QPixmap* bamboo_image;
    QPixmap* special_bamboo_image;
    QPoint *bamboo_positions;
    size_t number_bamboo;
    size_t bamboo_left;
    size_t number_traps;
    int bamboo_index;
    std::vector<int> bamboo_vector;
    bool special_power;
    bool power_used;

    //hunter variables
    QPixmap* hunter_image;
    QPoint *hunter_positions;
    size_t number_hunters;
    std::vector<int> hunter_vector;
    int hunter_index;

    //trap variables
    QPixmap* trap_image;
    QPoint *trap_positions;
    std::vector<int> trap_vector;
    int trap_index;

    //win and lose message boxes
    QMessageBox youwin;
    QMessageBox youlost;
    QMessageBox specialbamboo;

    //Top bar variables
    QWidget* Top;
    QWidget* Lives;
    QWidget* Difficulty;
    QString difficulty;
    unsigned int lives_left;
    QLabel* title;
    QLabel** lives;
    QLabel* level;
    QLabel* display_score;
    int score;

    //Board variables
    QWidget* Board;
    size_t board_size;
    QLabel ** labels;

    //Bottom bar and timer variables
    QWidget* Bottom;
    QTimer* timer;
    QTimer* game_timer;
    QLabel* display_time;
    QString minutes;
    QString seconds;
    QString time;
    QString extra_zero;
    int min;
    int sec;
};

#endif // GAMEBOARD_H
