#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include "Teleporter.h"
#include "SuperBug.h"
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int BOARD_SIZE = 10;
const int TILE_SIZE = WINDOW_WIDTH / BOARD_SIZE;
void displayAllCells(const std::vector<std::vector<std::vector<Bug*>>>& board) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            std::cout << "(" << x << "," << y << "): ";
            if (board[x][y].empty()) {
                std::cout << "empty";
            } else {
                for (size_t i = 0; i < board[x][y].size(); ++i) {
                    Bug* bug = board[x][y][i];
                    std::string type = "Unknown";

                    if (dynamic_cast<Crawler*>(bug)) {
                        type = "Crawler";
                    } else if (dynamic_cast<Hopper*>(bug)) {
                        type = "Hopper";
                    } else if (dynamic_cast<Teleporter*>(bug)) {
                        type = "Teleporter";
                    }

                    if (i > 0) std::cout << ", ";
                    std::cout << type << " " << bug->getId() << " " << (bug->isAlive() ? "Alive" : "Dead");
                }
            }
            std::cout << std::endl;
        }
    }
}
int countAliveBugs(const std::vector<Bug*>& bug_vector) {
    int aliveCount = 0;
    for (auto& bug : bug_vector) {
        if (bug->isAlive()) {
            aliveCount++;
        }
    }
    return aliveCount;
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
        if (bug->isAlive()) {
            int x = bug->getX();
            int y = bug->getY();
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                board[x][y].push_back(bug);
            }
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
    std::cout << "Processing bug conflicts...\n";

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            auto& cell = board[x][y];
            if (cell.size() > 1) {
                Bug* largestBug = nullptr;
                int maxSize = -1;
                std::vector<Bug*> largestBugs;

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

                largestBug = largestBugs.empty() ? nullptr : (largestBugs.size() == 1 ? largestBugs[0] : largestBugs[rand() % largestBugs.size()]);

                if (largestBug) {
                    std::cout << "Largest bug in cell (" << x << ", " << y << ") is Bug ID " << largestBug->getId() << " with size " << largestBug->getSize() << "\n";
                    for (Bug* bug : cell) {
                        if (bug != largestBug && bug->isAlive()) {
                            std::cout << "Bug ID " << bug->getId() << " is eaten by Bug ID " << largestBug->getId() << "\n";
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
        if (bug->isAlive()) {
            bug->move();
        }
    }
    updateBoard(board, bug_vector);
    BugEating(board);
}
void displayBugs(const std::vector<Bug*>& bug_vector){
    std::cout << "Bugs:" << std::endl;
    for (const Bug* bug : bug_vector) {
        // Attempt to cast Bug to each derived type
        const Crawler* crawler = dynamic_cast<const Crawler*>(bug);
        const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
        const Teleporter* teleporter = dynamic_cast<const Teleporter*>(bug);

        std::cout << " " << bug->getId() << " ";

        if (crawler) {
            std::cout << "Crawler";
        } else if (hopper) {
            std::cout << "Hopper";
        } else if (teleporter) {
            std::cout << "Teleporter";
        } else {
            std::cout << "Unknown Type";
        }

        std::cout << " (" << bug->getX() << "," << bug->getY() << ") " << bug->getSize()
                  << " " << directionToString(bug->getDirection());

        if (hopper) {
            std::cout << " " <<hopper->getHopLength();
        }

        std::cout << " " << (bug->isAlive() ? "Alive" : "Dead") << std::endl;
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

void FindBugByID(const std::vector<Bug*>& bug_vector, int bugID) {
    bool found = false;

    for (const Bug* bug : bug_vector) {
        const Crawler* crawler = dynamic_cast<const Crawler*>(bug);
        const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
        const Teleporter* teleporter = dynamic_cast<const Teleporter*>(bug);
        if (bug->getId() == bugID) {
            std::cout << " " << bug->getId() << " ";
            if (crawler) {
                std::cout << "Crawler";
            } else if (hopper) {
                std::cout << "Hopper";
            } else if (teleporter) {
                std::cout << "Teleporter";
            }
            std::cout << " (" << bug->getX() << "," << bug->getY() << ") " << bug->getSize()
                      << " " << directionToString(bug->getDirection())
                      << " " << (bug->isAlive() ? "Alive" : "Dead");
            if (const Hopper* hopper = dynamic_cast<const Hopper*>(bug)) {
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


void runSimulation(std::vector<Bug*>& bug_vector, std::vector<std::vector<std::vector<Bug*>>>& board) {
    bool gameInProgress = true;

    while (gameInProgress) {
        tapBoard(bug_vector, board);
        displayAllCells(board);

        // Check for game over conditions
        int aliveCount = 0;
        Bug* lastStanding = nullptr;
        for (auto& bugs : bug_vector) {
            if (bugs->isAlive()) {
                aliveCount++;
                lastStanding = bugs;
            }
        }

        if (aliveCount <= 1) {
            gameInProgress = false;
            std::cout << "Game Over. ";
            if (lastStanding) {
                std::cout << "Last bug standing: Bug ID " << lastStanding->getId() << "\n";
            } else {
                std::cout << "No bugs are alive.\n";
            }
        }

        std::chrono::seconds dura( 1);
        std::this_thread::sleep_for( dura );
    }

    writeLifeHistoryToFile(bug_vector);
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
        }else if (type == 'T'){
            bug_vector.push_back(new Teleporter(id, x, y, static_cast<Direction>(dir), size));
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
        std::cout << "7. Run Simulation\n";
        std::cout << "8. Run SFML\n";
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
                FindBugByID(bug_vector, bugID);
                break;
            }
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
            case 7:
                runSimulation(bug_vector, board);
                break;
            case 8: {
                // Load textures
                sf::Texture crawlerTexture, hopperTexture, teleporterTexture, SuperBugTexture;
                if (!crawlerTexture.loadFromFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\crawler.png") ||
                    !hopperTexture.loadFromFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\hopper.png") ||
                    !teleporterTexture.loadFromFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\teleporter.png") ||
                    !SuperBugTexture.loadFromFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\super.png")) {
                    std::cerr << "Error loading textures\n";
                    return -1;
                }
                SuperBug superBug(121, 2, 3, Direction::North, 10);
                bug_vector.push_back(&superBug);
                float scale = 32.0f / (float)teleporterTexture.getSize().x;
                sf::Sprite crawlerSprite(crawlerTexture);
                sf::Sprite hopperSprite(hopperTexture);
                sf::Sprite teleporterSprite(teleporterTexture);
                sf::Sprite SuperBugSprite(SuperBugTexture);

                crawlerSprite.setScale(scale, scale);
                hopperSprite.setScale(scale, scale);
                teleporterSprite.setScale(scale, scale);
                SuperBugSprite.setScale(scale, scale);

                // Create window
                sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "BugLife Simulation");

                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        superBug.setDirection(Direction::North);
                        superBug.move();
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        superBug.setDirection(Direction::South); //
                        superBug.move();
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        superBug.setDirection(Direction::West); //
                        superBug.move();
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        superBug.setDirection(Direction::East); //
                        superBug.move();
                    }

                    tapBoard(bug_vector, board);
                    updateBoard(board, bug_vector);
                    BugEating(board);

                    int aliveCount = countAliveBugs(bug_vector);
                    if (aliveCount <= 1) {

                        if (aliveCount == 1) {

                            for (auto& bug : bug_vector) {
                                if (bug->isAlive()) {
                                    std::cout << "Last bug standing: Bug ID " << bug->getId() << "\n";
                                    break;
                                }
                            }
                        } else {
                            std::cout << "No bugs are alive.\n";
                        }
                        window.close();
                        break;
                    }

                    window.clear();

                    // Draw all bugs
                    for (auto &column : board) {
                        for (auto &cell : column) {
                            for (auto *bug : cell) {
                                if (bug->isAlive()) {
                                    sf::Sprite* spriteToUse = nullptr;
                                    if (dynamic_cast<Crawler*>(bug)) {
                                        spriteToUse = &crawlerSprite;
                                    } else if (dynamic_cast<Hopper*>(bug)) {
                                        spriteToUse = &hopperSprite;
                                    } else if (dynamic_cast<Teleporter*>(bug)) {
                                        spriteToUse = &teleporterSprite;
                                    }

                                    if (spriteToUse) {
                                        spriteToUse->setPosition(static_cast<float>(bug->getX()) * TILE_SIZE,
                                                                 static_cast<float>(bug->getY()) * TILE_SIZE);
                                        window.draw(*spriteToUse);
                                    }
                                }
                            }
                        }
                    }
                    if (superBug.isAlive()) {
                        SuperBugSprite.setPosition(static_cast<float>(superBug.getX()) * TILE_SIZE,
                                                   static_cast<float>(superBug.getY()) * TILE_SIZE);
                        window.draw(SuperBugSprite);
                    }

                    window.display();

                    // Sleep to simulate time step
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
                break;
        }
    } while (choice != 5);


    for (auto bug : bug_vector) {
        delete bug;
    }

    return 0;
}

