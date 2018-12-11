
#pragma once

#include "units.h"

class Detector
{
public:
    Detector();
    ~Detector();

    // Returns true if there is fire
    // Writes the distance and angle into the provided vars
    bool checkForFire(AngleDeg* angle = nullptr, TempF* temp = nullptr);

    // Same as check for fire, but blocks until there is/isn't fire
    bool waitForFire(AngleDeg* angle = nullptr, TempF* temp = nullptr, bool look = true);
};
