#pragma once

#include <functional>
#include <string>
#include <vector>

namespace kxt
{
/// A code region for which time has been measured.
struct TimedRegion
{
    std::string name; ///< Region name
    double duration;  ///< Region duration in seconds
};

/// Initialise the timing library.
void init ();

/// Deinitialise the timing library.
void quit ();

/// Set the sync function.
/// The sync function is called by the timing library on every call to region_start()
/// and region_end(). This can be used to perform synchronisation steps prior to
/// obtaining time measurements.
void set_sync_function (const std::function<void()>&);

/// Set the sample frequency.
/// For frequency values >= 1, the timing library will gather, on average,
/// frame statistics every 'freq' number of frames.
/// A frequency value of 0 disables this functionality, forcing the timing library
/// to gather samples every frame.
void set_sample_frequency (int freq);

/// Start a new frame.
/// This function must be called prior to marking code regions.
/// If your application runs in a loop, call this at the start of every frame.
void frame_start ();

/// End a frame.
/// This function must be called prior to calling 'timed_regions' to retrieve the
/// gathered time measurements.
/// If your application runs in a loop, call this at the end of every frame.
void frame_end ();

/// Mark the start of a code region.
void region_start (const char* name = nullptr);
void region_start (const std::string &name);

/// Mark the end of a code region.
/// If a region name is provided, the timing library will double-check that the region
/// being ended matches the region that was last started.
/// Throw std::logic_error on region match failure.
void region_end (const char* name = nullptr);
void region_end (const std::string &name);

/// Get the duration of a code region.
double region_duration (const char* name);
double region_duration(const std::string &name);

/// Retrieve time measurements for the last frame.
/// Make sure to call 'frame_end' before calling this function.
const std::vector<TimedRegion>& timed_regions ();

} // namespace kxt
