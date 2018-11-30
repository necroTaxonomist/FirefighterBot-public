
#include "units.h"
#include "drivetrain.h"

#include <iostream>

#include <thread>
#include <mutex>

enum MovementMode
{
    PATROL,
    SUPPRESSION
};

MovementMode mode = PATROL;
std::mutex modeMutex;

DriveTrain dt;

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
			dt::set(.6, .75);
			if (/*fire image detected TODO*/)
			{
				setMode(SUPPRESSION);
			}
        }
        else if (curMode == SUPPRESSION)//assuming that when entering this mode, the fire is straight ahead
        {
			if (/*fire has been suppressed TODO*/)
			{
				st::drive(-1, 1, true); //back away from the extinguished fire
				setMode(PATROL);
			}
			else if (!/*temperature sensor picks up fire TODO*/)
			{
				dt::set(.5, .5);
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
