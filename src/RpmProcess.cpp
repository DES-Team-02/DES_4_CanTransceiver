#include "RpmProcess.hpp"

const double        RpmProcess::_wheel_radius = 0.065;
const double        RpmProcess::_disk_radius = 0.025;
MovingAverageFilter RpmProcess::_movingAverageFilter(10, 5);

RpmProcess::RpmProcess() {}

RpmProcess::~RpmProcess() {}

RpmData RpmProcess::process(const std::vector<uint8_t>& data) {
    if (data.size() < 2) {
        return {0.0, 0.0}; // 충분한 데이터가 없는 경우
    }

    unsigned int    rpmValue    = static_cast<unsigned int>((data[0] << 8) | data[1]);
    double          filteredRpm = _movingAverageFilter.filter(static_cast<double>(rpmValue));
    double          speed       = (filteredRpm * _wheel_radius * 2 * M_PI) / 60.0;

    return {filteredRpm, speed};
}