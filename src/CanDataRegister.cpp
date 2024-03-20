#include "CanDataRegister.hpp"
#include <iostream>
#include <thread>
using namespace v0_1::commonapi;

/*Constructor.*/
CanTransceiverStubImpl::CanTransceiverStubImpl() {
    // Get the commonAPI runtime instance & create the vSOME/IP service
    runtime = CommonAPI::Runtime::get();
    CanTransceiverService = std::make_shared<CanTransceiverStubImpl>();
    // Register the SOME/IP service
    CanTransceiverInit();
}

/*Destructor*/
CanTransceiverStubImpl::~CanTransceiverStubImpl() {}

/* Registers the vSOME/IP service*/
void CanTransceiverStubImpl::CanTransceiverInit(){
    // define service's domain, instance, connection
    std::string domain      = "local";
    std::string instance    = "commonapi.CanTransceiver";
    std::string connection  = "service-CanTransceiver";
    bool successfullyRegistered = runtime->registerService(domain, instance, CanTransceiverService, connection);
    while(!successfullyRegistered){
        std::cout << "Register CanTransceiver Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, CanTransceiverService, connection);
    }
    std::cout << "Successfully Registered CanTransceiver Service!" << std::endl;
}


/* sets the attributes of vSOME/IP service*/
void CanTransceiverStubImpl::setServiceRpmAttributes(uint32_t rpm, uint32_t speed) {
    // set via StubImpl
    CanTransceiverService->setRpmAttribute(rpm);
    CanTransceiverService->setSpeedAttribute(speed);
}

void CanTransceiverStubImpl::setServiceSonarAttributes(uint32_t sensorFL, uint32_t sensorFM, uint32_t sensorFR) {
    // set via StubImpl
    CanTransceiverService->setSonarArrayStruct(sensorFL, sensorFM, sensorFR);
}