#include "Network.hpp"
#include <zlib.h>

namespace network
{
    Payload decompressPayload(const Payload &in)
    {
        unsigned long size = in.size();

        std::vector<unsigned char> ret(size);
        uncompress(ret.data(), &size, in.data(), in.size());
        return Payload(std::move(ret));
    }

    Payload compressPayload(const Payload &in)
    {
        unsigned long size = in.size();

        std::vector<unsigned char> ret(size);
        compress(ret.data(), &size, in.data(), in.size());
        return Payload(std::move(ret));
    }

    Poco::Int32 readVarInt(Poco::Net::StreamSocket &socket)
    {
        static unsigned char buf[1];
        Poco::Int32 position = 0;
        Poco::Int32 ret = 0;

        while (true)
        {
            socket.receiveBytes(buf, 1, MSG_WAITALL);
            unsigned char current_byte = buf[0];

            ret |= (current_byte & SEGMENT_BITS) << position;
            if ((current_byte & CONTINUE_BIT) == 0)
                break;
            position += 7;

            if (position >= 32)
                throw std::runtime_error("Variable is too large");
        }

        return (ret);
    }

    void sendVarInt(Poco::Net::StreamSocket &socket, const Poco::Int32 &in_value)
    {
        Poco::UInt32 uvalue = static_cast<Poco::UInt32>(in_value);

        static unsigned char buf[1];

        while (true)
        {
            if ((uvalue & ~SEGMENT_BITS) == 0)
            {
                buf[0] = uvalue;
                socket.sendBytes(buf, 1);
                break;
            }

            buf[0] = (uvalue & SEGMENT_BITS) | CONTINUE_BIT;
            socket.sendBytes(buf, 1);
            uvalue >>= 7;
        }
    }

    void sendPacket(Poco::Net::StreamSocket &socket, const Packet &packet, bool compressed)
    {
        if (compressed)
        {
            Payload compressed_payload = compressPayload(packet.getPayload());

            network::sendVarInt(socket, compressed_payload.size());
            socket.sendBytes(compressed_payload.data(), compressed_payload.size());
        }
        else
        {
            network::sendVarInt(socket, packet.getLength());
            socket.sendBytes(packet.getPayload().data(), packet.getPayload().size());
        } 
    }

    Packet readPacket(Poco::Net::StreamSocket &socket, bool compressed)
    {
        auto packet_length = network::readVarInt(socket);
        auto packet_id = network::readVarInt(socket);
        Payload payload = Payload(socket, packet_length);
        if (compressed)
            payload = network::decompressPayload(payload);

        return (Packet(packet_id, std::move(payload)));
    }
}