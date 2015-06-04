/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#ifndef HOWTOPLAY_H
#define HOWTOPLAY_H

#include <QDialog>

namespace Ui {
class howtoplay;
}

class howtoplay : public QDialog
{
    Q_OBJECT

public:
    explicit howtoplay(QWidget *parent = 0);
    ~howtoplay();

private:
    Ui::howtoplay *ui;
};

#endif // HOWTOPLAY_H
