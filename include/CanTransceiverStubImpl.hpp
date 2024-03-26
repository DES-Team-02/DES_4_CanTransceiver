#ifndef CANTRANSCEIVERSTUBIMPL_HPP
#define CANTRANSCEIVERSTUBIMPL_HPP

#include <iostream>
#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>
#include <chrono>
#include <thread>
#include <memory>

class CanTransceiverStubImpl : public v0::commonapi::CanTransceiverStubDefault,
                               public std::enable_shared_from_this<CanTransceiverStubImpl> {
public:
    CanTransceiverStubImpl();
    virtual ~CanTransceiverStubImpl();

    void init(); // Renamed for clarity.

private:
    std::shared_ptr<CommonAPI::Runtime> runtime;
};

#endif // CANTRANSCEIVERSTUBIMPL_HPP
