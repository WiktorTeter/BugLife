#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include <random>
#include <ctime>
const int BOARD_SIZE = 10;
void displayAllCells(const std::vector<std::vector<std::vector<Bug*>>>& board) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            std::cout << "(" << x << "," << y << "): ";
            if (board[x][y].empty()) {
                std::cout << "empty";
            } else {
                for (size_t i = 0; i < board[x][y].size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << (dynamic_cast<Crawler*>(board[x][y][i]) ? "Crawler " : "Hopper ")
                              << board[x][y][i]->getId();
                }
            }
            std::cout << std::endl;
        }
    }
}
void clearBoard(std::vector<std::vector<std::vector<Bug*>>>& board) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            board[x][y].clear();
        }
    }
}
void updateBoard(std::vector<std::vector<std::vector<Bug*>>>& board, const std::vector<Bug*>& bug_vector) {
    clearBoard(board);
    for (auto bug : bug_vector) {
        int x = bug->getX();
        int y = bug->getY();
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            board[x][y].push_back(bug);
        }
    }
}

std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::North: return "North";
        case Direction::East: return "East";
        case Direction::South: return "South";
        case Direction::West: return "West";
        default: return "Unknown";
    }
}

void BugEating(std::vector<std::vector<std::vector<Bug*>>>& board) {
    std::srand(std::time(nullptr));  // Seed for random number generation

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            auto& cell = board[x][y];
            if (cell.size() > 1) {
                Bug* largestBug = nullptr;
                int maxSize = 0;
                std::vector<Bug*> largestBugs;

                // Determine the largest bug(s)
                for (Bug* bug : cell) {
                    if (bug->isAlive()) {
                        if (bug->getSize() > maxSize) {
                            largestBugs.clear();
                            maxSize = bug->getSize();
                            largestBugs.push_back(bug);
                        } else if (bug->getSize() == maxSize) {
                            largestBugs.push_back(bug);
                        }
                    }
                }

                if (largestBugs.size() == 1) {
                    largestBug = largestBugs.front();
                } else if (largestBugs.size() > 1) {
                    // Resolve ties randomly
                    largestBug = largestBugs[std::rand() % largestBugs.size()];
                }

                // Process eating
                if (largestBug) {
                    for (Bug* bug : cell) {
                        if (bug != largestBug && bug->isAlive()) {
                            largestBug->setSize(largestBug->getSize() + bug->getSize());
                            bug->setAlive(false);
                        }
                    }
                }
            }
        }
    }
}
void tapBoard(std::vector<Bug*>& bug_vector, std::vector<std::vector<std::vector<Bug*>>>& board) {
    for (Bug* bug : bug_vector) {
        bug->move();
    }
    updateBoard(board, bug_vector);  // Make sure to call this to refresh the board
    BugEating(board);         // Resolve any conflicts after movement

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

void writeLifeHistoryToFile(const std::vector<Bug*>& bug_vector) {
    std::ofstream outFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\bugs_life_history_date_time.out");
    if (!outFile) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }

    for (const Bug* bug : bug_vector) {
        outFile << "Bug ID: " << bug->getId() << " Path: ";
        for (const auto& pos : bug->getPath()) {
            outFile << "(" << pos.first << "," << pos.second << "), ";
        }
        outFile << (bug->isAlive() ? "Alive!" : "Dead!") << std::endl;
    }

    outFile.close();
    std::cout << "Life history written to 'bugs_life_history.out'\n";
}
int main() {
    std::ifstream inFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\bugs.txt");
    if (!inFile) {
        std::cerr << "Unable to open file bugs.txt";
        return 1;
    }

    std::vector<Bug*> bug_vector;
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        char type;
        int id, x, y, dir, size, hopLength = 0;
        char delimiter;

        iss >> type >> delimiter >> id >> delimiter >> x >> delimiter >> y >> delimiter >> dir >> delimiter >> size;


        if (type == 'C') {
            bug_vector.push_back(new Crawler(id, x, y, static_cast<Direction>(dir), size));
        } else if (type == 'H') {
            iss >> delimiter >> hopLength;
            bug_vector.push_back(new Hopper(id, x, y, static_cast<Direction>(dir), size, hopLength));
        }
    }
    inFile.close();
    std::vector<std::vector<std::vector<Bug*>>> board(BOARD_SIZE, std::vector<std::vector<Bug*>>(BOARD_SIZE));

    for (auto bug : bug_vector) {
        int x = bug->getX();
        int y = bug->getY();
        if (x < BOARD_SIZE && y < BOARD_SIZE) {
            board[x][y].push_back(bug);
        }
    }
    updateBoard(board, bug_vector);
    int choice;
    do {
        std::cout << "\n--- Bug Management Menu ---\n";
        std::cout << "1. Display All Bugs\n";
        std::cout << "2. Display Bug by ID\n";
        std::cout << "3. Tap the Bug Board\n";
        std::cout << "4. Show Bug Paths\n";
        std::cout << "5. Exit and write bugs path to file\n";
        std::cout << "6. Display All Cells\n";
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
                tapBoard(bug_vector, board);
                break;
            case 4:
                displayLifeHistory(bug_vector);
                break;
            case 5:
                writeLifeHistoryToFile(bug_vector);
                std::cout << "Exiting the program.\n";
                break;
            case 6:
                displayAllCells(board);
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);


    for (auto bug : bug_vector) {
        delete bug;
    }

    return 0;
}

