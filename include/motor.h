
#pragma once

class Motor
{
public:
    Motor(int _pwmPin, int _reversePin);
    ~Motor();

    void set(const float& speed);

    Motor& operator=(const float& rhs);

private:
    int pwmPin;
    int reversePin;
};
