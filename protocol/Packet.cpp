#include "Packet.hpp"

Packet::Packet(Poco::Int32 id, const Payload &payload) : m_packet_id(id), m_packet_length(payload.size()), m_payload(payload)
{
}

Packet::Packet(Poco::Int32 id, Payload &&payload) : m_packet_id(id), m_packet_length(payload.size()), m_payload(std::move(payload))
{
}

Packet::~Packet()
{
}

const Payload &Packet::getPayload() const
{
    return (m_payload);
}

Poco::Int32 Packet::getLength() const
{
    return (m_payload.size());
}

Poco::Int32 Packet::getId() const
{
    return (m_packet_id);
}

const std::optional<Packet> Packet::generateAnswer(void) const
{
    std::optional<Packet> ret;

    switch (m_packet_id)
    {   
        default:
        {
            ret.reset();
            break;
        }
    }
    return (ret);
}