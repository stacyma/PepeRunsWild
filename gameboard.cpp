/*
Stacy Ma
6/4/15
PIC 10C
Spring 2015

Final Project: Pepe Runs Wild!
*/

#include "gameboard.h"
#include "ui_gameboard.h"
#include "mainwindow.h"

//set seed for random number generator
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);

/**Constructor to initialize gameboard
 * @brief gameboard::gameboard
 * @param parent is the main window of the board
 * @param board_sz is the board size based on the difficulty level
 * @param diff is the text of the difficulty level
 */
gameboard::gameboard(QWidget *parent, size_t board_sz, QString diff) :
    QMainWindow(parent),
    ui(new Ui::gameboard), board_size(board_sz), difficulty(diff)
{
    ui->setupUi(this);

    /* Display game title at top of window */
    Top = new QWidget;
    Top->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 10);}"); //make top bar transparent
    QVBoxLayout *top_bar = new QVBoxLayout(Top);

    title = new QLabel ("Pepe Runs Wild");
    title->setStyleSheet("QLabel{font-size: 16pt; font-family: comic sans ms;}");
    top_bar->addWidget(title);

    /* Display number of lives below title with panda image */
    QString panda_filename(":/Images/pandatransparent.bmp");
    panda_image=new QPixmap(panda_filename);

    Lives = new QWidget;
    QHBoxLayout *lives_bar = new QHBoxLayout(Lives);
    lives_left=3; //give player 3 lives
    lives = new QLabel*[lives_left-1]; //set number of lives shown in top bar to lives left - 1

    for(size_t i = 0; i < lives_left-1; ++i){
        lives[i]= new QLabel;
        lives[i]->setPixmap(*panda_image);
        lives[i]->setMinimumSize(40, 40);
        lives[i]->setMaximumSize(40, 40);
        lives[i]->setScaledContents(true);
        lives_bar->addWidget(lives[i]);
    } //display lives with panda image
    top_bar->addWidget(Lives);


    /* Display difficulty level */
    Difficulty = new QWidget;
    QHBoxLayout *difficulty_bar = new QHBoxLayout(Difficulty);
    QLabel* difficulty_text = new QLabel("Difficulty:");
    level = new QLabel(difficulty);
    difficulty_text->setStyleSheet("QLabel{font-size: 10pt; font-family: comic sans ms;}");
    level->setStyleSheet("QLabel{font-size: 10pt; font-family: comic sans ms;}");
    difficulty_bar->addWidget(difficulty_text);
    difficulty_bar->addWidget(level);

    top_bar->addWidget(Difficulty);

    /* Display score next to difficulty level*/
    score=0;
    QLabel* bar = new QLabel("|");
    display_score=new QLabel;
    display_score->setNum(score);
    display_score->setStyleSheet("QLabel{font-size: 11pt; font-family: comic sans ms;}");
    display_score->setAlignment(Qt::AlignRight);
    difficulty_bar->addWidget(bar);
    difficulty_bar->addWidget(display_score);


    /* Create board */
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size];
    QGridLayout *board_grid = new QGridLayout(Board); //create gridlayout for board

    for(size_t i = 0; i < board_size; ++i){
        for(size_t j = 0; j < board_size; ++j){
            labels[i*board_size+j]=new QLabel;
            labels[i*board_size+j]->setStyleSheet("QLabel { background-color: white;} ");

            if(board_size==10){
                labels[i*board_size+j]->setMinimumSize(40, 40);
                labels[i*board_size+j]->setMaximumSize(40, 40);
            }
            else if(board_size==15){
                labels[i*board_size+j]->setMinimumSize(28, 28);
                labels[i*board_size+j]->setMaximumSize(28, 28);
            }
            else if(board_size==20){
                labels[i*board_size+j]->setMinimumSize(20, 20);
                labels[i*board_size+j]->setMaximumSize(20, 20);
            }
            //set size of grid boxes based on board size

            labels[i*board_size+j]->setFrameStyle(0);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);
            board_grid -> addWidget(labels[i*board_size+j], i, j);
        }
    }

    /*Create bottom bar with timer*/
    min = 3;
    sec = 0;
    //set timer to 3 minutes

    minutes.setNum(min);
    seconds.setNum(sec);
    extra_zero.setNum(0);

    if(sec==0)
        time = minutes + ":" + extra_zero + seconds;
    else
        time = minutes + ":" + seconds;

    display_time=new QLabel;
    display_time->setText(time);
    display_time->setStyleSheet("QLabel{font-size: 16pt; color: white; font-weight: bold; font-family: arial;}");

    Bottom = new QWidget;
    Bottom->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 10);}");
    QHBoxLayout *bottom_bar = new QHBoxLayout(Bottom);
    bottom_bar->addWidget(display_time);

    game_timer = new QTimer(this);
    connect(game_timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    game_timer->start(1000);
    //create timer to update time every second by calling updateTimer function


    /*Initialize Pepe's position*/
    panda_position=new QPoint(0,0);
    labels[0]= new QLabel("Pepe");
    labels[0]->setPixmap(*panda_image);
    labels[0]->setScaledContents(true);
    labels[0]->setStyleSheet("QLabel{background-color: white;}");
    board_grid->addWidget(labels[0], 0, 0);

    Board->setFixedSize(400,400); //fix size of board

    /* Put all widgets together */
    setFixedSize(500, 635); //fix size of window
    QWidget *main = new QWidget(parent);
    QVBoxLayout *full = new QVBoxLayout(main);
    full->addWidget(Top, 0, Qt::AlignCenter);
    full->addWidget(Board, 0, Qt::AlignCenter);
    full->addWidget(Bottom,0,Qt::AlignCenter);

    this->setLayout(full);

    setCentralWidget(main);

    //set random number generator
    std::uniform_int_distribution<int> distribution(1,board_size + 1);

    /* set bamboo positions */
    QString bamboo_filename(":/Images/bamboo.bmp");
    bamboo_image=new QPixmap(bamboo_filename);

    number_bamboo = board_size*2;
    bamboo_left = number_bamboo;
    bamboo_positions=new QPoint[number_bamboo]; //create coordinate system for bamboo
    bamboo_vector=std::vector<int>(number_bamboo); //create vector to keep track of whether or not bamboo was eaten

    for(size_t i = 0; i < number_bamboo; ++i){
        bamboo_positions[i].setX(distribution(generator)%(board_size));
        bamboo_positions[i].setY(distribution(generator)%(board_size));
        bamboo_vector[i]=1; //value of 1 means bamboo has not been eaten
    }

    /* initialize special bamboo image */
    QString special_bamboo(":/Images/bluebamboo.bmp");
    special_bamboo_image = new QPixmap(special_bamboo);
    special_power=false; //initialize special power variable to false to show that special bamboo has not been eaten
    power_used=false;

    /* set hunter positions */
    QString hunter_filename(":/Images/hunter.bmp");
    hunter_image = new QPixmap(hunter_filename);

    number_hunters = (int)(board_size/2); //set number of hunters based on board size
    hunter_positions = new QPoint[number_hunters]; //create coordinate system for hunters
    hunter_vector=std::vector<int>(number_hunters); //create vector to keep track of whether or not hunter was destroyed

    for(size_t i = 0; i < number_hunters; ++i){
        hunter_positions[i].setX(distribution(generator)%(board_size) + 1);
        hunter_positions[i].setY(distribution(generator)%(board_size) + 1);
        hunter_vector[i]=1; //value of 1 means hunter still exists
    }

    /* set timer for hunters to move based on difficulty */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateHunters()));
    if(board_size==10)
        timer->start(100);
    else if(board_size==15)
        timer->start(75);
    else if(board_size==20)
        timer->start(50);

    /* set up trap positions */
    trap_vector=std::vector<int>(board_size); //create vector to keep track of whether or not trap was destroyed
    number_traps=0; //start number of traps as 0
    trap_positions = new QPoint[board_size]; //create coordinate system for traps
    QString trap_filename(":/Images/trap.bmp");
    trap_image = new QPixmap(trap_filename);

    QWidget::setAttribute(Qt::WA_DeleteOnClose); //delete board when close() is called
}

