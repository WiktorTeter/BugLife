//
// Created by wikto on 25/04/2024.
//
//teleporter bug teleports randomly around the board
#include "Teleporter.h"
#include <cstdlib>
#include <ctime>
int BOARD_SIZE = 10;
void Teleporter::move() {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    int newX, newY;
    do {
        newX = std::rand() % BOARD_SIZE;
        newY = std::rand() % BOARD_SIZE;
    } while (newX == position.first && newY == position.second);

    position.first = newX;
    position.second = newY;
    addToPath();
}