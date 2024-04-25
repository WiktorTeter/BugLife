//
// Created by wikto on 25/04/2024.
//

#ifndef BUGLIFE_TELEPORTER_H
#define BUGLIFE_TELEPORTER_H
#include "Bug.h"
class Teleporter : public Bug {
public:
    using Bug::Bug;
    void move() override;

};
#endif //BUGLIFE_TELEPORTER_H
