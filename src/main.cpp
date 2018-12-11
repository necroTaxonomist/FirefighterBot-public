
#include "units.h"
#include "drivetrain.h"
#include "detector.h"

#include <iostream>

#include <thread>
#include <mutex>
#include <atomic>

#define SIGN(X) ((X) == 0 ? 0 : ((X) > 0 ? 1 : -1))
#define ABS(X) ((X) * SIGN(X))

#define DESIRED_FIRE_DISTANCE .5

enum MovementMode
{
    PATROL,
    SUPPRESSION
};

MovementMode mode = PATROL;
std::mutex modeMutex;

DriveTrain dt(1,1);

Detector detector;
std::atomic<AngleDeg> angleToFire(0);

void controlThreadCB();
void moveThreadCB();

void setMode(MovementMode newMode);

int main(int argc, char** argv)
{
    std::thread moveThread(moveThreadCB);

    controlThreadCB();

    return 0;
}

void controlThreadCB()
{
    for (;;)
    {
        modeMutex.lock();
        MovementMode curMode = mode;
        modeMutex.unlock();

        if (curMode == PATROL)
        {
            AngleDeg angle;

            // Wait until a fire is found
            bool foundFire = detector.waitForFire(&angle, true);
            angleToFire.store(angle);

            if (foundFire)
            {
                setMode(SUPPRESSION);
            }
        }
        else if (curMode == SUPPRESSION)
        {
            AngleDeg angle;

            // Poll the detector for fire
            bool foundFire = detector.checkForFire(&angle, true);
            angleToFire.store(angle);

            if (!foundFire)
            {
                setMode(PATROL);
            }
        }
    }
}

void moveThreadCB()
{
    for (;;)
    {
        modeMutex.lock();
        MovementMode curMode = mode;
        modeMutex.unlock();

        if (curMode == PATROL)
        {
			// Drive in a 2x2ft square
            drive(1, 2);
            turnInPlace(90, -90);
            drive(1, 2);
            turnInPlace(90, -90);
            drive(1, 2);
            turnInPlace(90, -90);
            drive(1, 2);
            turnInPlace(90, -90, true); // wait on the last command
        }
        else if (curMode == SUPPRESSION)
        {
            if (angleToFire.load() != 0)
            {
                // turn towards the fire
                turnInPlace(90, angleToFire, true);
            }

            /*if (distToFire > DESIRED_FIRE_DISTANCE)
            {
                // get close to the fire
                drive(.5, distToFire - DESIRED_FIRE_DISTANCE, true);
                distToFire = DESIRED_FIRE_DISTANCE;
            }*/
        }
    }
}

void setMode(MovementMode newMode)
{
    modeMutex.lock()
    mode = newMode;
    dt.forceStop();
    modeMutex.unlock();
}
