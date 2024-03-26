#ifndef CANTRANSCEIVERSTUBIMPL_HPP
#define CANTRANSCEIVERSTUBIMPL_HPP

#include <iostream>
#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>
#include <chrono>
#include <thread>
#include <memory>
class CanTransceiverStubImpl:public v0_1::commonapi::CanTransceiverStubDefault
{
public:
    CanTransceiverStubImpl();
    virtual ~CanTransceiverStubImpl();
    
    // void setServiceRpmAttributes(struct rpmData);
    // void setServiceSonarAttributes(struct sonarData); 
private:
    std::shared_ptr<CommonAPI::Runtime> _runtime;
    std::shared_ptr<CanTransceiverStubImpl> _CanTransceiverService;
    void CanTransceiverInit();

};

#endif // CANTRANSCEIVERSTUBIMPL_HPP