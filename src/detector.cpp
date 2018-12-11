
#include "camera.h"

Detector::Detector()
{

}

Detector::~Detector()
{

}

bool Detector::checkForFire(AngleRad* angle)
{
    return false;
}

bool Detector::waitForFire(AngleRad* angle, bool look)
{
    // block here
    return checkForFire(angle);
}
