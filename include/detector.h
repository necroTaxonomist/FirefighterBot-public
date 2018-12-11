
#pragma once

#include "units.h"

class Detector
{
public:
    Detector();
    ~Detector();

    // Returns true if there is fire
    // Writes the distance and angle into the provided vars
    bool checkForFire(AngleDeg* angle);

    // Same as check for fire, but blocks until there is/isn't fire
    bool waitForFire(AngleDeg* angle, bool look = true);
};
