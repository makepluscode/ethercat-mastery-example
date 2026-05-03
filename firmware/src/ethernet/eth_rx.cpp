// ETH0 + KSZ8081 RMII, ECAT block pinmux (kit), RX ISR -> main -> RTT log.
#include "ethernet/eth_rx.hpp"
#include "logging/log.hpp"

#include <cstddef>
#include <cstdio>

extern "C" {
#include "XMC4800.h"
#include "xmc_ecat.h"
#include "xmc_eth_mac.h"
#include "xmc_eth_phy.h"
#include "xmc_gpio.h"
}

// --- Board pin macros (Infineon Relax EtherCAT) --------------------------------

static constexpr uint8_t kEthPhyAddr = 0;

#define ETH_RXD1 P2_3
#define ETH_RXD0 P2_2
#define ETH_RXER P2_4
#define ETH_CLK_RMII P15_8
#define ETH_TX_EN P2_5
#define ETH_TXD1 P2_9
#define ETH_TXD0 P2_8
#define ETH_CRS_DV P15_9
#define ETH_MDIO P2_0
#define ETH_MDC P2_7

#define ECAT_MDO P0_12
#define ECAT_MCLK P3_3
#define ECAT_CLK25 P6_0
#define ECAT_PHY_RESET P0_0
#define ECAT_LED_RUN P0_8
#define ECAT_LED_ERR P0_7

#define ECAT_P0_LINK_STATUS P1_15
#define ECAT_P0_LED_LINK_ACT P6_3
#define ECAT_P0_RXD3 P5_7
#define ECAT_P0_RXD2 P5_2
#define ECAT_P0_RXD1 P5_1
#define ECAT_P0_RXD0 P5_0
#define ECAT_P0_RX_DV P5_6
#define ECAT_P0_RX_CLK P5_4
#define ECAT_P0_RX_ERR P2_6
#define ECAT_P0_TXD3 P6_6
#define ECAT_P0_TXD2 P6_5
#define ECAT_P0_TXD1 P6_4
#define ECAT_P0_TXD0 P6_2
#define ECAT_P0_TX_EN P6_1
#define ECAT_P0_TX_CLK P5_5

#define ECAT_P1_LINK_STATUS P3_4
#define ECAT_P1_LED_LINK_ACT P3_12
#define ECAT_P1_RXD3 P0_4
#define ECAT_P1_RXD2 P0_5
#define ECAT_P1_RXD1 P0_6
#define ECAT_P1_RXD0 P0_11
#define ECAT_P1_RX_DV P0_9
#define ECAT_P1_RX_CLK P0_1
#define ECAT_P1_RX_ERR P15_2
#define ECAT_P1_TXD3 P0_3
#define ECAT_P1_TXD2 P0_2
#define ECAT_P1_TXD1 P3_2
#define ECAT_P1_TXD0 P3_1
#define ECAT_P1_TX_EN P3_0
#define ECAT_P1_TX_CLK P0_10

// Station MAC (example; change if you need a unique OUI on LAN)
static constexpr uint64_t kMacAddr = (uint64_t{0x00} | (uint64_t{0x00} << 8) | (uint64_t{0x45} << 16) |
                                      (uint64_t{0x19} << 24) | (uint64_t{0x03} << 32) | (uint64_t{0x00} << 40));

static constexpr uint32_t kNumRxBuf = 4;
static constexpr uint32_t kNumTxBuf = 4;

