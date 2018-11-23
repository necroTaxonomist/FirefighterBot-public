
#include "units.h"
#include "drivetrain.h"

#include <iostream>

DriveTrain dt(1,1);

int main(int argc, char** argv)
{
    int t = 0;

    for(;;)
    {
        std::cin >> t;

        if (t < 0)
            break;

        dt.drive(1, t, true);
    }

    dt.forceStop();

    return 0;
}
