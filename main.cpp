#include <iostream>
#include "GameOfLife_csci2312.h"

int main() {

    csci2312::GameOfLife game (10);

    game.seedBoard(20);

    game.run(5);
    return 0;
}