#ifndef CANTRANSCEIVERSTUBIMPL_HPP
#define CANTRANSCEIVERSTUBIMPL_HPP

#include <iostream>
#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>

class CanTransceiverStubImpl: public v0_1::commonapi::CanTransceiverStubDefault
{
public:
    CanTransceiverStubImpl();
    virtual ~CanTransceiverStubImpl();
    void setServiceRpmAttributes(uint32_t rpm, uint32_t speed);
    void setServiceSonarAttributes(uint32_t sensorFL, uint32_t sensorFM, uint32_t sensorFR); 
private:
    std::shared_ptr<CommonAPI::Runtime> runtime;
    void CanTransceiverInit();
};

#endif // CANTRANSCEIVERSTUBIMPL_HPP