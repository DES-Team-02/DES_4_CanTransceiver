#pragma once

#include <vector>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>

typedef v0_1::commonapi::CanTransceiver::SonarArrayStruct SonarData;

class SonarProcess {
public:
    SonarProcess();
    ~SonarProcess();
    static SonarData process(const std::vector<uint8_t>& data);

};