/**Allow keyboard function with left, right, up, and down arrows for movement; spacebar for eliminating a trap or hunter
 * @brief gameboard::keyPressEvent
 * @param event is the event that occurs when a key is pressed
 */
void gameboard::keyPressEvent(QKeyEvent *event){

    //get x and y values of panda position
    int x = panda_position->rx();
    int y = panda_position->ry();

    //call updatePanda function with appropriate parameters when left, right, up, or down arrows are pressed
    switch (event->key()) {
    case Qt::Key_Left:
        if(panda_position->rx() != 0)
            updatePanda(x,y,x-1,y);

        break;
    case Qt::Key_Right:
        if(panda_position->rx() != board_size-1)
            updatePanda(x,y,x+1,y);

        break;
    case Qt::Key_Up:
        if(panda_position->ry() != 0)
            updatePanda(x,y,x,y-1);

        break;
    case Qt::Key_Down:
        if(panda_position->ry() != board_size-1)
            updatePanda(x,y,x,y+1);

        break;
    case Qt::Key_Space:
        //if press spacebar while at a hunter position, clear the hunter without losing a life
        if(special_power && hitHunter(x,y)){
            hunter_vector[hunter_index]=0;
            special_power=false;
            ++lives_left;
        }
        //if press spacebar while at a trap position, clear the trap without losing a life
       else if(special_power && hitTrap(x,y)){
            trap_vector[trap_index]=0;
            special_power=false;
            ++lives_left;
        }

        break;

    default:
        QWidget::keyPressEvent(event);
    }

    QCoreApplication::processEvents();
    repaint();
}

