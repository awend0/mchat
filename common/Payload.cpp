#include "Payload.hpp"

Payload::Payload(void) : m_data({}), m_current(0)
{
}

Payload::Payload(std::vector<unsigned char> &&vec) : m_data(std::move(vec)), m_current(0)
{
}

Payload::Payload(Poco::Net::StreamSocket &sock, const Poco::UInt32 &size)
{
    m_data.resize(size);
    sock.receiveBytes(m_data.data(), size, MSG_WAITALL);
    m_current= 0;
}

Payload::Payload(const Payload &other) : m_data(other.m_data), m_current(other.m_current)
{
}

Payload::Payload(Payload &&other) : m_data(std::move(other.m_data)), m_current(other.m_current)
{
}

Payload &Payload::operator=(Payload &&other)
{
    m_data = std::move(other.m_data);
    m_current = other.m_current;
    return (*this);
}

Payload &Payload::operator=(const Payload &other)
{
    m_data = other.m_data;
    m_current = other.m_current;
    return (*this);
}

Poco::Int32 Payload::size(void) const
{
    return m_data.size();
}

const unsigned char *Payload::data(void) const
{
    return m_data.data();
}

std::vector<unsigned char>::const_iterator Payload::begin(void) const
{
    return m_data.begin();
}

std::vector<unsigned char>::const_iterator Payload::end(void) const
{
    return m_data.end();
}

void Payload::push(Poco::Int32 value)
{
    Poco::UInt32 uvalue = static_cast<Poco::UInt32>(value);
    std::cout << "pushing UInt32: [";
    while (true)
    {
        if ((uvalue & ~SEGMENT_BITS) == 0)
        {
            m_data.push_back(uvalue);
            printf("%02X", uvalue);
            break;
        }

        printf("%02X ", (uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        m_data.push_back((uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        uvalue >>= 7;
    }
    std::cout << "]" << std::endl;
}

void Payload::push(Poco::Int64 value)
{
    Poco::UInt64 uvalue = static_cast<Poco::UInt64>(value);

    std::cout << "pushing UInt32: [";
    while (true)
    {
        if ((uvalue & ~SEGMENT_BITS) == 0)
        {
            m_data.push_back(uvalue);
            printf("%02lX", uvalue);
            break;
        }

        printf("%02lX ", (uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        m_data.push_back((uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        uvalue >>= 7;
    }
    std::cout << "]" << std::endl;
}

void Payload::push(const std::string &str)
{
    push(static_cast<Poco::Int32>(str.size()));
    std::cout << "pushing string [" << str << "]" << std::endl;
    m_data.insert(m_data.end(), str.begin(), str.end());
}

Poco::Int32 Payload::readVarInt(void)
{
    Poco::Int32 position = 0;
    Poco::Int32 ret = 0;

    while (true)
    {
        unsigned char current_byte = m_data[m_current];
        m_current++;

        ret |= (current_byte & SEGMENT_BITS) << position;
        if ((current_byte & CONTINUE_BIT) == 0)
            break;
        position += 7;

        if (position >= 32)
            throw std::runtime_error("VarInt is too large");
    }

    return (ret);
}

Poco::Int64 Payload::readVarLong(void)
{
    Poco::Int64 position = 0;
    Poco::Int64 ret = 0;

    while (true)
    {
        unsigned char current_byte = m_data[m_current];
        m_current++;

        ret |= (current_byte & SEGMENT_BITS) << position;
        if ((current_byte & CONTINUE_BIT) == 0)
            break;
        position += 7;

        if (position >= 64)
            throw std::runtime_error("VarLong is too large");
    }

    return (ret);
}

std::string Payload::readUtf8String(void)
{
    std::string ret;
    Poco::Int32 size = readVarInt();
    m_current++;

    for (int i = 0; i < size; i++)
        ret += m_data[m_current++];

    return (ret);
}

std::wstring Payload::readUtf16String(void)
{
    std::wstring ret;
    Poco::Int32 size = readVarInt();

    for (int i = 0; i < size; i++)
        ret += m_data[m_current++];

    return (ret);
}

