
#pragma once

#include "units.h"

class Detector
{
public:
    Detector();
    ~Detector();

    // Returns true if there is fire
    // Writes the distance and angle into the provided vars
    bool checkForFire(AngleRad* angle);

    // Same as check for fire, but blocks until there is/isn't fire
    bool waitForFire(AngleRad* angle, bool look = true);
};
