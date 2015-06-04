/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#include "howtoplay.h"
#include "ui_howtoplay.h"

howtoplay::howtoplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::howtoplay)
{
    ui->setupUi(this);
}

howtoplay::~howtoplay()
{
    delete ui;
}
