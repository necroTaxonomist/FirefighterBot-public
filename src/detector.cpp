
#include "camera.h"

Detector::Detector()
{

}

Detector::~Detector()
{

}

bool Detector::checkForFire(DistanceFt* dist, AngleRad* angle)
{
}

bool Detector::waitForFire(DistanceFt* dist, AngleRad* angle, bool look)
{
    // block here
    return checkForFire(dist, angle);
}
