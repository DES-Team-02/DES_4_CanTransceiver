#ifndef CANTRANSCEIVERSTUBIMPL_HPP
#define CANTRANSCEIVERSTUBIMPL_HPP

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/CanTransceiverStubDefault.hpp>

class CanTransceiverStubImpl: public v0_1::commonapi::CanTransceiverStubDefault
{
public:
    CanTransceiverStubImpl();
    virtual ~CanTransceiverStubImpl();
};

#endif // CANTRANSCEIVERSTUBIMPL_HPP