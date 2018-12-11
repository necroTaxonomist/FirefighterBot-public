
#include "detector.h"

void detectorThread(Detector* det);

Detector::Detector():
    done(false),
    detectThread(nullptr)
{
    detectThread = std::unique_ptr<std::thread>(new std::thread(detectorThread, this));
}

Detector::~Detector()
{
    done.store(true);
    detectThread->join();
}

bool Detector::checkForFire(AngleDeg* angle, float* dist)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    if (angle)
        *angle = foundAngle;

    if (dist)
        *dist = foundDist;

    bool check = found;

    lk.unlock();

    return check;
}

bool Detector::waitForFire(AngleDeg* angle, float* dist, bool look)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    if (look)
    {
        condFound.wait(lk, []{return found;});
    }
    else
    {
        condLost.wait(lk, []{return !found;});
    }

    std::unique_lock<std::mutex> lk(foundMutex);

    if (angle)
        *angle = foundAngle;

    if (dist)
        *dist = foundDist;

    bool check = found;

    lk.unlock();

    return check;
}

void Detector::detect()
{
    // check the camera for fire
    // call find() if it is found
    // call lose() if it not found
}

void Detector::find(AngleDeg _foundAngle, float _foundDist)
{
    std::unique_lock<std::mutex> lk(foundMutex);

    found = true;
    foundAngle = _foundAngle;
    foundDist = _foundDist;

    condFound.notify_all();

    lk.unlock();
}

void Detector::lose()
{
    std::unique_lock<std::mutex> lk(foundMutex);

    found = false;

    condLost.notify_all();

    lk.unlock();
}

void detectorThread(Detector* det)
{
    while (!det.done.load())
    {
        det.detect();
    }
}
