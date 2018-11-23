
#include "motor.h"

Motor::Motor(/*pin initialization*/)
{
}

Motor::~Motor()
{
}

void Motor::set(const float& speed)
{
}

Motor& Motor::operator=(const float& rhs)
{
    set(rhs);
    return *this;
}
