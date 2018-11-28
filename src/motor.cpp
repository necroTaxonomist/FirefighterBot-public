
#include "motor.h"

#include "pinout.h"

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
}

void Motor::set(const float& speed)
{
    digitalWrite(fwdPin, SIGN(speed) < 0);
    digitalWrite(revPin, SIGN(speed) > 0);
    pwmWrite(pwmPin, ABS(speed));
}

Motor& Motor::operator=(const float& rhs)
{
    set(rhs);
    return *this;
}
