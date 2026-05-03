#include "ecat/raw_socket.hpp"

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace ecat {
namespace {

int get_interface_index(int fd, const std::string& name) {
    ifreq ifr{};
    std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
        throw std::runtime_error(std::string("SIOCGIFINDEX failed: ") + std::strerror(errno));
    }
    return ifr.ifr_ifindex;
}

std::array<uint8_t, 6> get_interface_mac(int fd, const std::string& name) {
    ifreq ifr{};
    std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        throw std::runtime_error(std::string("SIOCGIFHWADDR failed: ") + std::strerror(errno));
    }

    std::array<uint8_t, 6> mac{};
    for (size_t i = 0; i < mac.size(); ++i) {
        mac[i] = static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[i]);
    }
    return mac;
}

} // namespace

RawSocket::RawSocket(std::string interface_name, uint16_t ethertype)
    : ethertype_(ethertype), interface_name_(std::move(interface_name)) {
    fd_ = socket(AF_PACKET, SOCK_RAW, htons(ethertype_));
    if (fd_ < 0) {
        throw std::runtime_error(std::string("socket failed: ") + std::strerror(errno));
    }

    ifindex_ = get_interface_index(fd_, interface_name_);
    mac_ = get_interface_mac(fd_, interface_name_);

    sockaddr_ll bind_addr{};
    bind_addr.sll_family = AF_PACKET;
    bind_addr.sll_protocol = htons(ethertype_);
    bind_addr.sll_ifindex = ifindex_;
    if (bind(fd_, reinterpret_cast<sockaddr*>(&bind_addr), sizeof(bind_addr)) < 0) {
        throw std::runtime_error(std::string("bind failed: ") + std::strerror(errno));
    }

    packet_mreq request{};
    request.mr_ifindex = ifindex_;
    request.mr_type = PACKET_MR_PROMISC;
    (void)setsockopt(fd_, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &request, sizeof(request));
}

RawSocket::~RawSocket() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

void RawSocket::send_broadcast(const std::vector<uint8_t>& frame) const {
    sockaddr_ll address{};
    address.sll_family = AF_PACKET;
    address.sll_protocol = htons(ethertype_);
    address.sll_ifindex = ifindex_;
    address.sll_halen = 6;
    for (size_t i = 0; i < 6; ++i) {
        address.sll_addr[i] = 0xff;
    }

    const ssize_t sent =
        sendto(fd_, frame.data(), frame.size(), 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    if (sent < 0) {
        throw std::runtime_error(std::string("sendto failed: ") + std::strerror(errno));
    }
}

std::optional<ReceivedFrame> RawSocket::receive(std::chrono::milliseconds timeout) const {
    timeval tv{};
    tv.tv_sec = static_cast<long>(timeout.count() / 1000);
    tv.tv_usec = static_cast<long>((timeout.count() % 1000) * 1000);
    if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        throw std::runtime_error(std::string("SO_RCVTIMEO failed: ") + std::strerror(errno));
    }

    std::array<uint8_t, 1536> buffer{};
    sockaddr_ll from{};
    socklen_t from_len = sizeof(from);
    const ssize_t n = recvfrom(fd_, buffer.data(), buffer.size(), 0, reinterpret_cast<sockaddr*>(&from), &from_len);
    if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            return std::nullopt;
        }
        throw std::runtime_error(std::string("recvfrom failed: ") + std::strerror(errno));
    }

    ReceivedFrame result;
    result.outgoing = from.sll_pkttype == PACKET_OUTGOING;
    result.bytes.assign(buffer.begin(), buffer.begin() + n);
    return result;
}

std::string mac_to_string(const std::array<uint8_t, 6>& mac) {
    std::ostringstream oss;
    for (size_t i = 0; i < mac.size(); ++i) {
        if (i != 0U) {
            oss << ':';
        }
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(mac[i]);
    }
    return oss.str();
}

} // namespace ecat
