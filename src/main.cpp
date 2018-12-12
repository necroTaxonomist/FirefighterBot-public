
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

DriveTrain dt;
Detector detector;
Pump pump;

void exitControlThread();
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

	dt.calibrate(2.25,360);

    for (;;)
    {
        AngleDeg angleToFire;

        // Until fire is found
        for (bool found = false; !found;)
        {
            if (exit.load())
                goto done;

            // Turn 20 degrees in place
            dt.turnInPlace(360, 20, true);

            // Wait a litle bit
            sleep(1);

            // Check for fire
            found = detector.checkForFire(&angleToFire);
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
            dt.drive(1, .25, true);

            // Wait a litle bit
            sleep(1);

            // Check for fire
            found = detector.checkForFire(&angleToFire);
        }

        // Deactivate the pump
        pump.deactivate();
    }

done:
    dt.forceStop();

    return 0;
}

void exitControlThread()
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
