#include "DhcpAddressesCache.hpp"

namespace Dhcp {

void AddressesCache::push(AssignedAddress&& assignedAddress) {
    std::lock_guard lock{this->mutex};
    this->assignedAddresses.push_back(std::move(assignedAddress));
}

} // namespace Dhcp