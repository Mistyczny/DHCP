#pragma once
#include "DhcpPacket.hpp"

namespace Dhcp {

enum AssignationType : uint8_t {
    Static=0,
    ShortLeasing,
    LongLeasing
};

enum AssignationDurationInSeconds{
    SHORT_LEASING_TIME = 43200,
    LONG_LEASING_TIME = 86400
};

struct AssignedAddress {
    AssignationType leasingType;
    boost::asio::ip::address_v4 ip;
    std::chrono::system_clock::time_point assignationTime;
    HardwareAddress clientHardwareAddress;

    bool operator==(AssignedAddress& other) {
        if(this->ip != other.ip) return false;
        else if(this->clientHardwareAddress != other.clientHardwareAddress) return false;
        else return true;
    }
};

}