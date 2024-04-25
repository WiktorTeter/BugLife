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

void displayBugs(const std::vector<Bug*>& bug_vector){
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
}

void displayLifeHistory(const std::vector<Bug*>& bug_vector) {
    for (const Bug* bug : bug_vector) {
        std::cout << "Bug ID: " << bug->getId() << " Path: ";
        for (const auto& pos : bug->getPath()) {
            std::cout << "(" << pos.first << "," << pos.second << "), ";
        }
        std::cout << (bug->isAlive() ? "Alive!" : "Dead!") << std::endl;
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


    int choice;
    do {
        std::cout << "\n--- Bug Management Menu ---\n";
        std::cout << "1. Display All Bugs\n";
        std::cout << "2. Display Bug by ID\n";
        std::cout << "3. Tap the Bug Board\n";
        std::cout << "4. Show Bug Paths\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayBugs(bug_vector);
                break;
            case 2:
            {
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
                    std::cout << "Bug " << bugID << " not found.\n";
                }
            }
                break;
            case 3:
                std::cout << "Tapping the Bug Board...\n";
                for (Bug* bug : bug_vector) {
                    bug->move();
                }
                break;
            case 4:
                displayLifeHistory(bug_vector);
                break;
            case 5:
                std::cout << "Exiting the program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    // Don't forget to free memory for dynamically allocated objects
    for (auto bug : bug_vector) {
        delete bug;
    }

    return 0;
}

