//
// Created by wikto on 23/04/2024.
//

#include "Hopper.h"

Hopper::Hopper(int id, int x, int y, Direction dir, int size, int hopLength)
        : Bug(id, x, y, dir, size), hopLength(hopLength) {
    // Constructor specific to Hopper
}

void Hopper::move() {
    // Implement Hopper-specific move logic
    // ...
}

// Other member functions' definitions specific to Hopper
