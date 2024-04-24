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
    inFile.close();

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
    int bugID;
    std::cout << "Enter bug ID: ";
    std::cin >> bugID;
    bool found = false;

    for (const Bug* bug : bug_vector) {
        if (bug->getId() == bugID) {
            std::cout << "Bug Found: " << bug->getId() << " " <<(dynamic_cast<const Crawler*>(bug) ? "Crawler" : "Hopper")
                      << " (" << bug->getX() << "," << bug->getY() << ") " << bug->getSize()
                      << " " << directionToString(bug->getDirection())
                      << " " << (bug->isAlive() ? "Alive" : "Dead");
            if (const auto* hopper = dynamic_cast<const Hopper*>(bug)) {
                std::cout << " " << hopper->getHopLength();
            }
            std::cout << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Bug " << bugID << " not found." << std::endl;
    }
    // Move all bugs
    std::cout << "Tapping the Bug Board..." << std::endl;
    for (Bug* bug : bug_vector) {
        bug->move();  // Call the move method for each bug
    }

    // Display bugs after movement
    for (const Bug* bug : bug_vector) {
        std::cout << "Bug ID: " << bug->getId() << " at Position: (" << bug->getX() << "," << bug->getY() << ")" << std::endl;
    }


    // Don't forget to free memory for dynamically allocated objects
    for (auto bug : bug_vector) {
        delete bug;
    }

    return 0;
}
