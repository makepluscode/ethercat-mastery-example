// ETH0 (RMII): init + poll; logs raw RX frames to SEGGER RTT.
#pragma once

namespace app::eth {

void init();

// Call from main only; never from ISR (printf/RTT here).
void poll_and_log();

} // namespace app::eth
