
#include "drivetrain.h"

#include <chrono>
#include <iostream>

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

void driveTrainThread(DriveTrain* dt);

DriveTrain::DriveTrain():
    calibrated(false),
    cmdQueue(nullptr),
    stop(true),
    queueThread(nullptr)
{
}

DriveTrain::DriveTrain(DistanceFt _width, SpeedFtPerSec _maxSpeed):
    calibrated(true),
    width(_width),
    maxSpeed(_maxSpeed),
    cmdQueue(nullptr),
    stop(false)
{
    maxAngularSpeed = 2 * maxSpeed / width;

    queueThread = std::unique_ptr<std::thread>(new std::thread(driveTrainThread, this));
}

DriveTrain::~DriveTrain()
{
    std::unique_lock<std::mutex> lk(cmdMutex);
    if (!stop)
    {
        stop = true;
        cmdNotEmpty.notify_all();

        cmdCancel = true;
        cmdCancelCond.notify_one();

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
    setMotors(leftSpeed, rightSpeed);

    std::unique_lock<std::mutex> lk(cmdMutex);

    cmdCancel = true;
    cmdCancelCond.notify_one();

    lk.unlock();

    // Set twice just in case
    setMotors(leftSpeed, rightSpeed);
}

void DriveTrain::drive(SpeedFtPerSec speed, DistanceFt distance, bool wait)
{
    if (!calibrated)
        return;

    float motorSpeed = speed / maxSpeed;

    SpeedFtPerSec realSpeed = MIN(ABS(speed), maxSpeed);
    TimeSec duration = distance / realSpeed;

    Command* cmd = new Command(motorSpeed, motorSpeed, distance != 0 ? duration : 0, wait);

    addToQueue(cmd, true);

    if (wait)
    {
        cmd->waitUntilDone();
        delete cmd;
    }
}

void DriveTrain::turnInPlace(SpeedRadPerSec speed, AngleRad angle, bool wait)
{
    if (!calibrated)
        return;

    SpeedFtPerSec rectSpeed = speed * width / 2;
    float motorSpeed = rectSpeed / maxSpeed;

    SpeedRadPerSec realSpeed = MIN(ABS(speed), maxAngularSpeed);
    TimeSec duration = angle / realSpeed;

    Command* cmd = new Command(motorSpeed, -motorSpeed, angle != 0 ? duration : 0, wait);

    addToQueue(cmd, true);

    if (wait)
    {
        cmd->waitUntilDone();
        delete cmd;
    }
}

void DriveTrain::setMotors(float leftSpeed, float rightSpeed)
{
    std::unique_lock<std::mutex> lk(motorsMutex);

    std::cout << "Set motors (" << leftSpeed << "," << rightSpeed << ")\n";

    lk.unlock();
}

void DriveTrain::addToQueue(Command* cmd, bool lock)
{
    std::unique_lock<std::mutex> lk(cmdMutex, std::defer_lock);

    if (lock)
        lk.lock();

    if (cmdQueue == nullptr)
    {
        cmdQueue = cmd;
        cmd->next = nullptr;
    }
    else
    {
        Command* prev = cmdQueue;

        while (prev->next)
        {
            prev = prev->next;
        }

        prev->next = cmd;
        cmd->next = nullptr;
    }

    cmdNotEmpty.notify_one();

    if (lock)
        lk.unlock();
}

void DriveTrain::takeFromQueue(Command** cmd, bool lock)
{
    if (!cmd)
        return;

    std::unique_lock<std::mutex> lk(cmdMutex, std::defer_lock);

    if (lock)
        lk.lock();

    *cmd = cmdQueue;

    if (cmdQueue != nullptr)
        cmdQueue = cmdQueue->next;

    if (lock)
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

            if (dt->cmdQueue != nullptr)
                break;
            else
            {
                dt->cmdNotEmpty.wait(lk);
            }
        }

        // Run a single command off the queue
        DriveTrain::Command* cmd = nullptr;
        dt->takeFromQueue(&cmd, false);

        if (cmd) // This should always be true but you never know
        {
            dt->setMotors(cmd->leftSpeed, cmd->rightSpeed);

            if (cmd->duration > 0)
            {
                auto targetTime = std::chrono::system_clock::now() + std::chrono::milliseconds((long int)(cmd->duration * 1000));

                // Wait for the required duration
                // Or until the command is cancelled
                dt->cmdCancel = false;
                while (!dt->cmdCancel && std::chrono::system_clock::now() < targetTime)
                {
                    dt->cmdCancelCond.wait_until(lk, targetTime);
                }

                if (dt->cmdCancel)
                {
                    dt->cmdCancel = false;
                }
                else
                {
                    // Stop motors
                    dt->setMotors(0, 0);
                }
            }

            if (!cmd->wait)
            {
                delete cmd;
            }
            else
            {
                cmd->setDone();
            }
        }

        lk.unlock();
    }
}
