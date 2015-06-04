/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"
#include "howtoplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    create_window();
    board = new gameboard(this, 10, "Easy");
}

/**Initialize window for easy difficulty level with board size of 10
 * @brief MainWindow::easy_start
 */
void MainWindow::easy_start(){
    board = new gameboard(this, 10, "Easy");
}

/**Initialize window for medium difficulty level with board size of 15
 * @brief MainWindow::medium_start
 */
void MainWindow::medium_start(){
    board = new gameboard(this, 15, "Medium");
}

/**Initialize window for hard difficulty level with board size of 20
 * @brief MainWindow::hard_start
 */
void MainWindow::hard_start(){
    board = new gameboard(this, 20, "Hard");
}

/**Initializes main window
 * @brief MainWindow::create_window
 */
void MainWindow::create_window(){
    ui = new Ui::MainWindow;
    ui->setupUi(this);
}

/**Open gameboard window; reopens main window when gameboard object is deleted when game ends
 * @brief MainWindow::play_now
 */
void MainWindow::play_now(){
    this->setCentralWidget(board);
    connect(board, SIGNAL(destroyed()), this, SLOT(create_window()));
}

/**Open how to play window
 * @brief MainWindow::how_to_play
 */
void MainWindow::how_to_play(){
   instructions = new howtoplay;
   instructions->exec();
}

/**Destructor for main window
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
