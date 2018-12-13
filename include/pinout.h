
#pragma once

#ifndef NO_PI
#include <wiringPi.h>
#endif

void gpioInit();
void gpioPinMode(int pin, int mode);
void gpioDigitalWrite(int pin, int value);
void gpioPwmWrite(int pin, float value);
