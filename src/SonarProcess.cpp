#include "SonarProcess.hpp"

SonarProcess::SonarProcess() {}

SonarProcess::~SonarProcess() {}

SonarData SonarProcess::process(const struct can_frame& frame)
{
    std::vector<uint8_t> data = std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc);
    if (data.size() < 4)
        return {100, 100};

    uint32_t frontSensor = static_cast<unsigned int> ((data[0] << 8) | data[1]);
    uint32_t rearSensor = static_cast<unsigned int> ((data[2] << 8) | data[3]);

    if (frontSensor == 0)
        frontSensor = 100;
    if (rearSensor == 0)
        rearSensor = 100;

    return {frontSensor, rearSensor};
}