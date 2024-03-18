#include "CanDataRegister.hpp"
#include <iostream>
#include <thread>
using namespace v0_1::commonapi;

/*Constructor.*/
CanDataRegister::CanDataRegister() {
    // Get the commonAPI runtime instance & create the vSOME/IP service
    runtime = CommonAPI::Runtime::get();
    CanTransceiverService = std::make_shared<CanTransceiverStubImpl>();
    // Register the SOME/IP service
    CanTransceiverInit();
}

/*Destructor*/
CanDataRegister::~CanDataRegister() {}

/* Registers the vSOME/IP service*/
void CanDataRegister::CanTransceiverInit(){
    while(!runtime->registerService("local", "commonapi.CanTransceiver", CanTransceiverService, "CanTransceiverService")){
        std::cout << "Register CanTransceiver Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Successfully Registered CanTransceiver Service!" << std::endl;
}

/* sets the attributes of vSOME/IP service*/
void CanDataRegister::setServiceAttributes(uint32_t rpm, uint32_t speed, uint32_t sensor0, uint32_t sensor1, uint32_t sensor2) {
    // set via StubImpl
    CanTransceiverService->setSonarArrayStruct();
    CanTransceiverService->setRpmAttribute(rpm);
    CanTransceiverService->setSpeedAttribute(speed);
}