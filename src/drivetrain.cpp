
#include "drivetrain.h"

void driveTrainThread(DriveTrain* dt);

DriveTrain::DriveTrain(DistanceFt _width, SpeedFtPerSec _maxSpeed):
    width(_width),
    maxSpeed(_maxSpeed),
    cmdQueue(nullptr),
    stop(false)
{
    queueThread = std::unique_ptr<std::thread>(new std::thread(driveTrainThread, this));
}

DriveTrain::~DriveTrain()
{
    std::unique_lock<std::mutex> lk(cmdMutex);
    stop = true;
    cmdNotEmpty.notify_all();
    lk.unlock();

    queueThread->join();
}

void DriveTrain::set(float leftSpeed, float rightSpeed)
{

}

void DriveTrain::drive(SpeedFtPerSec speed, DistanceFt distance)
{

}

void DriveTrain::turnInPlace(SpeedRadPerSec speed, AngleRad angle)
{

}

void driveTrainThread(DriveTrain* dt)
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(dt->cmdMutex);

        for (;;)
        {
            if (dt->stop)
            {
                lk.unlock();
                return;
            }

            if (dt->cmdQueue)
                break;
            else
                dt->cmdNotEmpty.wait(lk);
        }

        // Run a single command off the queue

        lk.unlock();
    }
}
