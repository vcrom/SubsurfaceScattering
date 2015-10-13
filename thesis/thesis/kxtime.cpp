#include "kxtime.h"
#include <Timer.h>

#include <unordered_map>
#include <vector>
#include <random>

#include <stdexcept>
#include <sstream>

using namespace kxt;

namespace
{
using Gen = std::mt19937;

// The data structure we use to keep track of region times
// This is different from the public TimedRegion data structure, which also includes
// the region name. In the implementation, the region name is the key to the hashmap
// below.
struct StartEnd
{
    TimeReading start;
    TimeReading end;

    StartEnd (TimeReading start = 0, TimeReading end = 0)
        : start(start), end(end) {}
};

// Map region names to region start/end time pairs
using Map = std::unordered_map<std::string, StartEnd>;

// Holds region start and end times
Map map;

// Keeps track of the started and ended regions
// This is used to detect logic errors by matching start/end pairs
std::vector<std::string> region_stack;

// Holds time region data
// We keep this here to avoid allocating memory for the region data every frame
std::vector<TimedRegion> regions;

// The synchronisation function called on every region start/end prior to taking
// time measurements
 std::function<void()> sync = [] {};

// The frequency at which time measurements should be taken
// A frequency <= 1 causes the timing library to gather measurements every frame
// Frequency values > 1 cause the timing library to randomly gather measurements,
// doing so at an average of once every 'gather_frequency' frames
int gather_frequency = -1;

// The prng used to determine whether time measurements should be gather in this frame
Gen gl_timings_gen;

// Whether time measurements should be gathered in this frame
bool gather_timings = false;
} // namespace

void kxt::init ()
{

}

void kxt::quit ()
{

}

void kxt::set_sync_function (const std::function<void ()>& sync_f)
{
    sync = sync_f;
}

void kxt::set_sample_frequency (int freq)
{
    gather_frequency = freq;
}

void kxt::frame_start ()
{
    // Clear state regardless of whether we should gather timings this frame so that
    // the client does not obtain duplicate results when calling timed_regions() to
    // get frame statistics.
    map.clear();
    region_stack.clear();
    regions.clear();

    if (gather_frequency > 1)
    {
        // Decide whether we should gather gl timings in this frame
        std::uniform_int_distribution<int> dis(1, gather_frequency);
        gather_timings = dis(gl_timings_gen) == 1;
    }
    else gather_timings = false;
}

void kxt::frame_end ()
{
    if (!gather_timings) return;

    // Populate the regions vector with this frame's time measurements
    regions.reserve(map.size());
    for (const auto& kv : map)
    {
        TimedRegion region;
        region.name = kv.first;
        region.duration = time_diff(kv.second.start, kv.second.end);
        regions.push_back(region);
    }
}

void kxt::region_start (const char* name)
{
    if (!gather_timings) return;
    sync();
    map[name].start = now();
    region_stack.push_back(name);
}

void kxt::region_end (const char *name)
{
    if (!gather_timings) return;
    // If a region name is provided, double-check that this region
    // matches the one that was last started. Match failure implies
    // logic error in client code.
    if (name != nullptr)
    {
        if (region_stack.back() != std::string(name))
        {
            std::ostringstream os;
            os << "Called kxt::region_end(name) with name = " << name
               << ", but last started region was " << region_stack.back();
            throw std::logic_error(os.str());
        }
        region_stack.pop_back();
    }
    sync();
    map[name].end = now();
}

double kxt::region_duration (const char *name)
{
    return time_diff(map[name].start, map[name].end);
}

const std::vector<TimedRegion>& kxt::timed_regions ()
{
    return regions;
}
