#pragma once

#include "ecat/raw_socket.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ecat {

constexpr uint16_t kEtherType = 0x88A4;

enum class Command : uint8_t {
    nop = 0x00,
    aprd = 0x01,
    apwr = 0x02,
    aprw = 0x03,
    fprd = 0x04,
    fpwr = 0x05,
    fprw = 0x06,
    brd = 0x07,
    bwr = 0x08,
    brw = 0x09,
    lrd = 0x0a,
    lwr = 0x0b,
    lrw = 0x0c,
    armw = 0x0d,
    frmw = 0x0e,
};

struct DatagramRequest {
    Command command{};
    uint16_t adp{};
    uint16_t ado{};
    std::vector<uint8_t> data;
};

struct DatagramResult {
    Command command{};
    uint8_t index{};
    uint16_t adp{};
    uint16_t ado{};
    std::vector<uint8_t> data;
    uint16_t wkc{};
};

struct SlaveInfo {
    int position{};
    uint16_t adp{};
    uint16_t esc_type{};
    uint16_t station{};
    uint16_t alias{};
    uint16_t al_status{};
    uint16_t dl_status{};
};

struct ScanResult {
    uint16_t slave_count{};
    uint16_t broadcast_esc_type{};
    uint16_t broadcast_al_status{};
    uint16_t broadcast_dl_status{};
    std::vector<SlaveInfo> slaves;
};

class Master {
  public:
    explicit Master(RawSocket& socket) : socket_(socket) {}

    std::optional<DatagramResult> transfer(const DatagramRequest& request);
    std::optional<DatagramResult> read(Command command, uint16_t adp, uint16_t ado, uint16_t length);
    std::optional<DatagramResult> write(Command command, uint16_t adp, uint16_t ado, const std::vector<uint8_t>& data);
    std::optional<DatagramResult> read_logical(uint32_t address, uint16_t length);
    std::optional<DatagramResult> write_logical(uint32_t address, const std::vector<uint8_t>& data);
    std::optional<DatagramResult> read_write_logical(uint32_t address, const std::vector<uint8_t>& data);
    std::optional<ScanResult> scan(int max_slaves);

  private:
    RawSocket& socket_;
    uint8_t index_{1};
};

uint16_t read_le16_data(const DatagramResult& result);
std::string command_name(Command command);
std::string hex_bytes(const std::vector<uint8_t>& data);
std::string hex16(uint16_t value);

} // namespace ecat