/**Paint new panda position, updated bamboo positions, updated hunter positions, and updated trap positions
 * @brief gameboard::paintEvent
 * @param e is the event that is being repainted
 */
void gameboard::paintEvent(QPaintEvent *e){
    int x = panda_position->rx();
    int y = panda_position->ry();

    //paint bamboo on board at updated locations
    for(size_t i = 0; i<number_bamboo; ++i){
        int px = bamboo_positions[i].rx();
        int py = bamboo_positions[i].ry();

        if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size && bamboo_vector[i]!=0 && i == number_bamboo-1){

            labels[py*board_size+px]->setPixmap(*special_bamboo_image);
            labels[py*board_size+px]->setScaledContents(true);
        }
        else if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size && bamboo_vector[i]!=0) {
            labels[py*board_size+px]->setPixmap(*bamboo_image);
            labels[py*board_size+px]->setScaledContents(true);
        }
        else{
            labels[px*board_size+py]->clear();
        }
    }

    //paint panda at new location
    labels[y*board_size+x]->setPixmap(*panda_image);
    labels[y*board_size+x]->setScaledContents(true);

    //paint hunters at new locations
    for(size_t i = 0; i<number_hunters; ++i){
        int px = hunter_positions[i].rx();
        int py = hunter_positions[i].ry();

        if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size && hunter_vector[i]!=0) {
            labels[py*board_size+px]->setPixmap(*hunter_image);
            labels[py*board_size+px]->setScaledContents(true);
        }
    }

    //paint traps at locations
    for(size_t i = 0; i<number_traps; ++i){
        int px = trap_positions[i].rx();
        int py = trap_positions[i].ry();

        if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size && trap_vector[i]!=0) {
            labels[py*board_size+px]->setPixmap(*trap_image);
            labels[py*board_size+px]->setScaledContents(true);
        }
    }


}

/**Show repainted event
 * @brief gameboard::showEvent
 * @param e is the event being repainted
 */
