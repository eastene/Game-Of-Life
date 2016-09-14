
#include <fstream>
#include <chrono>   // milliseconds
#include <thread>   // thread.sleep_for()
#include <cstdlib>  // srand, rand
#include <ctime>    // time(), used to seed srand

#include "GameOfLife_csci2312.h"

using namespace csci2312;


/*** CELL CLASS DEFINITIONS ***/


// default constructor, left blank
// PRECONDITION: none
//
// POSTCONDITION: cell object will be initialized
Cell::Cell() {

}

// overloaded constructor, creates a cell set to a given state
// PRECONDITION: none
//
// POSTCONDITION: cell object will be initialized to the state passed as an argument
Cell::Cell(bool state) {
    this->state = state;
}

// deconstructor, unused since no memory is allocated in this class
Cell::~Cell(){

}

// returns the current state of this cell
// PRECONDITION: none
//
// POSTCONDITION: true = alive, false = dead
bool Cell::getState() const{
    return state;
}

// set this cell to alive or dead
// PRECONDITION: none
//
// POSTCONDITION: cell will be set to passed state
void Cell::setState(bool newState) {
    state = newState;
}


/*** GAMEOFLIFE CLASS DEFINITIONS ***/


/*** Constructors ***/

// defualt constructor, initializes game to 30 x 30 cells
// PRECONDITION: none
//
// POSTCONDITION: current life board will be initialized to 30 x 30 'dead' cells, boardSize will be set to MAX_BOARD
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

// overloaded constructor, initializes game to boardSize x boardSize cells
// PRECONDITION: boardSize must not be bigger than MAX_BOARD, or the game will be initialized incorrectly
//
// POSTCONDITION: current life board will be initialized to boardSize x boardSize 'dead' cells
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

// Destructor, unused since no memory is allocated
GameOfLife::~GameOfLife() {

}

/*** Public Methods ***/

// seeds the board with a given file, characters in the file determine the cell's state
// PRECONDITION: file must contain either the Cell's alive or dead character, no others, and must be
//  less than boardSize lines, each containing boardSize columns
//
// POSTCONDITION: game grid will contain cells in states specified in the file
int GameOfLife::seedBoard(string fileName) {
    std::fstream seed_file(fileName, std::ios::in); // file containing a matrix of dead and/or alive cells
    std::string line; // holds a line from the file to convert to the board
    int row = 0; // row iterator, increased with each line of the file read

    if (!seed_file){
        // returns 1 if something goes wrong when opening the file
        return 1;
    }

    while (!seed_file.eof()){
        if (row > boardSize){
            // too many lines in the seed_file, too large for matrix
            return 1;
        }

        getline(seed_file, line);

        if (line.size() > boardSize){
            // seed board too large to read
            return 1;
        }

        for (int col = 0; col < line.size(); col++){
            // iterate through each character in the line and change the cell in the current life accordingly
            if (line[col] == currentLife[row][col].alive){
                // if the character in the seed file matches the character of an alive cell, make that cell alive
                currentLife[row][col].setState(true);
            }
            else if (line[col] == currentLife[row][col].dead){
                // if the character in the seed file matches the character of a dead cell, make that cell dead
                currentLife[row][col].setState(false);
            }else{
                // character not recognized
                return 1;
            }
        }

        row++; // increment line count (which is also the row number in the matrix of cells)
    }

    // print out the now seeded board
    std::cout << *this;
    return 0; // success
}

// seeds board with a given number of cells to set as alive
// PRECONDITION: seeds must be less than boardSize * boardSize
//
// POSTCONDITION: seeds random cells will be set to alive
int GameOfLife::seedBoard(size_t seeds) {
    // check if seed is too large for matrix
    if (seeds > (boardSize * boardSize)){
        return 1;
    }

    // seed random number generator
    srand(time(NULL));

    // loop until all seed cells are filled
    size_t x, y; // a randomly assigned row and column

    while (seeds){
        x = rand() % boardSize; // random number between 0 and boardSize - 1
        y = rand() % boardSize; // random number between 0 and boardSize - 1

        if (!currentLife[x][y].getState()){
            // if the cell is not alive, make it alive, decrease number of seeded cells needed by 1
            currentLife[x][y].setState(true);
            seeds--;
        }

        // otherwise, continue
    }

    // print out the now seeded board
    std::cout << *this;
    return 0; // success
}

