#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
// Function to convert Direction enum value to string
std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::North: return "North";
        case Direction::East: return "East";
        case Direction::South: return "South";
        case Direction::West: return "West";
        default: return "Unknown";
    }
}
int main() {
    std::ifstream inFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\bugs.txt");
    if (!inFile) {
        std::cerr << "Unable to open file bugs.txt";
        return 1; // Exit if the file cannot be opened
    }

    std::vector<Bug*> bug_vector; // Vector to store pointers to Bug objects
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        char type;
        int id, x, y, dir, size, hopLength = 0; // Initialize hopLength to 0
        char delimiter;

        // Parse the line
        iss >> type >> delimiter >> id >> delimiter >> x >> delimiter >> y >> delimiter >> dir >> delimiter >> size;

        // Create Bug object based on type
        if (type == 'C') {
            bug_vector.push_back(new Crawler(id, x, y, static_cast<Direction>(dir), size));
        } else if (type == 'H') {
            iss >> delimiter >> hopLength;
            bug_vector.push_back(new Hopper(id, x, y, static_cast<Direction>(dir), size, hopLength));
        }
    }

    // Display the number of bugs read
    std::cout << "Number of bugs read: " << bug_vector.size() << std::endl;
    // Display bugs
    std::cout << "Bugs:" << std::endl;
    for (const Bug* bug : bug_vector) {
        std::cout << " " << bug->getId() << " " <<(dynamic_cast<const Crawler*>(bug) ? "Crawler" : "Hopper")
                  << " (" << bug->getX() << "," << bug->getY() << ") " << bug->getSize()
                  << " " << directionToString(bug->getDirection())
                  << " " << (bug->isAlive() ? "Alive" : "Dead");
        if (auto* hopper = dynamic_cast<const Hopper*>(bug)) {
            std::cout << " "<<hopper->getHopLength();
        }
        std::cout << std::endl;
    }

    // Don't forget to free memory for dynamically allocated objects
    for (auto bug : bug_vector) {
        delete bug;
    }

    return 0;
}
