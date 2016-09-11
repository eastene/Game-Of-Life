// FILE GameOfLife.h
// CLASSES: GamOfLife, Cell(in the namespace csci2312)
//
// DESCRIPTION: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
// The universe of the Game of Life is an infinite two - dimensional orthogonal grid of square cells, 
// each of which is in one of two possible states, alive or dead, or "populated" or "unpopulated"
// Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.
// At each step in time, the following transitions occur:
//
// Any live cell with fewer than two live neighbours dies, as if caused by under-population.
// Any live cell with two or three live neighbours lives on to the next generation.
// Any live cell with more than three live neighbours dies, as if by over-population.
// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
// 
// The initial pattern constitutes the seed of the system. 
// The first generation is created by applying the above rules simultaneously to every cell in the seed: 
// - births and deaths occur simultaneously, and the discrete moment at which this happes is sometimes called a tick. 
// In other words, each generation is a pure function of the preceding one. The rules continue to be applied 
// repeatedly to create further generations. 
//    
//   
////////////////  GameOfLife /////////////////////////
// HIGH LEVEL DESIGN
// State the key points of your high level design: 
// - What are the GameOfLife (GOL) class' responsibilities?
//   GOL houses the Cells in a matrix. 
//   It sets and executes all the rules on the Cells (future idea: imagine this responsibility given to a Cell)
// - What are the responsibilities of a Cell?
//   A Cell knows and can articulate its state (even if it is dead :) 
// - What data structure(s) of cells will you use to model and implement the board?
//   This implementation will use static two-dimensional arrays
// - How will you go about keeping track of the current and next cell populations?
//   One array will hold the current state, and the second array will hold the calculated next generation state. 
//   After the current state is displayed, the next state will be copied over the current state for subsequent display
//   [ OPTIONAL STUDENT TASK: briefly describe another, perhaps more efficient, implementation idea ]
// - How will you go about game boundary conditions (edges and corners)?
//   [ STUDENT TASK:  fill in your implementation approach ]
// - How will you handle error conditions (when a method fails in any way)?
//   A method with a known risk of failure will return a success/fail (zero/one) status.  
//   Every calling layer will propagate the status code to the outer layer, and the most outer layer (the main) will exit
//   
// A PRECONDITION states what must be true when/before a function is invoked. 
// The responsibility to meet/check the pre-condition belongs with the calling layer.
// If that condition is not met, the behavior of the called function is not predictable and may result in failure
//
// A POSTCONDITION states what is the function outcome, and if/how the state of the object(s) has changed
//
// =================== PUBLIC CLASS API - Cell
// CONSTRUCTORS:
//     Cell();
//     POSTCONDITION: Creates a dark/dead Cell
// 
// MODIFIERS/MUTATORS:
//     void setState(bool newState);
//     POSTCONDITION: Cell's state is set to the new state
//
// ACCESSORS:
//     bool getState();  
//     POSTCONDITION: Returns Cell's state.  True if alive/lit, and false if dead/dark
//
// =================== PRIVATE CLASS MEMBERS - Cell
//   
//     bool state;  
//
// =================== PUBLIC CLASS API - GameOfLife
// MEMBER CONSTANTS:
//   const static size_t MAX_BOARD
//   The max size of the visible gameboard.  The actual size will be passed into the game's constructor. 
//
// CONSTRUCTORS:
//     GameOfLife();
//       POSTCONDITION: Creates a maximum size matrix of dark cells
//
//     GameOfLife(size_t boardSize);
//       PRECONDITION: boardSize is not greater than MAX_BOARD  
//       POSTCONDITION: Creates a boardSize-size matrix of dark cells. Sets boardSize private member.
//
// MODIFIERS/MUTATORS:
//   
//   int seedBoard(string fileName);
//     FUNCIONALITY: Initializes the board; reads the configuration of lit from a text file. Returns 0/1 for sucsess/failure
//     POSTCONDITION: Board is successfully seeded with a known predictable pattern, method returns 0 (zero) to indicate success.  
//                    If the operation failed (could not open the file, wrong file format, etc), the function returns 1.
//
//   int seedBoard(size_t seeds);
//     FUNCTIONALITY:  Initializes the board with seeds-number of lit cells at randomly generated locations.  Returns 0/1 for sucess/failure
//     PRECONDITION: Number of seeds requested is not greater than maximum matrix capacity  
//     POSTCONDITION: Board is successfully seeded, method returns 0 (zero) to indicate success.  
//                    If the operation failed (too many seeds requested, etc), the function returns 1.//
//                   
//   void run( );
//     FUNCTIONALITY: Starts the simulation.  Asks a user if to generate another iteration, or if to end
//     POSTCONDITION: Simulation has run

