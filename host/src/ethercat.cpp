#include "ecat/ethercat.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace ecat {
namespace {

constexpr size_t kEthernetHeaderSize = 14;
constexpr size_t kEcatFrameHeaderSize = 2;
constexpr size_t kDatagramHeaderSize = 10;
constexpr uint16_t kRegEscType = 0x0000;
constexpr uint16_t kRegStationAddress = 0x0010;
constexpr uint16_t kRegStationAlias = 0x0012;
constexpr uint16_t kRegDlStatus = 0x0110;
constexpr uint16_t kRegAlStatus = 0x0130;

void put_u16_le(std::vector<uint8_t>& out, uint16_t value) {
    out.push_back(static_cast<uint8_t>(value & 0x00ffU));
    out.push_back(static_cast<uint8_t>((value >> 8U) & 0x00ffU));
}

uint16_t get_u16_le(const uint8_t* data) {
    return static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8U);
}

uint16_t logical_adp(uint32_t address) {
    return static_cast<uint16_t>(address & 0x0000ffffU);
}

uint16_t logical_ado(uint32_t address) {
    return static_cast<uint16_t>((address >> 16U) & 0x0000ffffU);
}

std::vector<uint8_t> make_frame(const std::array<uint8_t, 6>& source, const DatagramRequest& request, uint8_t index) {
    if (request.data.size() > 0x07ffU) {
        throw std::runtime_error("EtherCAT datagram payload exceeds 11-bit length field");
    }

    const auto data_len = static_cast<uint16_t>(request.data.size());
    const uint16_t payload_len = static_cast<uint16_t>(kDatagramHeaderSize + data_len + 2U);
    std::vector<uint8_t> frame;
    frame.reserve(64);

    for (size_t i = 0; i < 6; ++i) {
        frame.push_back(0xff);
    }
    frame.insert(frame.end(), source.begin(), source.end());
    frame.push_back(static_cast<uint8_t>((kEtherType >> 8U) & 0x00ffU));
    frame.push_back(static_cast<uint8_t>(kEtherType & 0x00ffU));

    put_u16_le(frame, static_cast<uint16_t>((1U << 12U) | payload_len));
    frame.push_back(static_cast<uint8_t>(request.command));
    frame.push_back(index);
    put_u16_le(frame, request.adp);
    put_u16_le(frame, request.ado);
    put_u16_le(frame, data_len);
    put_u16_le(frame, 0x0000);

    frame.insert(frame.end(), request.data.begin(), request.data.end());
    put_u16_le(frame, 0x0000);

    while (frame.size() < 60U) {
        frame.push_back(0x00);
    }
    return frame;
}

std::optional<DatagramResult> parse_datagram(const std::vector<uint8_t>& frame, uint8_t expected_index) {
    if (frame.size() < kEthernetHeaderSize + kEcatFrameHeaderSize + kDatagramHeaderSize + 2U) {
        return std::nullopt;
    }
    const uint16_t ethertype = static_cast<uint16_t>(frame[12] << 8U) | frame[13];
    if (ethertype != kEtherType) {
        return std::nullopt;
    }

    const size_t base = kEthernetHeaderSize + kEcatFrameHeaderSize;
    const uint8_t index = frame[base + 1U];
    if (index != expected_index) {
        return std::nullopt;
    }

    const uint16_t datagram_len = static_cast<uint16_t>(get_u16_le(&frame[base + 6U]) & 0x07ffU);
    const size_t data_offset = base + kDatagramHeaderSize;
    const size_t wkc_offset = data_offset + datagram_len;
    if (wkc_offset + 2U > frame.size()) {
        return std::nullopt;
    }

    DatagramResult result;
    result.command = static_cast<Command>(frame[base]);
    result.index = index;
    result.adp = get_u16_le(&frame[base + 2U]);
    result.ado = get_u16_le(&frame[base + 4U]);
    result.data.assign(frame.begin() + data_offset, frame.begin() + data_offset + datagram_len);
    result.wkc = get_u16_le(&frame[wkc_offset]);
    return result;
}

uint16_t optional_word(const std::optional<DatagramResult>& result) {
    if (!result || result->wkc == 0U) {
        return 0;
    }
    return read_le16_data(*result);
}

} // namespace

