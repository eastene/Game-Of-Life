
#include "GameOfLife_csci2312.h"

using namespace csci2312;

/*** CELL CLASS DEFINITIONS ***/
Cell::Cell() {

}

Cell::~Cell(){

}

bool Cell::getState() const{
    return state;
}

void Cell::setState(bool newState) {
    state = newState;
}

/*** GAMEOFLIFE CLASS DEFINITIONS ***/
GameOfLife::GameOfLife() {
    // set the board size to the maximum size
    boardSize = MAX_BOARD;

    // row iterator
    for (int i = 0; i < MAX_BOARD; i++){
        // column iterator
        for (int j = 0; j < MAX_BOARD; j++){
            // make each cell dead initially
            currentLife[i][j].setState(false);
            nextLife[i][j].setState(false);
        }
    }
}

GameOfLife::GameOfLife(size_t boardSize) {
    if (boardSize <= MAX_BOARD){
        // set boardSize to the specified value
        this->boardSize = boardSize;

        // create a new board boardSize x boardSize and fill it with dead cells
        // row iterator
        for (int i = 0; i < boardSize; i++){
            // column iterator
            for (int j = 0; j < boardSize; j++){
                // make each cell dead initially
                currentLife[i][j].setState(false);
                nextLife[i][j].setState(false);//  Display current board
            }
        }
    }else{
        // if the specified boardsize is greater than the maximum allowed, use the max size
        // already implemented in the default constructor
        GameOfLife::GameOfLife();
    }
}

GameOfLife::~GameOfLife() {

}

// decided the cells fate based on how many neighbors are alive
void GameOfLife::cellsFate(int r, int c, int neighbors) {
    // check state of cell currently
    if (currentLife[r][c].getState()){
        // if alive run this logic

        // cell only lives if 2 or 3 neighbors are alive, otherwise dies
        if (neighbors != 2 && neighbors != 3){
            nextLife[r][c].setState(false);
        }
    }else{
        // if the cell is dead run this logic

        // if the cell has exactly 3 neighbors alive, bring it to life
        if (neighbors == 3){
            nextLife[r][c].setState(true);
        }
    }
}

void GameOfLife::checkCell(int r, int c){

    int neighborsAlive = 0; // tally of the number of the cells neighbors that are alive

    // row iterator (looks at row above, on, and below current cell)
    for (int i = r - 1; i < r + 1; i++){
        // if that row is in the board check, else ignore
        if (i >= 0 && i < boardSize){
            // column iterator (looks at column left, on, and right of the current cell)
            for (int j = c - 1; j < c + 1; j++){
                // if the column is on the board check, else ignore
                if (j >= 0 && j < boardSize){
                    // if the neighbor cell is alive, add 1 to the tally
                    if (currentLife[i][j].getState()){
                        neighborsAlive++;
                    }
                }
            }
        }
    }

    // call function to decide cells fate based on the game rules
    cellsFate(r, c, neighborsAlive);
}

void GameOfLife::nextGeneration() {
    for(int i = 0; i < boardSize; i++){
        for(int j = 0; j < boardSize; j++){
            checkCell(i, j);
        }
    }

   /* // swap this generation with the next
    temp = currentLife;
    currentLife = nextLife;
    nextLife = temp; */
}

// runs the game of life
void GameOfLife::run() {
    // iterate over each cell and generate the next generation
    nextGeneration();
}


ostream& operator << (ostream& out, GameOfLife& board){
    for (int i = 0; i < board.boardSize; i++){
        for (int j = 0; j < board.boardSize; j++){
            if (board.nextLife[i][j].getState()){
                out << board.nextLife[i][j].alive;
            }else{
                out << board.nextLife[i][j].dead;
            }
        }
        out << std::endl;
    }

    return out;
}

ostream& Cell::operator << (ostream& out){
    if (state){
        out << alive;
    }else{
        out << dead;
    }

    return out;
}



