#include <iostream>
#include "GameOfLife_csci2312.h"

int main() {

    csci2312::GameOfLife game;

    game.run();

    std::cout << game;
    return 0;
}