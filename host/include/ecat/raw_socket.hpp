#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ecat {

struct ReceivedFrame {
    std::vector<uint8_t> bytes;
    bool outgoing{};
};

class RawSocket {
  public:
    explicit RawSocket(std::string interface_name, uint16_t ethertype);
    ~RawSocket();

    RawSocket(const RawSocket&) = delete;
    RawSocket& operator=(const RawSocket&) = delete;

    const std::string& interface_name() const {
        return interface_name_;
    }
    const std::array<uint8_t, 6>& mac() const {
        return mac_;
    }

    void send_broadcast(const std::vector<uint8_t>& frame) const;
    std::optional<ReceivedFrame> receive(std::chrono::milliseconds timeout) const;

  private:
    int fd_{-1};
    int ifindex_{0};
    uint16_t ethertype_{};
    std::string interface_name_;
    std::array<uint8_t, 6> mac_{};
};

std::string mac_to_string(const std::array<uint8_t, 6>& mac);

} // namespace ecat