// runs the game of life one generation
// PRECONDITION: board must be seeded first
//
// POSTCONDITION: next generation of cells will be computed and printed
void GameOfLife::run() {
    // iterate over each cell and generate the next generation
    nextGeneration();
    // print out this object to show update
    std::cout << *this;
}

// runs the game of life numberOfIterations generations
// PRECONDITION: board must be seeded first
//
// POSTCONDITION: next generation of cells will be computed and printed numberOfIterations times
void GameOfLife::run(unsigned int numberOfIterations) {
    for (int i = 0; i < numberOfIterations; i++){
        nextGeneration();
        // print out this object every iteration
        std::cout << *this;
    }
}

// loops through each cell and calls functions to compute what it should be in the next generation
// PRECONDITION: board must be seeded first
//
// POSTCONDITION: currentLife will be used to generate nextLife, then the boards will be swapped
void GameOfLife::nextGeneration() {
    for(int i = 0; i < boardSize; i++){
        for(int j = 0; j < boardSize; j++){
            checkCell(i, j);
        }
    }

    // swap this generation with the next
    // this is the only solution I could find to swap arrays, simply assigning one to the other
    // threw an error
    std::swap(currentLife, nextLife);
}

/*** Private Methods ***/

// decided the cells fate based on how many neighbors are alive, accessory to checkCell function
// this function sets the rules of the game for each cell
// PRECONDITION: must be called from checkCell function only
//
// POSTCONDITION: cell will be set to alive or dead in the next generation based on its number of neighbors
void GameOfLife::cellsFate(int r, int c, int neighbors) {
    // check state of cell currently
    if (currentLife[r][c].getState()){
        // if alive run this logic

        // cell only lives if 2 or 3 neighbors are alive, otherwise dies
        nextLife[r][c].setState(neighbors == 2 || neighbors == 3);
    }else{
        // if the cell is dead run this logic

        // if the cell has exactly 3 neighbors alive, bring it to life
        nextLife[r][c].setState(neighbors == 3);
    }
}

// checks the number of alive cells neighboring the cell of interest, accessory function to nextGeneration
// PRECONDITION: must be called from nextGeneration function only
//
// POSTCONDITION: will compute the number of alive neighbors for a cell and call cellsFate to decide what the cell will
//  be in the next iteration
void GameOfLife::checkCell(int r, int c){

    int neighborsAlive = 0; // tally of the number of the cells neighbors that are alive

    // row iterator (looks at row above, on, and below current cell)
    for (int i = r - 1; i <= r + 1; i++){
        // if that row is in the board check, else ignore
        if (i >= 0 && i < boardSize){
            // column iterator (looks at column left, on, and right of the current cell)
            for (int j = c - 1; j <= c + 1; j++){
                // if the column is on the board check, else ignore
                if (j >= 0 && j < boardSize){
                    // if the neighbor cell is alive, add 1 to the tally
                    if (i != r || j != c){
                        // don't count the cell itself
                        if (currentLife[i][j].getState()){
                            // if a cell is alive, add to the tally
                            neighborsAlive++;
                        }
                    }
                }
            }
        }
    }

    // call function to decide cells fate based on the game rules
    cellsFate(r, c, neighborsAlive);
}

/*** Friend Functions ***/

namespace csci2312 {

    // prints the game board to an ostream object (console or file)
    // PRECONDITION: board must be seeded
    //
    // POSTCONDITION: game will be formatted and printed to stream
    ostream &operator << (ostream &out, const GameOfLife &board) {
        for (int i = 0; i < board.boardSize; i++) {
            for (int j = 0; j < board.boardSize; j++) {
                out << board.currentLife[i][j];
            }
            out << std::endl;
        }

        out << std::endl;
        // sleep between printing to show board more clearly
        std::this_thread::sleep_for(std::chrono::milliseconds(550));
        return out;
    }

    // prints a cell to an ostream object (console or file)
    // PRECONDITION: cell must be initialized (game board must be created first)
    //
    // POSTCONDITION: cell will be formatted and printed to stream
    ostream& operator << (ostream& out, const Cell& cell) {
        if (cell.getState()) {
            out << cell.alive;
        } else {
            out << cell.dead;
        }

        return out;
    }
}



