#include "ecat/ethercat.hpp"

#include <exception>
#include <iostream>
#include <optional>

namespace {

void usage(const char* argv0) {
    std::cerr << "usage: " << argv0 << " <interface> [max-slaves]\n";
}

void print_datagram_probe(const char* label, const std::optional<ecat::DatagramResult>& result) {
    if (!result) {
        std::cout << label << ": timeout\n";
        return;
    }
    std::cout << label << ": cmd=" << ecat::command_name(result->command) << " adp=" << ecat::hex16(result->adp)
              << " ado=" << ecat::hex16(result->ado) << " data=[" << ecat::hex_bytes(result->data)
              << "] wkc=" << result->wkc << "\n";
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return 2;
    }

    try {
        const int max_slaves = (argc == 3) ? std::stoi(argv[2]) : 16;
        if (max_slaves <= 0 || max_slaves > 128) {
            std::cerr << "max-slaves must be 1..128\n";
            return 2;
        }

        ecat::RawSocket socket(argv[1], ecat::kEtherType);
        ecat::Master master(socket);

        std::cout << "ecat_scan_xmc4800: interface=" << socket.interface_name()
                  << " mac=" << ecat::mac_to_string(socket.mac()) << "\n";
        std::cout << "probe: EtherType 0x88a4, raw AF_PACKET\n";
        std::cout << "commands: NOP APRD APWR APRW FPRD FPWR FPRW BRD BWR BRW LRD LWR LRW ARMW FRMW\n";

        print_datagram_probe("datagram BRD ESC type", master.read(ecat::Command::brd, 0x0000, 0x0000, 2));

        const auto scan = master.scan(max_slaves);
        if (!scan) {
            std::cout << "no EtherCAT response. Check cable, interface, and privileges.\n";
            return 1;
        }

        std::cout << "BRD ESC type: wkc=" << scan->slave_count << " value=" << ecat::hex16(scan->broadcast_esc_type)
                  << "\n";
        std::cout << "BRD AL status: wkc=" << scan->slave_count << " value=" << ecat::hex16(scan->broadcast_al_status)
                  << "\n";
        std::cout << "BRD DL status: wkc=" << scan->slave_count << " value=" << ecat::hex16(scan->broadcast_dl_status)
                  << "\n";
        std::cout << "slaves detected by WKC: " << scan->slave_count << "\n";

        for (const auto& slave : scan->slaves) {
            std::cout << "slave[" << slave.position << "] adp=" << ecat::hex16(slave.adp)
                      << " esc_type=" << ecat::hex16(slave.esc_type) << " station=" << ecat::hex16(slave.station)
                      << " alias=" << ecat::hex16(slave.alias) << " al=" << ecat::hex16(slave.al_status)
                      << " dl=" << ecat::hex16(slave.dl_status) << "\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
