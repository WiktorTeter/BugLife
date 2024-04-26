#ifndef BUGLIFE_SUPERBUG_H
#define BUGLIFE_SUPERBUG_H
#include "Bug.h"

class SuperBug : public Bug {
public:
    SuperBug(int id, int x, int y, Direction dir, int size);
    void move() override;
    void setDirection(Direction newDirection);
};

#endif //BUGLIFE_SUPERBUG_H