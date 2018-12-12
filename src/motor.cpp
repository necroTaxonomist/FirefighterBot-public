
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
    pinoutInit();

    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(fwdPin, OUTPUT);
    pinMode(revPin, OUTPUT);
}

Motor::~Motor()
{
    digitalWrite(fwdPin, 0);
    digitalWrite(revPin, 0);
    pwmWrite(pwmPin, 0);
}

void Motor::set(const float& speed)
{
    digitalWrite(fwdPin, SIGN(speed) < 0);
    digitalWrite(revPin, SIGN(speed) > 0);
    pwmWrite(pwmPin, ABS(speed)*1024);
}

Motor& Motor::operator=(const float& rhs)
{
    set(rhs);
    return *this;
}
