//
// Created by wikto on 26/04/2024.
//
#include "SuperBug.h"

void SuperBug::move() {
    switch (direction) {
        case Direction::North:
            position.second -= 1;
            break;
        case Direction::East:
            position.first += 1;
            break;
        case Direction::South:
            position.second += 1;
            break;
        case Direction::West:
            position.first -= 1;
            break;
        default:
            break;
    }
}
void SuperBug::setDirection(Direction newDirection) {
    direction = newDirection;
}

SuperBug::SuperBug(int id, int x, int y, Direction dir, int size) : Bug(id, x, y, dir, size) {

}
