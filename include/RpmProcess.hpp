#pragma once

#include <vector>
#include "MovingAverageFilter.hpp"

struct RpmData {
    double rpm;
    double speed;
};

class RpmProcess {
public:
    static RpmData process(const std::vector<uint8_t>& data, double wheel_radius, double disk_radius);
};
