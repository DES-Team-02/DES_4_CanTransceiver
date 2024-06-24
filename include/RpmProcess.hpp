#pragma once

#include <vector>
#include <cmath>
#include <cstdint>
#include <linux/can.h>
#include "MovingAverageFilter.hpp"

struct RpmData {
    double rpm;
    double speed;
};

class RpmProcess {
public:
    RpmProcess();
    ~RpmProcess();

    static RpmData process(const struct can_frame& frame);

private:
    static const double        _wheel_radius;
    static const double        _disk_radius;
    static MovingAverageFilter _movingAverageFilter;
};
