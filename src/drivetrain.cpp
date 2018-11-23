
#include "drivetrain.h"

void driveTrainThread(DriveTrain* dt);

DriveTrain::DriveTrain(DistanceFt _width, SpeedFtPerSec _maxSpeed):
    calibrated(false),
    cmdQueue(nullptr),
    stop(true),
    queueThread(nullptr)
{
}

DriveTrain::DriveTrain(DistanceFt _width, SpeedFtPerSec _maxSpeed):
    calibrated(false),
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
    if (!stop)
    {
        stop = true;
        cmdNotEmpty.notify_all();
        lk.unlock();

        queueThread->join();
    }
    else
    {
        lk.unlock();
    }
}

void DriveTrain::set(float leftSpeed, float rightSpeed)
{

}

void DriveTrain::drive(SpeedFtPerSec speed, DistanceFt distance)
{
    if (!calibrated)
        return;

    float motorSpeed = speed / maxSpeed;
    TimeSec duration = distance / speed;

    Command* cmd = new Command(motorSpeed, motorSpeed, distance != 0 ? duration : 0);

    addToQueue(cmd);
}

void DriveTrain::turnInPlace(SpeedRadPerSec speed, AngleRad angle)
{
    if (!calibrated)
        return;

    SpeedFtPerSec rectSpeed = speed * width / 2;
    float motorSpeed = rectSpeed / maxSpeed;
    TimeSec duration = angle / speed;

    Command* cmd = new Command(motorSpeed, -motorSpeed, angle != 0 ? duration : 0);

    addToQueue(cmd);
}

void DriveTrain::addToQueue(Command* cmd)
{
    std::unique_lock<std::mutex> lk(dt->cmdMutex);

    if (!cmdQueue)
    {
        cmdQueue = cmd;
    }
    else
    {
        Command prev = cmdQueue;

        while (prev->next)
        {
            prev = prev->next;
        }

        prev->next = cmd;
    }

    lk.unlock();
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
