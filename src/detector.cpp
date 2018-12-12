
#include "detector.h"

void detectorThread(Detector& det);

Detector::Detector():
    done(false),
    detectThread(nullptr),
    found(false),
    foundAngle(0),

{
    detectThread = std::unique_ptr<std::thread>(new std::thread(detectorThread, *this));
}

Detector::~Detector()
{
    done.store(true);
    detectThread->join();
}

bool Detector::checkForFire(AngleDeg& angle)
{
    std::unique_lock<std::mutex> lk(foundMutex);
    angle = foundAngle;
    return found;
}

bool Detector::waitForFire(bool look, AngleDeg& angle)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    while (found != look)
        condFound.wait(lk);

    angle = foundAngle;
    return found;
}

void Detector::update(bool _found, AngleDeg _foundAngle)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    found = _found;
    foundAngle = _foundAngle;
    condFound.notify_all();
}

void detectorThread(Detector& det)
{
    while (!det.done.load())
    {
        // looks for fire and calls update
    }
}
