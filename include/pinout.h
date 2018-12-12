
#ifndef NO_PI

#include <wiringPi.h>

void gpioInit()
{
    static bool init = false;
    if (!init)
    {
        wiringPiSetupGpio();
        init = true;
    }
}

void gpioPinMode(int pin, int mode)
{
    pinMode(pin, mode);
}

void gpioDigitalWrite(int pin, int value)
{
    digitalWrite(pin, value);
}

void gpioPwmWrite(int pin, float value)
{
    pwmWrite(pin, value * 1024);
}

#else

#define INPUT 0
#define OUTPUT 0
#define PWM_OUTPUT 0

void gpioInit()
{
}

void gpioPinMode(int pin, int mode)
{
}

void gpioDigitalWrite(int pin, int value)
{
}

void gpioPwmWrite(int pin, float value)
{
}

#endif
