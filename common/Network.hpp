#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <protocol/Packet.hpp>

namespace network
{
    Payload decompressPayload(const Payload &in);
    Payload compressPayload(const Payload &in);

    Poco::Int32 readVarInt(Poco::Net::StreamSocket &socket);
    void sendVarInt(Poco::Net::StreamSocket &socket, const Poco::Int32 &in_value);

    void sendPacket(Poco::Net::StreamSocket &socket, const Packet &packet, bool compressed);
    Packet readPacket(Poco::Net::StreamSocket &socket, bool compressed);
}

#endif