#include "CanTransceiverStubImpl.hpp"


CanTransceiverStubImpl::CanTransceiverStubImpl() {
    // Get the commonAPI runtime instance & create the vSOME/IP service
    _runtime = CommonAPI::Runtime::get();
    CanTransceiverInit();
}

CanTransceiverStubImpl::~CanTransceiverStubImpl() {}

/* Registers the vSOME/IP service*/
void CanTransceiverStubImpl::CanTransceiverInit(){
    // define service's domain, instance, connection
    std::string domain      = "local";
    std::string instance    = "commonapi.CanTransceiver";
    std::string connection  = "service-CanTransceiver";

    bool successfullyRegistered = false;
    while(!successfullyRegistered){
        successfullyRegistered = _runtime->registerService(domain, instance, shared_from_this(), connection);
        if(!successfullyRegistered){
            std::cout << "Register CanTransceiver Service failed, trying again in 0.1seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }      
    }
    std::cout << "Successfully Registered CanTransceiver Service!" << std::endl; 
}

// void CanTransceiverStubImpl::setServiceRpmAttributes(uint32_t) {
//     // Directly set the RPM and Speed attributes
//     setRpmAttribute(rpm);
//     setSpeedAttribute(speed);
// }

// void CanTransceiverStubImpl::setServiceSonarAttributes(uint32_t sensorFL, uint32_t sensorFM, uint32_t sensorFR) {
//     // Directly set the Sonar attributes
//     v0::commonapi::CanTransceiver::SonarArrayStruct distances;
//     distances.setSensorfrontleft(sensorFL);
//     distances.setSensorfrontmiddle(sensorFM);
//     distances.setSensorfrontright(sensorFR);
//     setDistancesAttribute(distances);
// } 