// DMA + frame storage in ETH_RAM (linker)
#if defined(__GNUC__)
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t s_rx_desc[kNumRxBuf] __attribute__((section("ETH_RAM")));
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t s_tx_desc[kNumTxBuf] __attribute__((section("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t s_rx_buf[kNumRxBuf][XMC_ETH_MAC_BUF_SIZE]
    __attribute__((section("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t s_tx_buf[kNumTxBuf][XMC_ETH_MAC_BUF_SIZE]
    __attribute__((section("ETH_RAM")));
#endif

static XMC_ETH_PHY_CONFIG_t s_phy_cfg{};
static XMC_ETH_MAC_t s_eth_mac{};

static void init_eth_driver_static_state(void) {
    s_phy_cfg.interface = XMC_ETH_LINK_INTERFACE_RMII;
    s_phy_cfg.enable_auto_negotiate = true;

    s_eth_mac.regs = ETH0;
    s_eth_mac.address = kMacAddr;
    s_eth_mac.rx_desc = s_rx_desc;
    s_eth_mac.tx_desc = s_tx_desc;
    s_eth_mac.rx_buf = &s_rx_buf[0][0];
    s_eth_mac.tx_buf = &s_tx_buf[0][0];
    s_eth_mac.num_rx_buf = static_cast<uint8_t>(kNumRxBuf);
    s_eth_mac.num_tx_buf = static_cast<uint8_t>(kNumTxBuf);
}

alignas(4) static uint8_t s_rx_frame[XMC_ETH_MAC_BUF_SIZE];

static volatile bool g_rx_pending = false;
static volatile uint32_t g_rx_len = 0;

static void ecat_init_kit_pins(void) {
    XMC_ECAT_PORT_CTRL_t port_control{};
    XMC_ECAT_CONFIG_t ecat_config{};
    XMC_GPIO_CONFIG_t gpio_config{};

    gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
    XMC_GPIO_Init(ECAT_P0_LINK_STATUS, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RXD3, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RXD2, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RXD1, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RXD0, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RX_DV, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RX_CLK, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_RX_ERR, &gpio_config);
    XMC_GPIO_Init(ECAT_P0_TX_CLK, &gpio_config);

    XMC_GPIO_Init(ECAT_P1_LINK_STATUS, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RXD3, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RXD2, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RXD1, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RXD0, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RX_DV, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RX_CLK, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_RX_ERR, &gpio_config);
    XMC_GPIO_Init(ECAT_P1_TX_CLK, &gpio_config);

    XMC_GPIO_Init(ECAT_MDO, &gpio_config);

    port_control.common.mdio = XMC_ECAT_PORT_CTRL_MDIO_P0_12;
    port_control.port0.rxd0 = XMC_ECAT_PORT0_CTRL_RXD0_P5_0;
    port_control.port0.rxd1 = XMC_ECAT_PORT0_CTRL_RXD1_P5_1;
    port_control.port0.rxd2 = XMC_ECAT_PORT0_CTRL_RXD2_P5_2;
    port_control.port0.rxd3 = XMC_ECAT_PORT0_CTRL_RXD3_P5_7;
    port_control.port0.rx_clk = XMC_ECAT_PORT0_CTRL_RX_CLK_P5_4;
    port_control.port0.rx_dv = XMC_ECAT_PORT0_CTRL_RX_DV_P5_6;
    port_control.port0.rx_err = XMC_ECAT_PORT0_CTRL_RX_ERR_P2_6;
    port_control.port0.link = XMC_ECAT_PORT0_CTRL_LINK_P1_15;
    port_control.port0.tx_clk = XMC_ECAT_PORT0_CTRL_TX_CLK_P5_5;
    port_control.port1.rxd0 = XMC_ECAT_PORT1_CTRL_RXD0_P0_11;
    port_control.port1.rxd1 = XMC_ECAT_PORT1_CTRL_RXD1_P0_6;
    port_control.port1.rxd2 = XMC_ECAT_PORT1_CTRL_RXD2_P0_5;
    port_control.port1.rxd3 = XMC_ECAT_PORT1_CTRL_RXD3_P0_4;
    port_control.port1.rx_clk = XMC_ECAT_PORT1_CTRL_RX_CLK_P0_1;
    port_control.port1.rx_dv = XMC_ECAT_PORT1_CTRL_RX_DV_P0_9;
    port_control.port1.rx_err = XMC_ECAT_PORT1_CTRL_RX_ERR_P15_2;
    port_control.port1.link = XMC_ECAT_PORT1_CTRL_LINK_P3_4;
    port_control.port1.tx_clk = XMC_ECAT_PORT1_CTRL_TX_CLK_P0_10;
    XMC_ECAT_SetPortControl(port_control);

    ecat_config.enable_dc_sync_out = false;
    ecat_config.enable_dc_latch_in = false;
    ecat_config.enable_enhanced_link_p0 = true;
    ecat_config.enable_enhanced_link_p1 = true;
    ecat_config.sync_pulse_length = 1000;
    ecat_config.station_alias = 0;
    ecat_config.checksum = 0x88A4;

    XMC_ECAT_Init(&ecat_config);

    gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
    gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE;
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_6_AF_ECAT0_P0_TXD3);
    XMC_GPIO_Init(ECAT_P0_TXD3, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_5_AF_ECAT0_P0_TXD2);
    XMC_GPIO_Init(ECAT_P0_TXD2, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_4_AF_ECAT0_P0_TXD1);
    XMC_GPIO_Init(ECAT_P0_TXD1, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_2_AF_ECAT0_P0_TXD0);
    XMC_GPIO_Init(ECAT_P0_TXD0, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_1_AF_ECAT0_P0_TX_ENA);
    XMC_GPIO_Init(ECAT_P0_TX_EN, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P0_3_AF_ECAT0_P1_TXD3);
    XMC_GPIO_Init(ECAT_P1_TXD3, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P0_2_AF_ECAT0_P1_TXD2);
    XMC_GPIO_Init(ECAT_P1_TXD2, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P3_2_AF_ECAT0_P1_TXD1);
    XMC_GPIO_Init(ECAT_P1_TXD1, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P3_1_AF_ECAT0_P1_TXD0);
    XMC_GPIO_Init(ECAT_P1_TXD0, &gpio_config);
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P3_0_AF_ECAT0_P1_TX_ENA);
    XMC_GPIO_Init(ECAT_P1_TX_EN, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_0_AF_ECAT0_PHY_CLK25);
    XMC_GPIO_Init(ECAT_CLK25, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P3_3_AF_ECAT0_MCLK);
    XMC_GPIO_Init(ECAT_MCLK, &gpio_config);

    gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE;
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P6_3_AF_ECAT0_P0_LED_LINK_ACT);
    XMC_GPIO_Init(ECAT_P0_LED_LINK_ACT, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P3_12_AF_ECAT0_P1_LED_LINK_ACT);
    XMC_GPIO_Init(ECAT_P1_LED_LINK_ACT, &gpio_config);

    XMC_GPIO_SetHardwareControl(ECAT_MDO, P0_12_HWCTRL_ECAT0_MDO);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P0_0_AF_ECAT0_PHY_RESET);
    XMC_GPIO_Init(ECAT_PHY_RESET, &gpio_config);
}

static void wait_link_and_set_mac(void) {
    /* Without copper link the MAC RX path never enables; this loop is easy to mistake for a hang. */
    uint32_t spin = 0;
    while (XMC_ETH_PHY_GetLinkStatus(&s_eth_mac, kEthPhyAddr) != XMC_ETH_LINK_STATUS_UP) {
        if ((spin++ % 25000000U) == 0U) {
            app::log::info("PHY link still DOWN; use Ethernet jack X400 (KSZ8081), not ECAT IN/OUT\n");
        }
    }
    const XMC_ETH_LINK_SPEED_t speed = XMC_ETH_PHY_GetLinkSpeed(&s_eth_mac, kEthPhyAddr);
    const XMC_ETH_LINK_DUPLEX_t duplex = XMC_ETH_PHY_GetLinkDuplex(&s_eth_mac, kEthPhyAddr);
    XMC_ETH_MAC_SetLink(&s_eth_mac, speed, duplex);
}

static void eth_mac_init(void) {
    init_eth_driver_static_state();

    XMC_ETH_MAC_PORT_CTRL_t port_control{};
    XMC_GPIO_CONFIG_t gpio_config{};

    // Kit RUN/ERR LEDs (used in poll_and_log); not configured by ECAT pinmux block above.
    gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
    XMC_GPIO_Init(ECAT_LED_ERR, &gpio_config);
    XMC_GPIO_Init(ECAT_LED_RUN, &gpio_config);
    XMC_GPIO_SetOutputLow(ECAT_LED_ERR);
    XMC_GPIO_SetOutputLow(ECAT_LED_RUN);

    gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
    XMC_GPIO_Init(ETH_RXD0, &gpio_config);
    XMC_GPIO_Init(ETH_RXD1, &gpio_config);
    XMC_GPIO_Init(ETH_CLK_RMII, &gpio_config);
    XMC_GPIO_Init(ETH_CRS_DV, &gpio_config);
    XMC_GPIO_Init(ETH_RXER, &gpio_config);
    XMC_GPIO_Init(ETH_MDIO, &gpio_config);

    port_control.mode = XMC_ETH_MAC_PORT_CTRL_MODE_RMII;
    port_control.rxd0 = XMC_ETH_MAC_PORT_CTRL_RXD0_P2_2;
    port_control.rxd1 = XMC_ETH_MAC_PORT_CTRL_RXD1_P2_3;
    port_control.clk_rmii = XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P15_8;
    port_control.crs_dv = XMC_ETH_MAC_PORT_CTRL_CRS_DV_P15_9;
    port_control.rxer = XMC_ETH_MAC_PORT_CTRL_RXER_P2_4;
    port_control.mdio = XMC_ETH_MAC_PORT_CTRL_MDIO_P2_0;
    XMC_ETH_MAC_SetPortControl(&s_eth_mac, port_control);

    XMC_ETH_MAC_Init(&s_eth_mac);
    XMC_ETH_MAC_DisableJumboFrame(&s_eth_mac);
    /* Pass all destination addresses to the DMA RX path (lab / promiscuous logging). */
    XMC_ETH_MAC_DisableFrameFilter(&s_eth_mac);

    gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
    gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE;
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_8_AF_ETH0_TXD0);
    XMC_GPIO_Init(ETH_TXD0, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_9_AF_ETH0_TXD1);
    XMC_GPIO_Init(ETH_TXD1, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_5_AF_ETH0_TX_EN);
    XMC_GPIO_Init(ETH_TX_EN, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_7_AF_ETH0_MDC);
    XMC_GPIO_Init(ETH_MDC, &gpio_config);

    XMC_GPIO_SetHardwareControl(ETH_MDIO, P2_0_HWCTRL_ETH0_MDO);

    const int32_t phy_st = XMC_ETH_PHY_Init(&s_eth_mac, kEthPhyAddr, &s_phy_cfg);
    if (phy_st != (int32_t)XMC_ETH_PHY_STATUS_OK) {
        uint16_t id1 = 0;
        uint16_t id2 = 0;
        (void)XMC_ETH_MAC_ReadPhy(&s_eth_mac, kEthPhyAddr, 2U, &id1);
        (void)XMC_ETH_MAC_ReadPhy(&s_eth_mac, kEthPhyAddr, 3U, &id2);
        app::log::info("ETH PHY init failed (st=%ld) PHYID1=%04x PHYID2=%04x addr=%u\n", static_cast<long>(phy_st),
                       static_cast<unsigned>(id1), static_cast<unsigned>(id2), static_cast<unsigned>(kEthPhyAddr));
        while (true) {
        }
    }

    app::log::info("PHY: X400 link is optional; ECAT status logging continues while X400 is down\n");

    XMC_ETH_MAC_EnableEvent(&s_eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);

    NVIC_SetPriority(ETH0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0));
    NVIC_ClearPendingIRQ(ETH0_0_IRQn);
    NVIC_EnableIRQ(ETH0_0_IRQn);

    XMC_ETH_MAC_EnableTx(&s_eth_mac);
    XMC_ETH_MAC_EnableRx(&s_eth_mac);
}