//   void run(unsigned int numberOfIterations);
//     FUNCTIONALITY: Starts the simulation.  
//     POSTCONDITION: Simulation has run
//
// =================== PRIVATE CLASS MEMBERS - GameOfLife
//
//   void nextGeneration();
//     FUNCTIONALITY:  Calculates the next state of the population of Cells
//     POSTCONDITION: Next state has been calculated and is ready to display
//
// ===================  NON-MEMBER FUNCTIONS  
//
//   ostream& operator << (ostream& out, const GameOfLife& board)
//     POSTCONDITION: The game board has been displayed (printed to ostream out, which,
//     in turn, has been returned to the calling function)
//


#pragma once
#include <iostream>  // Provides ostream
#include <string>    // String operations
#include <cmath>     // Randomizer

namespace csci2312
{
	using std::string;
	using std::ostream;

	class Cell
	{
	
	public:
		static const char alive ='o';   // alive image
		static const char dead = '-';  // dead image
        
		Cell();
		Cell(bool state);  
		~Cell();

		// Accessors have no intention to modify the object, so it is a good practice to make them 'const' functions
		bool getState() const;

		void setState(bool newState);

		void displayBoard();

	private:
		bool state;
		char face;
        	
	};

    class GameOfLife
    {
		
    public:
		static const unsigned int MAX_BOARD = 30;
	
		GameOfLife();
		GameOfLife(size_t boardSize);
		~GameOfLife();
	
		int seedBoard(string fileName);
		int seedBoard(size_t seeds);
		void run();
		void run(unsigned int numberOfIterations);

		// A const(!) accessor method that returns a handle to the private currentLife array. 
		// The return type must also be 'const' because we return a pointer to a static array, and these are fixed
		// It is just an example.  It is not needed if we have a friend operator.		
		//const Cell** getCurrentLife() const  { return currentLife; };

		// friend operator can access private members of GameOfLife
		friend ostream& operator << (ostream& out, const GameOfLife& board);
		
    private:

        void cellsFate(int r, int c, int neighbors); // accessory function to checkCell
        void checkCell(int r, int c); // accessory function to nextGeneration
        void nextGeneration();

		Cell currentLife[MAX_BOARD][MAX_BOARD];
		Cell nextLife[MAX_BOARD][MAX_BOARD];

		// Example how to declare variable c as a pointer/handle to our array of Cells of size MAX_BOARD
		// The accessor method getCurrentLife() above uses the same syntax for the return type
		const Cell (*c)[MAX_BOARD] = currentLife;
	
	    size_t boardSize;       // Board size requested in the constructor	

    };
          
    // NON-MEMBER OUTPUT FUNCTIONS

    //  Display cell's state with alive/dead face
	ostream& operator << (ostream& out, const Cell& cell);

	
    //  Display current board.  Implementation needs to be moved to the .cpp file, only here temporarily to demo
	// ostream& operator << (ostream& out, const GameOfLife& board) {

		// Example of how to invoke the accessor method getCurrentLife() and access an element of the board
		// If we have an accessor, then the operator does not have to be a friend
		//out << board.getCurrentLife()[0][0];

		// If the operator is a friend, then any private data member (like currentLife)  can be accessed directly. 
		//out << board.currentLife[0][0];

		// Example how to access a cell's method.  
		// The board comes in to this operator function with a 'const' qualifier in front of the board, which means that
		// the function cannot alter the board.  Therefore only 'const' functions are allowed to operate on the baord. 
		// So getState() accessor must be a 'const' function - please see its declaration above
		//bool a = board.currentLife[1][2].getState()  ;

	//}
        
}
