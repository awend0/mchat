#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <protocol/Packet.hpp>
#include <iostream>
#include <iomanip>

class Packet;

namespace generator
{
    Packet generateHandshake(Poco::Int32 version, const std::string &host, Poco::UInt16 port);
    Packet generateLogin(const std::string &username);
}

#endif