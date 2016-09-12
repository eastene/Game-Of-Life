#include <iostream>
#include <sstream>
#include "GameOfLife_csci2312.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

void seedGame(int boardSize, csci2312::GameOfLife &game);
void runGame(csci2312::GameOfLife &game);

int main() {

    int boardSize = 0;
    string input;

    // menu to create and run game of life
    cout << "Enter a grid size (n x n) or leave blank to use default of 30 x 30:" << std::endl;
    getline(cin, input);

    if (input.empty())
    {
        csci2312::GameOfLife game;
        seedGame(30, game);
    }
    else
    {
        // create a game and transform the user input to int
        boardSize = std::stoi(input);
        csci2312::GameOfLife game (boardSize);
        seedGame(boardSize, game);
    }

    return 0;
}

void seedGame(int boardSize, csci2312::GameOfLife &game){
    string input;
    std::stringstream inCheck;
    int seedNum;

    cout << "Enter a number of cells to seed the game with (default is gird size), or enter a file to seed the board "
             << "with:" << endl;

    getline(cin, input);

    inCheck << input;
    inCheck >> seedNum;
    if (input.empty())
    {
        game.seedBoard(boardSize);
    }
    else if (inCheck.fail())
    {
        game.seedBoard(input);
    }
    else
    {
        game.seedBoard(seedNum);
    }

    runGame(game);

}

void runGame(csci2312::GameOfLife &game){
    string input;
    int iterations;

    do{
        cout << "Enter a number of iterations to run, or leave blank to run a single iteration (-1 to end):" << endl;

        getline(cin, input);

        if (input.empty())
        {
            game.run();
        }
        else if (input == "-1")
        {
            break;
        }
        else
        {
            iterations = std::stoi(input);
            game.run(iterations);
        }
    } while (input != "-1");

}