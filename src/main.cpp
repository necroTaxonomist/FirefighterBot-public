
#include "units.h"
#include "drivetrain.h"
#include "detector.h"
#include "pump.h"

#include <iostream>

#include <thread>
#include <mutex>
#include <atomic>

#define SIGN(X) ((X) == 0 ? 0 : ((X) > 0 ? 1 : -1))
#define ABS(X) ((X) * SIGN(X))

#define MIN_FIRE_TEMP 80

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
std::atomic<TempF> fireTemp(0);

Pump pump;

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
            TempF temp;

            // Wait until a fire is found
            bool foundFire = detector.waitForFire(&angle, &temp, true);
            angleToFire.store(angle);
            fireTemp.store(temp);

            if (foundFire)
            {
                setMode(SUPPRESSION);
            }
        }
        else if (curMode == SUPPRESSION)
        {
            AngleDeg angle;
            TempF temp;

            // Poll the detector for fire
            bool foundFire = detector.checkForFire(&angle, &temp);
            angleToFire.store(angle);
            fireTemp.store(temp);

            if (!foundFire)
            {
                // Deactivate the pump when the fire is gone
                pump.deactivate();
                setMode(PATROL);
            }
            else if (temp >= MIN_FIRE_TEMP)
            {
                // Activate the pump when close enough
                pump.activate();
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
            dt.drive(1, 2);
            dt.turnInPlace(90, -90);
            dt.drive(1, 2);
            dt.turnInPlace(90, -90);
            dt.drive(1, 2);
            dt.turnInPlace(90, -90);
            dt.drive(1, 2);
            dt.turnInPlace(90, -90, true); // wait on the last command
        }
        else if (curMode == SUPPRESSION)
        {
            if (angleToFire.load() != 0)
            {
                // turn towards the fire
                dt.turnInPlace(90, angleToFire, true);
            }

            if (fireTemp.load() < MIN_FIRE_TEMP)
            {
                // get close to the fire
                dt.drive(.25);
            }
            else
            {
                dt.forceStop();
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
