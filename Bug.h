// Bug.h
#ifndef BUG_H
#define BUG_H

#include <utility>  // For std::pair
#include <string>

enum class Direction {
    North = 1,
    East,
    South,
    West
};

class Bug {
protected:
    int id;
    std::pair<int, int> position;  // Using std::pair to store position as (x, y)
    Direction direction;
    int size;
    bool alive;

public:
    Bug(int id, int x, int y, Direction direction, int size) :
            id(id), position(x, y), direction(direction), size(size), alive(true) {}

    virtual ~Bug() {}

    // Getters
    int getId() const { return id; }
    std::pair<int, int> getPosition() const { return position; }
    int getX() const { return position.first; }
    int getY() const { return position.second; }
    Direction getDirection() const { return direction; }
    int getSize() const { return size; }
    bool isAlive() const { return alive; }

    // Setters
    void setPosition(int x, int y) { position = {x, y}; }
    void setDirection(Direction direction) { this->direction = direction; }
    void setSize(int size) { this->size = size; }
    void setAlive(bool alive) { this->alive = alive; }

    // Pure virtual move function
    virtual void move() = 0;
    bool isWayBlocked() const;
};

#endif // BUG_H
