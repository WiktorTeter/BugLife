//
// Created by wikto on 23/04/2024.
//

#include "Bug.h"

Bug::Bug(int id, int x, int y, Direction dir, int size)
        : id(id), position(x, y), direction(dir), size(size), alive(true) {
    // Initial position is added to the path
    path.push_back(position);
}

bool Bug::isWayBlocked() const {
    // Implement logic to check if the way is blocked
    // ...
}

// Other member functions' definitions
