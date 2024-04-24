//
// Created by wikto on 23/04/2024.
//

#include "Bug.h"
int BOARD_WIDTH = 10;
int BOARD_HEIGHT = 10;

bool Bug::isWayBlocked() const {
    switch (direction) {
        case Direction::North:
            return position.second == 0;
        case Direction::East:
            return position.first == BOARD_WIDTH - 1;
        case Direction::South:
            return position.second == BOARD_HEIGHT - 1;
        case Direction::West:
            return position.first == 0;
        default:
            return false;
    }
}

// Other member functions' definitions
