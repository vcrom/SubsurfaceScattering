#pragma once

#ifdef _WIN32
    #ifdef _MSC_VER
        typedef __int64 TimeReading;
    #else
        typedef __UINT64_TYPE__ TimeReading;
    #endif
#else
    typedef __UINT64_TYPE__  TimeReading;
#endif

/*
Class: Timer
A high resolution timer.
*/
class Timer
{
    TimeReading baseTime;   // The instant since we start timing.
    TimeReading stopTime;   // The instant the timer is stopped.
    TimeReading prevTime;   // The instant the timer was ticked prior to the last tick.
    TimeReading curTime;    // The instant the timer was last ticked.
    TimeReading pausedTime; // Amount of time the timer has been stopped for.
    float deltaTime; // Amount of time elapsed since the last call to tick.
    bool stopped;

public:

    /*
    Constructor: Timer
    Construct a new timer.

    The timer is initialised by making a call to reset(). Since time
    calculations are measured from the instant the timer is reset (base time),
    you probably want to make a manual call to reset() at the start of
    your application, otherwise the application will be measuring times
    from the instant the timer's constructor is called, which can be error prone.

    A call to start() must be made prior to any time calculations, as the
    timer is initialised as stopped.
    */
    Timer ();

    /*
    Function: tick
    Update the timer's values.

    This function updates the timer's running time and caches the time
    elapsed since the last tick or since the start if this is the first
    tick after the last call to start().

    This function has no effect on a stopped ticker.
    */
    void tick ();

    /*
    Function: start
    Start the timer.

    This function starts the timer for the first time or resumes it
    after a call to stop().

    Note that this function does not reset the timer's base time;
    it's only a mechanism to resume a stopped timer.
    */
    void start ();

    /*
    Function: stop
    Stop the timer.

    This function essentially freezes time; any values dependent on
    the timer will behave as if time had not passed since the moment
    the timer was stopped.

    To resume the timer call start().
    */
    void stop ();

    /*
    Function: reset
    Reset the timer.

    This function resets all of the timer's values such as running and
    stop times and sets the timer to stopped. The total running time is
    then measured from the instant the timer is reset, making the timer
    behave as a newly constructed one.

    A call to start() must be made prior to any further time calculations.
    */
    void reset ();

    /*
    Function: getTime
    Get the total running time.

    The amount of time the timer has been stopped for is not taken
    into account.
    */
    double getTime () const;

    /*
    Function: getDelta
    Get the time elapsed since the last tick, or since the start if
    this is the first tick.
    */
    float getDelta () const;

    /*
    Function: isRunning
    Return true if the timer is running (not stopped), false otherwise.
    */
    bool isRunning() const;
};

/*
Function: now
Get a timestamp of the current time.
*/
TimeReading now ();

/*
Function: time_diff
Compute the difference end-start, in seconds.
*/
double time_diff (TimeReading start, TimeReading end);

/*
Function: timer_sleep
Put the caller thread to sleep for the given number of seconds.
*/
void timer_sleep (float seconds);
