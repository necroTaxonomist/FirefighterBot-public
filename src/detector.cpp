
#include "detector.h"

#ifndef NO_PI
#include "fire_detection.h"
#endif

#include <unistd.h>

#define WAIT_TIME 10

Detector::Detector():
    found(false),
    foundAngle(0)
{
}

Detector::~Detector()
{
}

bool Detector::checkForFire(AngleDeg& angle)
{
    detect();

    std::unique_lock<std::mutex> lk(foundMutex);
    angle = foundAngle;
    return found;
}

void Detector::update(bool _found, AngleDeg _foundAngle)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    found = _found;
    foundAngle = _foundAngle;
}

void Detector::detect()
{
#ifdef NO_PI
    sleep(1);
#else
    auto val = processCapture(WAIT_TIME);
    update(val.first, val.second);
#endif
}
