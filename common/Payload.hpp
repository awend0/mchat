#ifndef PAYLOAD_HPP
#define PAYLOAD_HPP

#include <Poco/Poco.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <iostream>

#define DEFAULT_PORT 25565
#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

class Payload
{
    public:
        Payload(void);
        Payload(std::vector<unsigned char> &&vec);
        Payload(Poco::Net::StreamSocket &sock, const Poco::UInt32 &size);
        Payload(const Payload &other);
        Payload(Payload &&other);
        Payload &operator=(Payload &&other);
        Payload &operator=(const Payload &other);

        Poco::Int32 size(void) const;
        const unsigned char *data(void) const;
        std::vector<unsigned char>::const_iterator begin(void) const;
        std::vector<unsigned char>::const_iterator end(void) const;

        template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
        void pushIntegral(Integer value)
        {
            std::cout << "pushing integral: [";
            for (Poco::UInt32 i = 0; i < sizeof(Integer); i++)
            {
                auto byte = static_cast<unsigned char>(value >> (i * 8));
                printf("%02X ", byte);
                m_data.push_back(byte);
            }
            std::cout << "]" << std::endl;
        }
        void push(Poco::Int32 value);
        void push(Poco::Int64 value);
        void push(const std::string &str);

        Poco::Int32 readVarInt(void);
        Poco::Int64 readVarLong(void);
        std::string readUtf8String(void);
        std::wstring readUtf16String(void);

    private:
        std::vector<unsigned char> m_data;
        std::size_t m_current;
};

#endif