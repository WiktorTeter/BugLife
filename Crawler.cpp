#include "Crawler.h"
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()

void Crawler::move() {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }


    while (isWayBlocked()) {
        direction = static_cast<Direction>((std::rand() % 4) + 1);
    }


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
    addToPath();
}


