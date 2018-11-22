
#pragma once

#include "units.h"

#include <thread>

//----
// Class: DriveTrain
// Description:
//      The drive train can be controlled in two ways:
//       - Manually setting the speed of the left and right motors with set()
//       - Sending queued commands with drive() and turnInPlace()
//----

class DriveTrain
{
public:
    DriveTrain(DistanceFt _width, SpeedFtPerSec _maxSpeed);
    ~DriveTrain();

    void set(float leftSpeed, float rightSpeed);

    void drive(SpeedFtPerSec speed, DistanceFt distance = 0);
    void turnInPlace(SpeedRadPerSec speed, AngleRad angle = 0);

private:
    struct Command
    {
        float leftSpeed;
        float rightSpeed;
        TimeSec duration;
    }

private:
    DistanceFt width;
    SpeedFtPerSec maxSpeed;
};
