
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

        }
        else if (curMode == SUPPRESSION)
        {

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
