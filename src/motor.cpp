
#include "motor.h"

#include "pinout.h"

#include <cstdio>

#define SIGN(X) ((X) == 0 ? 0 : ((X) > 0 ? 1 : -1))
#define ABS(X) ((X) * SIGN(X))

Motor::Motor(int _pwmPin, int _fwdPin, int _revPin):
    pwmPin(_pwmPin),
    fwdPin(_fwdPin),
    revPin(_revPin)
{
    gpioInit();

    gpioPinMode(pwmPin, PWM_OUTPUT);
    gpioPinMode(fwdPin, OUTPUT);
    gpioPinMode(revPin, OUTPUT);
}

Motor::~Motor()
{
    gpioDigitalWrite(fwdPin, 0);
    gpioDigitalWrite(revPin, 0);
    gpioPwmWrite(pwmPin, 0);
}

void Motor::set(const float& speed)
{
    gpioDigitalWrite(fwdPin, SIGN(speed) < 0);
    gpioDigitalWrite(revPin, SIGN(speed) > 0);
    gpioPwmWrite(pwmPin, ABS(speed));
}

Motor& Motor::operator=(const float& rhs)
{
    set(rhs);
    return *this;
}
