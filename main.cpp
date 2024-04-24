#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"

int main() {
    std::ifstream inFile("C:\\Users\\wikto\\CLionProjects\\BugLife\\bugs.txt");
    if (!inFile) {
        std::cerr << "Unable to open file bugs.txt";
        return 1; // Exit if the file cannot be opened
    }
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
}
