#pragma once
#include <array>
#include <boost/asio.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace Dhcp {

#define MAX_DHCP_CHADDR_LENGTH 16
#define MAX_DHCP_SERVER_NAME_LENGTH 64
#define MAX_DHCP_FILE_LENGTH 128
#define MAX_DHCP_OPTIONS_LENGTH 78 // std::bytes
#define DHCP_OPTIONS_LENGTH 78

using namespace boost::asio::ip;

namespace DhcpDatagramTypes {
enum Options { SubnetMask = 1, LeasingTime = 51, MessageType = 53, DhcpServerIdentifier = 54 };
}

enum class OperationCode : uint8_t { Request = 1, Response };

enum MessageType { Invalid = -1, Discover = 1, Offer, Request, Decline, Ack, NAK, Release, Inform, Force_Renew };

enum OptionCode {
    Padding = 0,
    SubnetMask,
    TimeOffset,
    RouterAddress,
    ServerTime,
    ServerName,
    DnsServerAddress,
    ServerLog,
    ServerQuotes,
    LprServer,     // Printer Server Address
    ImpressServer, // Impress Server Address
    RlpServer,
    Hostname,
    BootFileSize,
    MeritDumpFile,
    DnsClientName,
    SwapServer,
    RootPath,
    ExtensionFile,
    ForwardOnOff,
    SrcRteOnOff,
    PolicyFilter,
    MaxDgAssembly,
    DefaultIpTLL,
    MtuTimeout,
    MtuPlateu,
    MtuInterface,
    MtuSubnet,
    BroadcastAddress,
    MaskDiscovery,
    MaskSupplier,
    RouterDiscovery,
    RouterRequest,
    StaticRoute,
    Trailers,
    ArpTimeout,
    Ethernet,
    DefaultTcpTLL,
    KeepaliveTime,
    KeepaliveData,
    NisDomain,
    NisServer,
    NtpServer,
    VendorSpecific,
    NetbiosNameServer,
    NetbiosDistributionDatagram,
    NetbiosNodeType,
    NetbiosScope,
    XWindowFont,
    XWindowManager,
    AddressRequest,
    LeasingAddressTime,
    Overload,
    DhcpMessageType,
    DhcpServerIdentifier,
    ParameterList,
    DhcpMessage,
    DhcpMaxMessageSize,
    RenewalTime,
    RebindingTime,
    ClassIdentifier,
    ClienIdentifier,
    NetWareIpDomainName,
    NetWareIpOption,
    NisDomainName,
    NisServerAddress,
    FtpServerName,
    BootfileName,
    HomeAgentsAddress,
    SmtpServerAddress,
    Pop3ServerAddress,
    NntpServerAddress,
    WwwServerAddress,
    FingerServerAddress,
    IrcServerAddress,
    StreetTalkServerAddress,
    StdaServerAddress,
    UserClass,
    DirectoryAgent,
    ServiceScope,
    RapidCommit,
    ClientFqdn,
    RelayAgentInformation,
    InternetStorageNameService,
    Unassgined_0,
    NdsServer
};

typedef std::array<unsigned char, MAX_DHCP_CHADDR_LENGTH> HardwareAddress;

#pragma pack(1)
struct PDU {
    OperationCode operationCode;

    /* Hardware type on which talk is performed */
    std::byte hardwareType;

    /* Length of hardware address */
    std::byte hardwareTypeLength;

    /* Number of made hops */
    std::byte hops;

    /* Random transaction id number */
    uint32_t transactionID;

    /* Seconds elapsed */
    uint16_t secondsElapsed;

    /* flags */
    uint16_t flags;

    /* IP address of this machine, if it has one already */
    address_v4 currentIpAddress;

    /* IP address offered by DHCP server */
    address_v4 offeredIpAddress;

    /* DHCP server ip address */
    address_v4 serverIpAddress;

    /* DHCP server gateway address */
    address_v4 gatewayIpAddress;

    /* Hardware address of client machine */
    HardwareAddress hardwareAddress;

    /* Name of DHCP server */
    std::array<char, MAX_DHCP_SERVER_NAME_LENGTH> serverName;

    /* Boot file name */
    std::array<char, MAX_DHCP_FILE_LENGTH> bootFileName;

    /* DHCP options */
    std::array<uint8_t, DHCP_OPTIONS_LENGTH> options;
};
#pragma pack()

class Packet {
private:
    PDU pdu;

    [[nodiscard]] std::optional<size_t> GetOptionPosition(uint8_t searchingOption) const {
        size_t position = 4; /* Starting from position 4, because 0,1,2,3 is taken by DHCP Magic, which we ignore */

        while (position < DHCP_OPTIONS_LENGTH) {
            if (pdu.options.at(position) == 0) {
                // Found 0, that means there will not be any more data available
                break;
            }

            if (pdu.options.at(position) == searchingOption) {
                return position;
            } else {
                auto step = pdu.options.at(position + 1);
                position += step + 2;
            }
        }

        return {};
    }

public:
    [[nodiscard]] const PDU& GetPDU() const { return pdu; }
    void setPDU(PDU&& dhcpPdu) { this->pdu = std::move(dhcpPdu); }
    [[nodiscard]] MessageType GetMessageType() const {
        MessageType messageType{MessageType::Invalid};
        std::optional<size_t> messageTypePosition = this->GetOptionPosition(OptionCode::DhcpMessageType);
        if (messageTypePosition.has_value()) {
            // We can ignore option length, since it's constant 1
            messageType = static_cast<MessageType>(pdu.options.at(*messageTypePosition + 2));
        }
        return messageType;
    }
    void PrintOptions() {
        std::cout << "OPTIONS: " << std::endl;
        for(int i =0; i < this->pdu.options.size(); i++) {
            std::cout << i << ": " << (int)this->pdu.options.at(i) << std::endl;
        }
    }
};

struct DynamicDhcpDatagram {
    /* Operation to be performed type */
    std::byte operationCode;

    /* Hardware type on which talk is performed */
    std::byte hardwareType;

    /* Length of hardware address */
    std::byte hardwareTypeLength;

    /* Number of made hops */
    std::byte hops;

    /* Random transaction id number */
    uint32_t transactionID;

    /* Seconds elapsed */
    uint16_t secondsElapsed;

    /* flags */
    uint16_t flags;

    /* IP address of this machine, if it has one already */
    address_v4 currentIpAddress;

    /* IP address offered by DHCP server */
    address_v4 offeredIpAddress;

    /* DHCP server ip address */
    address_v4 serverIpAddress;

    /* DHCP server gateway address */
    address_v4 gatewayIpAddress;

    /* Hardware address of client machine */
    std::array<unsigned char, MAX_DHCP_CHADDR_LENGTH> hardwareAddress;

    /* Name of DHCP server */
    std::array<char, MAX_DHCP_SERVER_NAME_LENGTH> serverName;

    /* Boot file name */
    std::array<char, MAX_DHCP_FILE_LENGTH> bootFileName;

    /* DHCP options */
    std::vector<std::byte> options;
};

} // namespace Dhcp