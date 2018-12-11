
#pragma once

#include "units.h"
#include "motor.h"

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
    ~DriveTrain();

    void calibrate(SpeedFtPerSec _driveRatio, SpeedDegPerSec _turnRatio);

    void set(float leftSpeed, float rightSpeed);

    void drive(SpeedFtPerSec speed, DistanceFt distance = 0, bool wait = false);
    void turnInPlace(SpeedDegPerSec speed, AngleDeg angle = 0, bool wait = false);

    void clearQueue();
    void forceStop();

private:
    struct Command
    {
        float leftSpeed;
        float rightSpeed;

        TimeSec duration;

        bool wait;
        bool done;
        std::mutex doneMutex;
        std::condition_variable doneCond;

        Command* next;

        Command(float _leftSpeed, float _rightSpeed, TimeSec _duration, bool _wait = false):
            leftSpeed(_leftSpeed),
            rightSpeed(_rightSpeed),
            duration(_duration),
            wait(_wait),
            done(false),
            next(nullptr)
        {
        }

        void setDone()
        {
            std::unique_lock<std::mutex> lk(doneMutex);

            done = true;
            doneCond.notify_all();

            lk.unlock();
        }

        void waitUntilDone()
        {
            if (!wait)
                return;

            std::unique_lock<std::mutex> lk(doneMutex);

            while (!done)
                doneCond.wait(lk);

            lk.unlock();
        }
    };

private:
    bool calibrated;

    SpeedFtPerSec driveRatio;
    SpeedDegPerSec turnRatio;

    Command* cmdQueue;
    std::mutex cmdMutex;
    std::condition_variable cmdNotEmpty;
    bool stop;

    bool cmdCancel;
    std::condition_variable cmdCancelCond;

    std::mutex motorsMutex;
    Motor leftMotor;
    Motor rightMotor;

    std::unique_ptr<std::thread> queueThread;

private:
    void setMotors(float leftSpeed, float rightSpeed);

    void addToQueue(Command* cmd, bool lock);
    void takeFromQueue(Command** cmd, bool lock);

    friend void driveTrainThread(DriveTrain* dt);
};
