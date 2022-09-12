#ifndef PACKET_HPP
#define PACKET_HPP

#include <common/Payload.hpp>
#include <protocol/Generator.hpp>

class Packet
{
    public:
        Packet(Poco::Int32 id, const Payload &payload);
        Packet(Poco::Int32 id, Payload &&payload);
        virtual ~Packet();

        virtual const Payload &getPayload() const;
        virtual Poco::Int32 getLength() const;
        virtual Poco::Int32 getId() const;

        virtual const std::optional<Packet> generateAnswer(void) const;
    private:
        Poco::Int32 m_packet_id;
        Poco::Int32 m_packet_length; 
        Payload m_payload;
};

#endif