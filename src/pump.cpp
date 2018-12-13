
#include "pump.h"
#include "pinout.h"

#include <iostream>

#define PUMP_PIN 6

Pump::Pump()
{
    gpioInit();
    gpioPinMod(PUMP_PIN, OUTPUT);
}

Pump::~Pump()
{
    digitalWrite(PUMP_PIN, 0);
}

void Pump::activate()
{
    digitalWrite(PUMP_PIN, 1);
    std::cout << "Activated pump\n";
}

void Pump::deactivate()
{
    digitalWrite(PUMP_PIN, 0);
    std::cout << "Deactivated pump\n";
}
