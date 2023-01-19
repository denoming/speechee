#include "client/SpeakerClientImpl.hpp"

namespace jar {

SpeakerClientImpl::SpeakerClientImpl(std::string destination, std::string objectPath)
    : ProxyInterfaces{std::move(destination), std::move(objectPath)}
{
    registerProxy();
}

SpeakerClientImpl::~SpeakerClientImpl()
{
    unregisterProxy();
}

} // namespace jar
