//
// Created by wikto on 23/04/2024.
//

#include "Hopper.h"
#include <cstdlib>
#include <ctime>
//hopper.cpp
Hopper::Hopper(int id, int x, int y, Direction dir, int size, int hopLength)
        : Bug(id, x, y, dir, size), hopLength(hopLength) {

}


void Hopper::move() {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    int BOARD_WIDTH = 10;
    int BOARD_HEIGHT = 10;
    bool isMoved = false;
    while (!isMoved) {
        if (!isWayBlocked()) {
            switch (direction) {
                case Direction::North:
                    position.second = std::max(0, position.second - hopLength);
                    break;
                case Direction::East:
                    position.first = std::min(BOARD_WIDTH - 1, position.first + hopLength);
                    break;
                case Direction::South:
                    position.second = std::min(BOARD_HEIGHT - 1, position.second + hopLength);
                    break;
                case Direction::West:
                    position.first = std::max(0, position.first - hopLength);
                    break;
            }
            isMoved = true;
        } else {
            direction = static_cast<Direction>((std::rand() % 4) + 1);
        }

    }
    addToPath();

}