void gameboard::showEvent(QShowEvent *e){
    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

/**Set panda to new position, update lives, update bamboo positions; check for win or loss and respective functions if so
   @brief gameboard::updatePanda
   @param px is the x coordinate of the current panda position
   @param py is the y coordinate of the current panda position
   @param nx is the x coordinate of the next panda position
   @param ny is the y coordinate of the next panda position
 */
void gameboard::updatePanda(int px, int py, int nx, int ny) {

    //if all bamboo are eaten, game over
    if(bamboo_left==0){
        win();
    }
    //if current position is the same as a hunter or trap, take a way a life
    if(this->hitHunterOrTrap(px, py)){
        if(lives_left > 1){
            lives_left--;
            lives[lives_left-1]->clear();
        }
        //if no more lives left, game over
        else
            lose();
    }
    //if time runs out, lose game
   if(min==0 && sec==0 || min<0)
        lose();

    //if position is same as bamboo, clear bamboo
    if(this->hitBamboo(px, py)){
        bamboo_vector[bamboo_index]=0;
        updateScore(10); //add 10 points to score when bamboo is eaten
        --bamboo_left;
    }

    //clear current panda position
    labels[py*board_size+px]->clear();
    labels[py*board_size+px]->setStyleSheet("QLabel {background-color: white;}");

    //set new panda posiiton
    panda_position->setX(nx);
    panda_position->setY(ny);

}

/**Make hunters move in random motions
   @brief gameboard::updateHunters
 */
void gameboard::updateHunters() {
    std::uniform_real_distribution<double> p(0,1);
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);
    std::uniform_int_distribution<int> trap(0,(25-board_size));

    for(size_t i=0;i<number_hunters;i++) {
        int px = hunter_positions[i].rx();
        int py = hunter_positions[i].ry();

        //clear hunter from current position
        if(px>=0 && py >= 0 && px<(int)board_size && py<(int)board_size) {
            labels[py*board_size+px]->clear();
        }

        int nx = px;
        int ny = py;

        //if it is time to move hunter, set new X position or Y position, check if new position is a free space
        if(p(generator) < 0.1) {
            if(x_or_y(generator) == 0) {
                nx = px + 2*increment(generator)-1;
                if(nx >= -1 && nx <= (int)board_size && freeSpace(nx, ny))
                    hunter_positions[i].setX(nx);
            }
            else {
                ny = py + 2*increment(generator)-1;
                if(ny >= -1 && ny <= (int)board_size && freeSpace(nx, ny))
                    hunter_positions[i].setY(ny);
            }
            //if it is time to leave a trap, call addATrap
            if(trap(generator)==1 && number_traps<board_size && freeSpace(px,py)){
                addATrap(px, py);
            }
        }
    }

    repaint();
    QCoreApplication::processEvents();

}

/**Update countdown game timer
   @brief gameboard::updateTimer
 */
void gameboard::updateTimer(){
   //update time if time is greater than 0:00
    if(min>0 || min==0 && sec>0){
        //if seconds is 0, decrement to next lower minute and 59 seconds, else just decrement seconds
        if(sec==0){
            sec=59;
            --min;
        }
        else
            --sec;

        minutes.setNum(min);
        seconds.setNum(sec);

        if(sec==0)
            time = minutes + ":" + seconds + extra_zero;
        else
            time = minutes + ":" + seconds;

        display_time->setText(time);
    }
}

/**Update the total score a certain amount of points
 * @brief gameboard::updateScore
 * @param inc is the amount of points to increment the total score by
 */
void gameboard::updateScore(int inc){
    score+=inc;
    display_score->setNum(score);
    if(inc==50 && !power_used)
       specialBamboo();
}

/**Adds a trap to a coordinate on the grid and increments the number of traps
 * @brief gameboard::addATrap
 * @param px is the x coordinate of the position of the trap to be added
 * @param py is the y coordinate of the position of the trap to be added
 */
void gameboard::addATrap(size_t px, size_t py){
    trap_positions[number_traps].setX(px);
    trap_positions[number_traps].setY(py);

    trap_vector[number_traps]=1;
    ++number_traps;
}

/**Check if space is free of bamboo, a hunter, and a trap
 * @brief gameboard::freeSpace
 * @param x is the x coordinate of the current position
 * @param y is the y coordinate of the current position
 * @returns true if space is free, false if not
 */
