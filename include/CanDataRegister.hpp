#ifndef CAN_DATA_REGISTER_HPP
#define CAN_DATA_REGISTER_HPP

#include <iostream>
#include <CommonAPI/CommonAPI.hpp>
#include "CanTransceiverStubImpl.hpp" 

using namespace v0_1::commonapi;

class CanDataRegister {
public:
    CanDataRegister();
    virtual ~CanDataRegister();
    void setServiceRpmAttributes(uint32_t rpm, uint32_t speed);
    void setServiceSonarAttributes(uint32_t sensorFL, uint32_t sensorFM, uint32_t sensorFR); 
private:
    std::shared_ptr<CommonAPI::Runtime> runtime;
    std::shared_ptr<CanTransceiverImpl> CanTransceiverService;
    void CanTransceiverInit();
};

#endif //CAN_DATA_REGISTER_HPP