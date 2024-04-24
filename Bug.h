//
// Created by wikto on 23/04/2024.
//
// Bug.h
#ifndef BUG_H
#define BUG_H

#include <utility>
#include <list>

enum class Direction {
    North = 1,
    East = 2,
    South = 3,
    West = 4
};

class Bug {
protected:
    int id;
    std::pair<int, int> position;
    Direction direction;
    int size;
    bool alive;
    std::list<std::pair<int, int>> path;

public:
    Bug(int id, int x, int y, Direction dir, int size);
    virtual ~Bug() = default;

    virtual void move() = 0;
    bool isWayBlocked() const;
    // ... other member functions as needed

};

#endif // BUG_H
