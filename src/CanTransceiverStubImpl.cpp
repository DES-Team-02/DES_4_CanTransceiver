#include "CanTransceiverStubImpl.hpp"

CanTransceiverStubImpl::CanTransceiverStubImpl() : runtime(CommonAPI::Runtime::get()) {}

CanTransceiverStubImpl::~CanTransceiverStubImpl() {}

void CanTransceiverStubImpl::init() {
    const std::string domain = "local";
    const std::string instance = "commonapi.CanTransceiver";
    const std::string connection = "service-CanTransceiver";

    if (runtime->registerService(domain, instance, shared_from_this(), connection)) {
        std::cout << "Successfully Registered CanTransceiver Service!" << std::endl;
    } else {
        std::cerr << "Failed to Register CanTransceiver Service!" << std::endl;
    }
}
