#include "RpmProcess.hpp"

RpmData RpmProcess::process(const std::vector<uint8_t>& data, double wheel_radius, double disk_radius) {
    MovingAverageFilter filter(10); // Example filter size.
    double rawRpm = ...; // Extract RPM value from data.
    double filteredRpm = filter.filter(rawRpm);

    // Calculate speed using filtered RPM.
    double speed = (filteredRpm * wheel_radius * 2 * M_PI) / 60.0; // Speed in meters per second assuming filteredRpm is in rotations per minute (RPM).

    return {filteredRpm, speed};
}
