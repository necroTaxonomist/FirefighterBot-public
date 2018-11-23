
#include "units.h"
#include "drivetrain.h"

#include <iostream>

DriveTrain dt;

int main(int argc, char** argv)
{
    dt.calibrate(1,1);

    int t = 0;

    for(;;)
    {
        std::cin >> t;

        for (int i = 0; i < 4; ++i)
        {
            dt.drive(1, 3);
            dt.turnInPlace(1, PI / 2);
        }

        std::cin >> t;

        dt.forceStop();

        if (t < 0)
            break;
    }

    return 0;
}
