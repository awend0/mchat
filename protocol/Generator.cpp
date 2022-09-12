#include "Generator.hpp"

namespace generator
{
    Packet generateHandshake(Poco::Int32 version, const std::string &host, Poco::UInt16 port)
    {
        Payload payload;

        payload.push(Poco::Int32(0x00));

        payload.push(version);
        payload.push(host);
        payload.pushIntegral(port);
        payload.push(Poco::Int32(2));

        return (Packet(0x00, std::move(payload)));
    }

    Packet generateLogin(const std::string &username)
    {
        Payload payload;

        payload.push(Poco::Int32(0x00));

        payload.push(username);
        payload.pushIntegral(false);
        payload.pushIntegral(false);

        return (Packet(0x00, std::move(payload)));
    }
}