//
// Created by wikto on 23/04/2024.
//
#ifndef CRAWLER_H
#define CRAWLER_H

#include "Bug.h"

class Crawler : public Bug {
public:
    using Bug::Bug; // Inherit Bug's constructor
    void move() override;
    // ... other member functions specific to Crawler
};

#endif // CRAWLER_H