static void recover_link_after_reconnect(void) {
    XMC_ETH_MAC_DisableRx(&s_eth_mac);
    XMC_ETH_MAC_DisableTx(&s_eth_mac);

    XMC_ETH_PHY_Init(&s_eth_mac, kEthPhyAddr, &s_phy_cfg);

    wait_link_and_set_mac();

    uint32_t st = XMC_ETH_MAC_GetEventStatus(&s_eth_mac);
    XMC_ETH_MAC_ClearEventStatus(&s_eth_mac, st);
    XMC_ETH_MAC_EnableEvent(&s_eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);

    NVIC_ClearPendingIRQ(ETH0_0_IRQn);

    XMC_ETH_MAC_EnableRx(&s_eth_mac);
    XMC_ETH_MAC_EnableTx(&s_eth_mac);
}

static void poll_ecat_status_log() {
    static bool first = true;
    static uint32_t last_p0 = 0;
    static uint32_t last_p1 = 0;
    static uint16_t last_al_status = 0xffffU;
    static uint16_t last_al_event = 0xffffU;
    static uint32_t last_status = 0xffffffffU;

    const uint32_t p0 = XMC_GPIO_GetInput(ECAT_P0_LINK_STATUS) ? 1U : 0U;
    const uint32_t p1 = XMC_GPIO_GetInput(ECAT_P1_LINK_STATUS) ? 1U : 0U;
    const uint16_t al_status = ECAT0->AL_STATUS;
    const uint16_t al_event = XMC_ECAT_GetALEventRegister();
    const uint32_t status = ECAT0->STATUS;

    if (first || p0 != last_p0 || p1 != last_p1 || al_status != last_al_status || al_event != last_al_event ||
        status != last_status) {
        first = false;
        last_p0 = p0;
        last_p1 = p1;
        last_al_status = al_status;
        last_al_event = al_event;
        last_status = status;

        app::log::info("ECAT: p0_link=%lu p1_link=%lu al=0x%04x al_event=0x%04x status=0x%08lx\n",
                       static_cast<unsigned long>(p0), static_cast<unsigned long>(p1), static_cast<unsigned>(al_status),
                       static_cast<unsigned>(al_event), static_cast<unsigned long>(status));
    }
}

