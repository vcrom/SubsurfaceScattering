#include <Timer.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include <mach/mach_time.h>
#elif _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else // Linux
    #include <time.h>
    const double NSEC_TO_SEC = 1.0 / 1000000000.0;
    const double SEC_TO_NSECd = 1000000000.0;
    const TimeReading SEC_TO_NSEC = 1000000000;
#endif

static double secondsPerCount;

static void timer_initialise_subsystem () {
#ifdef _WIN32
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    secondsPerCount = 1.0 / (double)countsPerSec;
#else
    struct timespec ts;
    clock_getres(CLOCK_REALTIME, &ts);
    secondsPerCount = (double)ts.tv_sec + ((double)ts.tv_nsec * NSEC_TO_SEC);
#endif
}

void timer_sleep (float seconds) {
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000.0f));
#else
    struct timespec ts;
    ts.tv_sec  = (int) seconds;
    ts.tv_nsec = (long) ((double)(seconds - (int)seconds) * SEC_TO_NSECd);
    nanosleep(&ts, NULL);
#endif
}

TimeReading now () {
    TimeReading t;
#ifdef __APPLE__
    t = mach_absolute_time();
#elif _WIN32
    QueryPerformanceCounter((LARGE_INTEGER*)&t);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    t = ts.tv_sec*SEC_TO_NSEC + ts.tv_nsec;
#endif
    return t;
}

double time_diff (TimeReading start, TimeReading end)
{
    return (double)((end-start)*secondsPerCount);
}

Timer::Timer () {
    timer_initialise_subsystem();
    reset();
}

void Timer::tick () {
    if (stopped) {
        deltaTime = 0.0;
        return;
    }

    //Get the time on this frame.
    curTime = now();

    //Time delta between the current frame and the previous.
    deltaTime = (float) ((curTime - prevTime) * secondsPerCount);

    //Update for next frame.
    prevTime = curTime;

    // Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
    // processor goes into a power save mode or we get shuffled to
    // another processor, then the delta time can be negative.
    if(deltaTime < 0.0f) deltaTime = 0.0f;
}

void Timer::reset () {
    TimeReading n = now();
    baseTime = n;
    stopTime = n;
    prevTime = n;
    curTime = n;
    pausedTime = 0;
    deltaTime = 0.0f;
    stopped = true;
}

void Timer::stop () {
    // Don't do anything if we are already stopped.
    if (!stopped) {
        // Grab the stop time.
        stopTime = now();

        // Now we are stopped.
        stopped = true;
    }
}

void Timer::start () {
    // Only start if we are stopped.
    if (stopped) {
        TimeReading startTime = now();

        // Accumulate the paused time.
        pausedTime = pausedTime + startTime - stopTime;

        // Make the previous time valid.
        prevTime = startTime;

        //Now we are running.
        stopTime = 0;
        stopped = false;
    }
}

double Timer::getTime () const
{
    // If we are stopped, we do not count the time we have been stopped for.
    if (stopped) {
        return (double)((stopTime - baseTime) * secondsPerCount);
    }
    // Otherwise return the time elapsed since the start but without
    // taking into account the time we have been stopped for.
    else {
        return (double)((curTime - baseTime - pausedTime) * secondsPerCount);
    }
}

float Timer::getDelta () const {
    return deltaTime;
}

bool Timer::isRunning () const {
    return !stopped;
}
