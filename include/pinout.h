
#include <wiringPi.h>

void pinoutInit()
{
    static bool init = false;
    if (!init)
    {
        wiringPiSetupGpio();
        init = true;
    }
}
