//
// Created by wikto on 23/04/2024.
//
#ifndef CRAWLER_H
#define CRAWLER_H

#include "Bug.h"
class Crawler : public Bug {
public:
    using Bug::Bug;
    void move() override;
};

#endif // CRAWLER_H