/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.2.14.v202310241605.
* Used org.franca.core 0.13.1.201807231814.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_COMMONAPI_Can_Transceiver_STUB_DEFAULT_HPP_
#define V0_COMMONAPI_Can_Transceiver_STUB_DEFAULT_HPP_


#include <CommonAPI/Export.hpp>

#include <v0/commonapi/CanTransceiverStub.hpp>
#include <cassert>
#include <sstream>

# if defined(_MSC_VER)
#  if _MSC_VER >= 1300
/*
 * Diamond inheritance is used for the CommonAPI::Proxy base class.
 * The Microsoft compiler put warning (C4250) using a desired c++ feature: "Delegating to a sister class"
 * A powerful technique that arises from using virtual inheritance is to delegate a method from a class in another class
 * by using a common abstract base class. This is also called cross delegation.
 */
#    pragma warning( disable : 4250 )
#  endif
# endif

namespace v0 {
namespace commonapi {

/**
 * Provides a default implementation for CanTransceiverStubRemoteEvent and
 * CanTransceiverStub. Method callbacks have an empty implementation,
 * remote set calls on attributes will always change the value of the attribute
 * to the one received.
 *
 * Override this stub if you only want to provide a subset of the functionality
 * that would be defined for this service, and/or if you do not need any non-default
 * behaviour.
 */
class COMMONAPI_EXPORT_CLASS_EXPLICIT CanTransceiverStubDefault
    : public virtual CanTransceiverStub {
public:
    COMMONAPI_EXPORT CanTransceiverStubDefault()
        : remoteEventHandler_(this),
          interfaceVersion_(CanTransceiver::getInterfaceVersion()) {
    }

    COMMONAPI_EXPORT const CommonAPI::Version& getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client) {
        (void)_client;
        return interfaceVersion_;
    }

    COMMONAPI_EXPORT CanTransceiverStubRemoteEvent* initStubAdapter(const std::shared_ptr< CanTransceiverStubAdapter> &_adapter) {
        CommonAPI::Stub<CanTransceiverStubAdapter, CanTransceiverStubRemoteEvent>::stubAdapter_ = _adapter;
        return &remoteEventHandler_;
    }

