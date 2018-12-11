
#pragma once

#include "units.h"

class Detector
{
public:
    Detector();
    ~Detector();

    // Returns true if there is fire
    // Writes the distance and angle into the provided vars
    bool checkForFire(DistanceFt* dist, AngleRad* angle);

    // Same as check for fire, but blocks until there is fire
    bool waitForFire(DistanceFt* dist, AngleRad* angle);
};
