#include "SonarProcess.hpp"

SonarProcess::SonarProcess() {}

SonarProcess::~SonarProcess() {}

SonarData SonarProcess::process(const std::vector<uint8_t>& data) {
    if (data.size() < 6){
        return {0, 0, 0};
    }
    uint32_t frontSensorLeft = static_cast<unsigned int> ((data[0] << 8) | data[1]);
    uint32_t frontSensorMiddle = static_cast<unsigned int> ((data[2] << 8) | data[3]);
    uint32_t frontSensorRight = static_cast<unsigned int> ((data[4] << 8) | data[5]);

    return {frontSensorLeft, frontSensorMiddle, frontSensorRight};
}