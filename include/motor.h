
#pragma once

class Motor
{
public:
    Motor(/*pin initialization*/);
    ~Motor();

    void set(const float& speed);

    Motor& operator=(const float& rhs);
};
