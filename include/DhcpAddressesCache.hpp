#pragma once
#include "DhcpLeasing.hpp"
#include <list>
#include <mutex>

namespace Dhcp {

class AddressesCache {
private:
    std::mutex mutex;
    std::list<AssignedAddress> assignedAddresses;

public:
    AddressesCache() = default;
    ~AddressesCache() = default;
    AddressesCache(const AddressesCache&) = delete;
    AddressesCache& operator=(const AddressesCache& other) = delete;
    AddressesCache(AddressesCache&&) = delete;
    AddressesCache& operator=(const AddressesCache&& other) = delete;

    void push(AssignedAddress&& assignedAddress);
};

}