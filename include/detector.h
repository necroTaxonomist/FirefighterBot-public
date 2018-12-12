
#pragma once

#include "units.h"

#include <memory>

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

class Detector
{
public:
    Detector();
    ~Detector();

    // Returns true if there is fire
    // Writes the distance and angle into the provided vars
    bool checkForFire(AngleDeg& angle);

    // Same as check for fire, but blocks until there is/isn't fire
    bool waitForFire(bool look, AngleDeg& angle);

private:
    std::atomic<bool> done;

    std::unique_ptr<std::thread> detectThread;

    bool found;
    AngleDeg foundAngle;

    std::mutex foundMutex;
    std::condition_variable foundCond;

private:
    void update(bool _found, AngleDeg _foundAngle = 0);

    friend void detectorThread(Detector* det);
};
