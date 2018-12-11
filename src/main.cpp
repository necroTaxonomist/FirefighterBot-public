
#include "units.h"
#include "drivetrain.h"
#include "detector.h"

#include <iostream>

#include <thread>
#include <mutex>

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

DriveTrain dt;

Detector detector;
DistanceFt distToFire;
AngleRad angleToFire;

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
            // Look for fire
            bool foundFire = detector.waitForFire(&distToFire, &angleToFire, true);
            if (foundFire)
            {
                setMode(SUPPRESSION);
            }
        }
        else if (curMode == SUPPRESSION)
        {
            // Wait until the fire is gone
            bool foundFire = detector.waitForFire(nullptr, nullptr, false);
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
            turnInPlace(1, PI/2);
            drive(1, 2);
            turnInPlace(1, PI/2);
            drive(1, 2);
            turnInPlace(1, PI/2);
            drive(1, 2);
            turnInPlace(1, PI/2, true); // wait on the last command
        }
        else if (curMode == SUPPRESSION)
        {
            if (angleToFire != 0)
            {
                // turn towards the fire
                turnInPlace(1, angleToFire, true);
                angleToFire = 0;
            }

            if (distToFire > DESIRED_FIRE_DISTANCE)
            {
                // get close to the fire
                drive(.5, distToFire - DESIRED_FIRE_DISTANCE, true);
                distToFire = DESIRED_FIRE_DISTANCE;
            }
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
