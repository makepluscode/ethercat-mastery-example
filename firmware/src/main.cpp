// Entry: BSP, RTT log, ETH RX loop (raw frames to RTT).
extern "C" {
#include "SEGGER_RTT.h"
}

#include "cybsp.h"
#include "ethernet/eth_rx.hpp"
#include "logging/log.hpp"

namespace {

[[noreturn]] void halt_on_failure() {
    while (true) {
        // Failed before UART logging is usable; stop for debugger attach.
    }
}

} // namespace

int main() {
    SEGGER_RTT_Init();

    const cy_rslt_t err = cybsp_init();
    if (err != CY_RSLT_SUCCESS) {
        halt_on_failure();
    }

    app::log::line("Boot: cybsp_init OK");
    app::log::info("Log: SEGGER RTT channel 0 (J-Link RTT Client)\n");
    app::log::info("Build: C++%ld\n", static_cast<long>(__cplusplus));

    app::eth::init();
    app::log::line("ETH/ECAT logs enabled: ECAT IN for WKC scan, X400 for raw RX probe frames.");

    while (true) {
        app::eth::poll_and_log();
    }
}
