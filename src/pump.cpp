
#include "pump.h"

#include <iostream>

Pump::Pump()
{

}

Pump::~Pump()
{

}

void Pump::activate()
{
    std::cout << "Activated pump\n";
}

void Pump::deactivate()
{
    std::cout << "Deactivated pump\n";
}
