
#pragma once

#include "units.h"

#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>

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
    DriveTrain();
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

        Command* next;

        Command(float _leftSpeed, float _rightSpeed, TimeSec _duration = 0):
            leftSpeed(_leftSpeed),
            rightSpeed(_rightSpeed),
            duration(_duration),
            next(nullptr)
        {
        }
    };

private:
    bool calibrated;

    DistanceFt width;
    SpeedFtPerSec maxSpeed;
    SpeedRadPerSec maxAngularSpeed;

    Command* cmdQueue;
    std::mutex cmdMutex;
    std::condition_variable cmdNotEmpty;
    bool stop;

    std::unique_ptr<std::thread> queueThread;

private:
    void addToQueue(Command* cmd);
    void takeFromQueue(Command** cmd);

    friend void driveTrainThread(DriveTrain* dt);
};
