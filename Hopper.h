//
// Created by wikto on 23/04/2024.
//

#ifndef HOPPER_H
#define HOPPER_H

#include "Bug.h"
//hopper.h
class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, int x, int y, Direction dir, int size, int hopLength);
    void move() override;
    // ... other member functions specific to Hopper
};

#endif // HOPPER_H
