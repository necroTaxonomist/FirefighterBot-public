
#include "detector.h"

Detector::Detector()
{

}

Detector::~Detector()
{

}

bool Detector::checkForFire(AngleDeg* angle, TempF* temp)
{
    return false;
}

bool Detector::waitForFire(AngleDeg* angle, TempF* temp, bool look)
{
    // block here
    return checkForFire(angle);
}
