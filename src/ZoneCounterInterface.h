//
// Created by serdar on 6.10.2020.
//

#ifndef HELLO_WORLD_ZONECOUNTERINTERFACE_H
#define HELLO_WORLD_ZONECOUNTERINTERFACE_H
#include "MapInterface.h"

class ZoneCounterInterface {
    public:
        virtual ~ZoneCounterInterface() {}

        // Feeds map instance into solution class, and initialize.
        virtual void Init(MapInterface * map) = 0;

        // Counts zones in provided map, and return result.
        virtual int Solve() = 0;
};


#endif //HELLO_WORLD_ZONECOUNTERINTERFACE_H
