
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
    bool checkForFire(AngleDeg* angle = nullptr, float* dist = nullptr);

    // Same as check for fire, but blocks until there is/isn't fire
    bool waitForFire(AngleDeg* angle = nullptr, float* dist = nullptr, bool look = true);

private:
    std::atomic<bool> done;

    std::unique_ptr<std::thread> detectThread;

    bool found;
    AngleDeg foundAngle;
    float foundDist;

    std::mutex foundMutex;
    std::condition_variable condFound;
    std::condition_variable condLost;

private:
    void detect();

    void find(AngleDeg _foundAngle, float _foundDist);
    void lose();

    friend void detectorThread(Detector* det);
};
