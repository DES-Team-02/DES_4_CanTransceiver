#pragma once

#include <vector>
#include <linux/can.h>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>

typedef v0_1::commonapi::CanTransceiver::SonarArrayStruct SonarData;

class SonarProcess {
public:
    SonarProcess();
    ~SonarProcess();
    static SonarData process(const struct can_frame& frame);
};

