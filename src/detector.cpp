
#include "camera.h"

Detector::Detector()
{

}

Detector::~Detector()
{

}

bool Detector::checkForFire(AngleDeg* angle)
{
    return false;
}

bool Detector::waitForFire(AngleDeg* angle, bool look)
{
    // block here
    return checkForFire(angle);
}
