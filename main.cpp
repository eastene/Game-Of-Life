#include <iostream>
#include "GameOfLife_csci2312.h"

int main() {

    csci2312::GameOfLife game (10);

    game.seedBoard("seed_file.txt");

    game.run(20);
    return 0;
}