std::optional<DatagramResult> Master::transfer(const DatagramRequest& request) {
    const uint8_t request_index = index_++;
    socket_.send_broadcast(make_frame(socket_.mac(), request, request_index));

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
    while (std::chrono::steady_clock::now() < deadline) {
        auto frame = socket_.receive(std::chrono::milliseconds(50));
        if (!frame || frame->outgoing) {
            continue;
        }
        if (auto parsed = parse_datagram(frame->bytes, request_index)) {
            return parsed;
        }
    }
    return std::nullopt;
}

std::optional<DatagramResult> Master::read(Command command, uint16_t adp, uint16_t ado, uint16_t length) {
    return transfer(DatagramRequest{command, adp, ado, std::vector<uint8_t>(length, 0x00)});
}

std::optional<DatagramResult> Master::write(Command command, uint16_t adp, uint16_t ado,
                                            const std::vector<uint8_t>& data) {
    return transfer(DatagramRequest{command, adp, ado, data});
}

std::optional<DatagramResult> Master::read_logical(uint32_t address, uint16_t length) {
    return read(Command::lrd, logical_adp(address), logical_ado(address), length);
}

std::optional<DatagramResult> Master::write_logical(uint32_t address, const std::vector<uint8_t>& data) {
    return write(Command::lwr, logical_adp(address), logical_ado(address), data);
}

std::optional<DatagramResult> Master::read_write_logical(uint32_t address, const std::vector<uint8_t>& data) {
    return write(Command::lrw, logical_adp(address), logical_ado(address), data);
}

std::optional<ScanResult> Master::scan(int max_slaves) {
    auto brd_type = read(Command::brd, 0x0000, kRegEscType, 2);
    if (!brd_type) {
        return std::nullopt;
    }

    ScanResult result;
    result.slave_count = brd_type->wkc;
    result.broadcast_esc_type = read_le16_data(*brd_type);
    result.broadcast_al_status = optional_word(read(Command::brd, 0x0000, kRegAlStatus, 2));
    result.broadcast_dl_status = optional_word(read(Command::brd, 0x0000, kRegDlStatus, 2));

    const int slots = std::min<int>(max_slaves, result.slave_count);
    result.slaves.reserve(static_cast<size_t>(slots));
    for (int slot = 0; slot < slots; ++slot) {
        const uint16_t adp = static_cast<uint16_t>(-slot);
        auto esc_type = read(Command::aprd, adp, kRegEscType, 2);
        if (!esc_type || esc_type->wkc == 0U) {
            continue;
        }

        SlaveInfo slave;
        slave.position = slot + 1;
        slave.adp = adp;
        slave.esc_type = read_le16_data(*esc_type);
        slave.station = optional_word(read(Command::aprd, adp, kRegStationAddress, 2));
        slave.alias = optional_word(read(Command::aprd, adp, kRegStationAlias, 2));
        slave.al_status = optional_word(read(Command::aprd, adp, kRegAlStatus, 2));
        slave.dl_status = optional_word(read(Command::aprd, adp, kRegDlStatus, 2));
        result.slaves.push_back(slave);
    }
    return result;
}

std::string command_name(Command command) {
    switch (command) {
    case Command::nop:
        return "NOP";
    case Command::aprd:
        return "APRD";
    case Command::apwr:
        return "APWR";
    case Command::aprw:
        return "APRW";
    case Command::fprd:
        return "FPRD";
    case Command::fpwr:
        return "FPWR";
    case Command::fprw:
        return "FPRW";
    case Command::brd:
        return "BRD";
    case Command::bwr:
        return "BWR";
    case Command::brw:
        return "BRW";
    case Command::lrd:
        return "LRD";
    case Command::lwr:
        return "LWR";
    case Command::lrw:
        return "LRW";
    case Command::armw:
        return "ARMW";
    case Command::frmw:
        return "FRMW";
    }
    return "UNKNOWN";
}

uint16_t read_le16_data(const DatagramResult& result) {
    if (result.data.size() < 2U) {
        return 0;
    }
    return get_u16_le(result.data.data());
}

std::string hex_bytes(const std::vector<uint8_t>& data) {
    std::ostringstream oss;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i != 0U) {
            oss << ' ';
        }
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(data[i]);
    }
    return oss.str();
}

std::string hex16(uint16_t value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::setw(4) << std::setfill('0') << value;
    return oss.str();
}

} // namespace ecat