    COMMONAPI_EXPORT virtual const ::v0::commonapi::CanTransceiver::SonarArrayStruct &getDistancesAttribute() {
        return distancesAttributeValue_;
    }
    COMMONAPI_EXPORT virtual const ::v0::commonapi::CanTransceiver::SonarArrayStruct &getDistancesAttribute(const std::shared_ptr<CommonAPI::ClientId> _client) {
        (void)_client;
        return getDistancesAttribute();
    }
    COMMONAPI_EXPORT virtual void setDistancesAttribute(::v0::commonapi::CanTransceiver::SonarArrayStruct _value) {
        const bool valueChanged = trySetDistancesAttribute(std::move(_value));
        if (valueChanged) {
            fireDistancesAttributeChanged(distancesAttributeValue_);
        }
    }
    COMMONAPI_EXPORT virtual const uint32_t &getSpeedAttribute() {
        return speedAttributeValue_;
    }
    COMMONAPI_EXPORT virtual const uint32_t &getSpeedAttribute(const std::shared_ptr<CommonAPI::ClientId> _client) {
        (void)_client;
        return getSpeedAttribute();
    }
    COMMONAPI_EXPORT virtual void setSpeedAttribute(uint32_t _value) {
        const bool valueChanged = trySetSpeedAttribute(std::move(_value));
        if (valueChanged) {
            fireSpeedAttributeChanged(speedAttributeValue_);
        }
    }
    COMMONAPI_EXPORT virtual const uint32_t &getRpmAttribute() {
        return rpmAttributeValue_;
    }
    COMMONAPI_EXPORT virtual const uint32_t &getRpmAttribute(const std::shared_ptr<CommonAPI::ClientId> _client) {
        (void)_client;
        return getRpmAttribute();
    }
    COMMONAPI_EXPORT virtual void setRpmAttribute(uint32_t _value) {
        const bool valueChanged = trySetRpmAttribute(std::move(_value));
        if (valueChanged) {
            fireRpmAttributeChanged(rpmAttributeValue_);
        }
    }


protected:
    COMMONAPI_EXPORT virtual bool trySetDistancesAttribute(::v0::commonapi::CanTransceiver::SonarArrayStruct _value) {
        if (!validateDistancesAttributeRequestedValue(_value))
            return false;

        bool valueChanged;
        std::shared_ptr<CanTransceiverStubAdapter> stubAdapter = CommonAPI::Stub<CanTransceiverStubAdapter, CanTransceiverStubRemoteEvent>::stubAdapter_.lock();
        if(stubAdapter) {
            stubAdapter->lockDistancesAttribute(true);
            valueChanged = (distancesAttributeValue_ != _value);
            distancesAttributeValue_ = std::move(_value);
            stubAdapter->lockDistancesAttribute(false);
        } else {
            valueChanged = (distancesAttributeValue_ != _value);
            distancesAttributeValue_ = std::move(_value);
        }

       return valueChanged;
    }
    COMMONAPI_EXPORT virtual bool validateDistancesAttributeRequestedValue(const ::v0::commonapi::CanTransceiver::SonarArrayStruct &_value) {
        (void)_value;
        return true;
    }
    COMMONAPI_EXPORT virtual bool trySetSpeedAttribute(uint32_t _value) {
        if (!validateSpeedAttributeRequestedValue(_value))
            return false;

        bool valueChanged;
        std::shared_ptr<CanTransceiverStubAdapter> stubAdapter = CommonAPI::Stub<CanTransceiverStubAdapter, CanTransceiverStubRemoteEvent>::stubAdapter_.lock();
        if(stubAdapter) {
            stubAdapter->lockSpeedAttribute(true);
            valueChanged = (speedAttributeValue_ != _value);
            speedAttributeValue_ = std::move(_value);
            stubAdapter->lockSpeedAttribute(false);
        } else {
            valueChanged = (speedAttributeValue_ != _value);
            speedAttributeValue_ = std::move(_value);
        }

       return valueChanged;
    }
    COMMONAPI_EXPORT virtual bool validateSpeedAttributeRequestedValue(const uint32_t &_value) {
        (void)_value;
        return true;
    }
    COMMONAPI_EXPORT virtual bool trySetRpmAttribute(uint32_t _value) {
        if (!validateRpmAttributeRequestedValue(_value))
            return false;

        bool valueChanged;
        std::shared_ptr<CanTransceiverStubAdapter> stubAdapter = CommonAPI::Stub<CanTransceiverStubAdapter, CanTransceiverStubRemoteEvent>::stubAdapter_.lock();
        if(stubAdapter) {
            stubAdapter->lockRpmAttribute(true);
            valueChanged = (rpmAttributeValue_ != _value);
            rpmAttributeValue_ = std::move(_value);
            stubAdapter->lockRpmAttribute(false);
        } else {
            valueChanged = (rpmAttributeValue_ != _value);
            rpmAttributeValue_ = std::move(_value);
        }

       return valueChanged;
    }
    COMMONAPI_EXPORT virtual bool validateRpmAttributeRequestedValue(const uint32_t &_value) {
        (void)_value;
        return true;
    }
    class COMMONAPI_EXPORT_CLASS_EXPLICIT RemoteEventHandler: public virtual CanTransceiverStubRemoteEvent {
    public:
        COMMONAPI_EXPORT RemoteEventHandler(CanTransceiverStubDefault *_defaultStub)
            : 
              defaultStub_(_defaultStub) {
        }


    private:
        CanTransceiverStubDefault *defaultStub_;
    };
protected:
    CanTransceiverStubDefault::RemoteEventHandler remoteEventHandler_;

private:

    ::v0::commonapi::CanTransceiver::SonarArrayStruct distancesAttributeValue_ {};
    uint32_t speedAttributeValue_ {};
    uint32_t rpmAttributeValue_ {};

    CommonAPI::Version interfaceVersion_;
};

} // namespace commonapi
} // namespace v0


// Compatibility
namespace v0_1 = v0;

#endif // V0_COMMONAPI_Can_Transceiver_STUB_DEFAULT