#include <protocol/Packet.hpp>
#include <common/Network.hpp>
#include <memory>
#include <unordered_set>

std::unordered_set<Poco::Int32> packets;

std::shared_ptr<Poco::Net::StreamSocket> login(const std::string &host, const Poco::UInt16 port, const std::string &username)
{
    Poco::Net::SocketAddress socket_address(host, port);
    std::shared_ptr<Poco::Net::StreamSocket> ret(new Poco::Net::StreamSocket(socket_address));

    std::cout << "Login..." << std::endl;
 
    auto handshake_rq = generator::generateHandshake(760, socket_address.host().toString(), port);
    network::sendPacket(*ret, handshake_rq, false);

    auto login_rq = generator::generateLogin(username);
    network::sendPacket(*ret, login_rq, false);

    return (ret);
}

void loop(std::shared_ptr<Poco::Net::StreamSocket> socket)
{
    std::cout << "Entering loop..." << std::endl;
    bool compressed = false;

    while (true)
    {
        try
        {
            auto packet = network::readPacket(*socket, compressed);
            
            if (packet.getId() == 0x03)
                compressed = true;
        
            if (!packets.count(packet.getId()))
            {
                packets.insert(packet.getId());
                std::cout << "Packets met:"
                            << std::endl;
                for (auto &packet : packets)
                    std::cout << " - " << packet << std::endl;

                printf("LAST NEW: [");
                for (const auto &elem : packet.getPayload())
                    std::cout << (char)(isprint(elem) ? elem : '.');
                printf("]\n");
            }

            auto answer = packet.generateAnswer();
            if (answer.has_value())
                network::sendPacket(*socket, answer.value(), compressed);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    if (!(argc == 3 || argc == 4))
    {
        std::cout << "Usage: ./mchat <host> <port> <username> or ./mchat <host> <username>" << std::endl;
        exit(1);
    }
    try
    {
        auto socket = login(argv[1], argc == 4 ? std::stoi(argv[2]) : DEFAULT_PORT, argc == 4 ? argv[3] : argv[2]);
        loop(socket);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}