// RX: copy one frame for main-thread logging (RTT).
extern "C" void ETH0_0_IRQHandler(void) {
    const uint32_t status = XMC_ETH_MAC_GetEventStatus(&s_eth_mac);

    if ((status & XMC_ETH_MAC_EVENT_RECEIVE) != 0U) {
        XMC_ETH_MAC_DisableEvent(&s_eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);

        uint32_t len = XMC_ETH_MAC_GetRxFrameSize(&s_eth_mac);
        if (len > XMC_ETH_MAC_BUF_SIZE) {
            len = XMC_ETH_MAC_BUF_SIZE;
        }

        XMC_ETH_MAC_ReadFrame(&s_eth_mac, &s_rx_frame[0], len);

        g_rx_len = len;
        g_rx_pending = true;

        XMC_ETH_MAC_EnableEvent(&s_eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);
    }

    XMC_ETH_MAC_ClearEventStatus(&s_eth_mac, status);
}

namespace app::eth {

void init() {
    ecat_init_kit_pins();
    app::log::info("ETH: bringing up KSZ8081 on silkscreen X400 (not EtherCAT jacks)...\n");
    eth_mac_init();
}

void poll_and_log() {
    static bool link_init_done = false;
    static bool link_prev = false;

    const bool link_now = (XMC_ETH_PHY_GetLinkStatus(&s_eth_mac, kEthPhyAddr) == XMC_ETH_LINK_STATUS_UP);

    if (!link_init_done) {
        link_init_done = true;
        app::log::info("PHY link %s\n", link_now ? "UP" : "DOWN");
    } else if (link_now != link_prev) {
        app::log::info("PHY link %s\n", link_now ? "UP" : "DOWN");
    }

    if (!link_now) {
        XMC_GPIO_SetOutputHigh(ECAT_LED_ERR);
        XMC_GPIO_SetOutputLow(ECAT_LED_RUN);
    } else {
        XMC_GPIO_SetOutputLow(ECAT_LED_ERR);
    }

    if (link_init_done && (!link_prev) && link_now) {
        recover_link_after_reconnect();
    }
    link_prev = link_now;

    poll_ecat_status_log();

    if (!g_rx_pending) {
        return;
    }

    const uint32_t len = g_rx_len;
    g_rx_pending = false;

    unsigned et_hi = (len >= 14U) ? s_rx_frame[12] : 0U;
    unsigned et_lo = (len >= 14U) ? s_rx_frame[13] : 0U;
    app::log::info("RX len=%lu ethertype=%02x%02x%s\n", static_cast<unsigned long>(len), et_hi, et_lo,
                   (et_hi == 0x88U && et_lo == 0xa4U) ? " EtherCAT probe" : "");

    const uint32_t dump_len = (len < 60U) ? len : 60U;
    for (uint32_t off = 0; off < dump_len; off += 16U) {
        char line[3 * 16 + 4]{};
        std::size_t pos = 0;
        const uint32_t end = (off + 16U < dump_len) ? (off + 16U) : dump_len;
        for (uint32_t i = off; i < end; ++i) {
            const int n = std::snprintf(line + pos, sizeof(line) - pos, "%02x ", s_rx_frame[i]);
            if (n > 0) {
                pos += static_cast<std::size_t>(n);
            }
        }
        app::log::info("  %s\n", line);
    }

    XMC_GPIO_SetOutputHigh(ECAT_LED_RUN);
}

} // namespace app::eth