bool gameboard::freeSpace(int x, int y){
    bool not_off_the_board = true;
    bool bamboo_present = false;
    bool hunter_present = false;
    bool trap_present = false;

    for(size_t i = 0; i < number_bamboo; ++i){
        if(bamboo_positions[i].rx()==x && bamboo_positions[i].ry()==y){
            bamboo_present= true;
        }
    }
    for(size_t i = 0; i < number_hunters; ++i){
        if(hunter_positions[i].rx()==x && hunter_positions[i].ry()==y){
            hunter_present= true;
        }
    }
    for(size_t i = 0; i < number_traps; ++i){
        if(trap_positions[i].rx()==x && trap_positions[i].ry()==y){
            trap_present= true;
        }
    }
    if(x<=0 || y <= 0 || x>((int)board_size-1)|| y>((int)board_size-1))
        not_off_the_board = false;

    return (!(hunter_present) && !(trap_present) && !(bamboo_present) && not_off_the_board);
}

/**Check if panda hit a hunter
 * @brief gameboard::hitHunter
 * @param x is the x coordinate of the current position
 * @param y is the y coordinate of the current position
 * @returns true if the current panda position is in the same position as a hunter, false if not
 */
bool gameboard::hitHunter(int x, int y){
    for(size_t i = 0; i < number_hunters; ++i){
        if(hunter_positions[i].rx()==x && hunter_positions[i].ry()==y){
            hunter_index=i;
            return true;
        }
    }
    return false;
}

/**Check if panda hit a trap
 * @brief gameboard::hitTrap
 * @param x is the x coordinate of the current position
 * @param y is the y coordinate of the current position
 * @returns true if the current panda position is in the same position as a trap, false if not
 */
bool gameboard::hitTrap(int x, int y){
    for(size_t i = 0; i < number_traps; ++i){
        if(trap_positions[i].rx()==x && trap_positions[i].ry()==y){
            trap_index=i;
            return true;
        }
    }
    return false;
}

/**Check if position is same as that of a hunter or a trap
 * @brief gameboard::hitHunterOrTrap
 * @param x is the x coordinate of the current position
 * @param y is the y coordinate of the current position
 * @returns true if panda is in the same position as a hunter or trap
 */
bool gameboard::hitHunterOrTrap(int x, int y){
    return (hitHunter(x,y) || hitTrap(x,y));
}

/**Check if position is same as that of a bamboo
 * @brief gameboard::hitBamboo
 * @param x is the x coordinate of the current position
 * @param y is the y coordinate of the current position
 * @returns true if panda is in the same position as bamboo
 */
bool gameboard::hitBamboo(int x, int y){

    for(size_t i = 0; i < number_bamboo; ++i){
        if(bamboo_positions[i].rx()==x && bamboo_positions[i].ry()==y && bamboo_vector[i]!=0){
            bamboo_index=i;
            if(i==number_bamboo-1 && !power_used){
                special_power=true;
                updateScore(50); //add 50 points to score when special bamboo is eaten
                power_used=true;
            }
            return true;
        }
    }
    return false;
}

/**Initializes message box when game is won, stops timers, and deletes gameboard
 * @brief gameboard::youWin
 */

void gameboard::win(){
    youlost.setText("You Win! :)");
    youlost.exec();

    timer->stop();
    game_timer->stop();
    this->close();
}

/**Initializes message box when game is lost, stops timers, and deletes gameboard
 * @brief gameboard::youLost
 */

void gameboard::lose(){
    youlost.setText("You Lost! :(");
    youlost.exec();

    timer->stop();
    game_timer->stop();
    this->close();
}

/**Initializes message box when special bamboo is picked up
 * @brief gameboard::specialBamboo
 */
void gameboard::specialBamboo(){
    specialbamboo.setText("You picked up a SPECIAL bamboo!\nUse it wisely.");
    specialbamboo.exec();
}


/**Deallocate gameboard and its components from heap memory
 * @brief gameboard::~gameboard
 */
gameboard::~gameboard()
{
    delete panda_position;
    delete [] bamboo_positions;
    delete [] hunter_positions;
    delete [] trap_positions;
    delete ui;
}
