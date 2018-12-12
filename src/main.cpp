
#include "units.h"
#include "drivetrain.h"
#include "detector.h"
#include "pump.h"

#include <iostream>

#include <thread>
#include <mutex>
#include <atomic>

#include <unistd.h>

#define SIGN(X) ((X) == 0 ? 0 : ((X) > 0 ? 1 : -1))
#define ABS(X) ((X) * SIGN(X))

//#define CALIBRATE_DRIVE
//#define CALIBRATE_TURN

#define DRIVE_RATIO 2.25
#define TURN_RATIO 360

#define WAIT_TIME 10

#define TURN_INC 30
#define APPROACH_INC .25

DriveTrain dt;
Detector detector;
Pump pump;

void exitThreadCB();
std::atomic<bool> exit(false);

void setMode(MovementMode newMode);

int main(int argc, char** argv)
{
#ifdef CALIBRATE_DRIVE
    dt.set(1,1);
    sleep(1);
    dt.set(0,0);
    return 0;
#endif
#ifdef CALIBRATE_TURN
    dt.set(-1,1);
    sleep(1);
    dt.set(0,0);
    return 0;
#endif

	dt.calibrate(DRIVE_RATIO, TURN_RATIO);

    std::thread exitThread(exitThreadCB);
    exitThread.detach();

    for (;;)
    {
        AngleDeg angleToFire;

        // Until fire is found
        for (bool found = false; !found;)
        {
            if (exit.load())
                goto done;

            // Turn in place
            dt.turnInPlace(360, TURN_INC, true);

            // Wait a litle bit
            sleep(WAIT_TIME);

            // Check for fire
            found = detector.checkForFire(angleToFire);
        }

        // Activate the pump
        pump.activate();

        // Until the fire is gone
        for (bool found = false; found;)
        {
            if (exit.load())
                goto done;

            // Adjust position to look at fire
            // Drive forward .25ft
            dt.turnInPlace(360, angleToFire);
            dt.drive(1, APPROACH_INC, true);

            // Wait a litle bit
            sleep(WAIT_TIME);

            // Check for fire
            found = detector.checkForFire(angleToFire);
        }

        // Deactivate the pump
        pump.deactivate();
    }

done:
    dt.forceStop();

    return 0;
}

void exitThreadCB()
{
    for (;;)
    {
        std::string input;
        std::cin >> input;

        if (input == "stop")
            break;
    }

    exit.store(true);
}
