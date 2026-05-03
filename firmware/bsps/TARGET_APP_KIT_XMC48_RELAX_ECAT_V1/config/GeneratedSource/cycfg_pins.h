/*******************************************************************************
 * File Name: cycfg_pins.h
 *
 * Description:
 * Pin configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.70.0
 * device-db 4.37.0.10260
 * mtb-xmclib-cat3 4.7.0.5494
 *
 *******************************************************************************
 * Copyright 2026 Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "xmc_gpio.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CYBSP_ECAT_LED_ERR_ENABLED 1U
#define CYBSP_ECAT_LED_ERR_PORT XMC_GPIO_PORT0
#define CYBSP_ECAT_LED_ERR_PORT_NUM 0U
#define CYBSP_ECAT_LED_ERR_PIN 7U
#ifndef ioss_0_port_0_pin_7_ALT
    #define ioss_0_port_0_pin_7_ALT 0U
#endif
#define CYBSP_ECAT_LED_ERR_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_0_pin_7_ALT)
#ifndef ioss_0_port_0_pin_7_HWO
    #define ioss_0_port_0_pin_7_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED_ERR_HWO ioss_0_port_0_pin_7_HWO
#define CYBSP_ECAT_LED_RUN_ENABLED 1U
#define CYBSP_ECAT_LED_RUN_PORT XMC_GPIO_PORT0
#define CYBSP_ECAT_LED_RUN_PORT_NUM 0U
#define CYBSP_ECAT_LED_RUN_PIN 8U
#ifndef ioss_0_port_0_pin_8_ALT
    #define ioss_0_port_0_pin_8_ALT 0U
#endif
#define CYBSP_ECAT_LED_RUN_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_0_pin_8_ALT)
#ifndef ioss_0_port_0_pin_8_HWO
    #define ioss_0_port_0_pin_8_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED_RUN_HWO ioss_0_port_0_pin_8_HWO
#define CYBSP_D15_PORT XMC_GPIO_PORT0
#define CYBSP_I2C_SCL_PORT CYBSP_D15_PORT
#define CYBSP_D15_PORT_NUM 0U
#define CYBSP_I2C_SCL_PORT_NUM CYBSP_D15_PORT_NUM
#define CYBSP_D15_PIN 13U
#define CYBSP_I2C_SCL_PIN CYBSP_D15_PIN
#define CYBSP_ECAT_LED2_ENABLED 1U
#define CYBSP_ECAT_LED2_PORT XMC_GPIO_PORT0
#define CYBSP_ECAT_LED2_PORT_NUM 0U
#define CYBSP_ECAT_LED2_PIN 14U
#ifndef ioss_0_port_0_pin_14_ALT
    #define ioss_0_port_0_pin_14_ALT 0U
#endif
#define CYBSP_ECAT_LED2_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_0_pin_14_ALT)
#ifndef ioss_0_port_0_pin_14_HWO
    #define ioss_0_port_0_pin_14_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED2_HWO ioss_0_port_0_pin_14_HWO
#define CYBSP_ECAT_LED4_ENABLED 1U
#define CYBSP_ECAT_LED4_PORT XMC_GPIO_PORT0
#define CYBSP_ECAT_LED4_PORT_NUM 0U
#define CYBSP_ECAT_LED4_PIN 15U
#ifndef ioss_0_port_0_pin_15_ALT
    #define ioss_0_port_0_pin_15_ALT 0U
#endif
#define CYBSP_ECAT_LED4_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_0_pin_15_ALT)
#ifndef ioss_0_port_0_pin_15_HWO
    #define ioss_0_port_0_pin_15_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED4_HWO ioss_0_port_0_pin_15_HWO
#define CYBSP_D2_PORT XMC_GPIO_PORT1
#define CYBSP_D2_PORT_NUM 1U
#define CYBSP_D2_PIN 0U
#define CYBSP_D3_PORT XMC_GPIO_PORT1
#define CYBSP_D3_PORT_NUM 1U
#define CYBSP_D3_PIN 1U
#define CYBSP_DEBUG_UART_RX_ENABLED 1U
#define CYBSP_DEBUG_UART_RX_PORT XMC_GPIO_PORT1
#define CYBSP_DEBUG_UART_RX_PORT_NUM 1U
#define CYBSP_DEBUG_UART_RX_PIN 4U
#ifndef ioss_0_port_1_pin_4_INPUT
    #define ioss_0_port_1_pin_4_INPUT 0U
#endif
#define CYBSP_DEBUG_UART_RX_MODE (XMC_GPIO_MODE_INPUT_TRISTATE | ioss_0_port_1_pin_4_INPUT)
#ifndef ioss_0_port_1_pin_4_HWO
    #define ioss_0_port_1_pin_4_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_DEBUG_UART_RX_HWO ioss_0_port_1_pin_4_HWO
#define CYBSP_DEBUG_UART_TX_ENABLED 1U
#define CYBSP_DEBUG_UART_TX_PORT XMC_GPIO_PORT1
#define CYBSP_DEBUG_UART_TX_PORT_NUM 1U
#define CYBSP_DEBUG_UART_TX_PIN 5U
#ifndef ioss_0_port_1_pin_5_ALT
    #define ioss_0_port_1_pin_5_ALT 0U
#endif
#define CYBSP_DEBUG_UART_TX_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_1_pin_5_ALT)
#ifndef ioss_0_port_1_pin_5_HWO
    #define ioss_0_port_1_pin_5_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_DEBUG_UART_TX_HWO ioss_0_port_1_pin_5_HWO
#define CYBSP_D4_PORT XMC_GPIO_PORT1
#define CYBSP_D4_PORT_NUM 1U
#define CYBSP_D4_PIN 8U
#define CYBSP_D7_PORT XMC_GPIO_PORT1
#define CYBSP_D7_PORT_NUM 1U
#define CYBSP_D7_PIN 9U
#define CYBSP_D8_PORT XMC_GPIO_PORT1
#define CYBSP_D8_PORT_NUM 1U
#define CYBSP_D8_PIN 10U
#define CYBSP_D9_PORT XMC_GPIO_PORT1
#define CYBSP_D9_PORT_NUM 1U
#define CYBSP_D9_PIN 11U
#define CYBSP_ECAT_LED5_ENABLED 1U
#define CYBSP_ECAT_LED5_PORT XMC_GPIO_PORT1
#define CYBSP_ECAT_LED5_PORT_NUM 1U
#define CYBSP_ECAT_LED5_PIN 12U
#ifndef ioss_0_port_1_pin_12_ALT
    #define ioss_0_port_1_pin_12_ALT 0U
#endif
#define CYBSP_ECAT_LED5_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_1_pin_12_ALT)
#ifndef ioss_0_port_1_pin_12_HWO
    #define ioss_0_port_1_pin_12_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED5_HWO ioss_0_port_1_pin_12_HWO
#define CYBSP_CAN_RX_PORT XMC_GPIO_PORT1
#define CYBSP_CAN_RX_PORT_NUM 1U
#define CYBSP_CAN_RX_PIN 13U
#define CYBSP_ECAT_LED1_ENABLED 1U
#define CYBSP_ECAT_LED1_PORT XMC_GPIO_PORT1
#define CYBSP_ECAT_LED1_PORT_NUM 1U
#define CYBSP_ECAT_LED1_PIN 14U
#ifndef ioss_0_port_1_pin_14_ALT
    #define ioss_0_port_1_pin_14_ALT 0U
#endif
#define CYBSP_ECAT_LED1_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_1_pin_14_ALT)
#ifndef ioss_0_port_1_pin_14_HWO
    #define ioss_0_port_1_pin_14_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED1_HWO ioss_0_port_1_pin_14_HWO
#define CYBSP_D6_PORT XMC_GPIO_PORT2
#define CYBSP_D6_PORT_NUM 2U
#define CYBSP_D6_PIN 11U
#define CYBSP_D5_PORT XMC_GPIO_PORT2
#define CYBSP_D5_PORT_NUM 2U
#define CYBSP_D5_PIN 12U
#define CYBSP_D1_PORT XMC_GPIO_PORT2
#define CYBSP_D1_PORT_NUM 2U
#define CYBSP_D1_PIN 14U
#define CYBSP_D0_PORT XMC_GPIO_PORT2
#define CYBSP_D0_PORT_NUM 2U
#define CYBSP_D0_PIN 15U
#define CYBSP_D12_PORT XMC_GPIO_PORT3
#define CYBSP_D12_PORT_NUM 3U
#define CYBSP_D12_PIN 7U
#define CYBSP_D11_PORT XMC_GPIO_PORT3
#define CYBSP_D11_PORT_NUM 3U
#define CYBSP_D11_PIN 8U
#define CYBSP_D13_PORT XMC_GPIO_PORT3
#define CYBSP_D13_PORT_NUM 3U
#define CYBSP_D13_PIN 9U
#define CYBSP_D10_PORT XMC_GPIO_PORT3
#define CYBSP_D10_PORT_NUM 3U
#define CYBSP_D10_PIN 10U
#define CYBSP_ECAT_LED8_ENABLED 1U
#define CYBSP_ECAT_LED8_PORT XMC_GPIO_PORT3
#define CYBSP_ECAT_LED8_PORT_NUM 3U
#define CYBSP_ECAT_LED8_PIN 11U
#ifndef ioss_0_port_3_pin_11_ALT
    #define ioss_0_port_3_pin_11_ALT 0U
#endif
#define CYBSP_ECAT_LED8_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_3_pin_11_ALT)
#ifndef ioss_0_port_3_pin_11_HWO
    #define ioss_0_port_3_pin_11_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED8_HWO ioss_0_port_3_pin_11_HWO
#define CYBSP_ECAT_LED6_ENABLED 1U
#define CYBSP_ECAT_LED6_PORT XMC_GPIO_PORT3
#define CYBSP_ECAT_LED6_PORT_NUM 3U
#define CYBSP_ECAT_LED6_PIN 13U
#ifndef ioss_0_port_3_pin_13_ALT
    #define ioss_0_port_3_pin_13_ALT 0U
#endif
#define CYBSP_ECAT_LED6_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_3_pin_13_ALT)
#ifndef ioss_0_port_3_pin_13_HWO
    #define ioss_0_port_3_pin_13_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED6_HWO ioss_0_port_3_pin_13_HWO
#define CYBSP_ECAT_LED3_ENABLED 1U
#define CYBSP_ECAT_LED3_PORT XMC_GPIO_PORT3
#define CYBSP_ECAT_LED3_PORT_NUM 3U
#define CYBSP_ECAT_LED3_PIN 14U
#ifndef ioss_0_port_3_pin_14_ALT
    #define ioss_0_port_3_pin_14_ALT 0U
#endif
#define CYBSP_ECAT_LED3_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_3_pin_14_ALT)
#ifndef ioss_0_port_3_pin_14_HWO
    #define ioss_0_port_3_pin_14_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED3_HWO ioss_0_port_3_pin_14_HWO
#define CYBSP_D14_PORT XMC_GPIO_PORT3
#define CYBSP_I2C_SDA_PORT CYBSP_D14_PORT
#define CYBSP_D14_PORT_NUM 3U
#define CYBSP_I2C_SDA_PORT_NUM CYBSP_D14_PORT_NUM
#define CYBSP_D14_PIN 15U
#define CYBSP_I2C_SDA_PIN CYBSP_D14_PIN
#define CYBSP_QSPI_SCK_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_SCK_PORT_NUM 4U
#define CYBSP_QSPI_SCK_PIN 2U
#define CYBSP_QSPI_SS_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_SS_PORT_NUM 4U
#define CYBSP_QSPI_SS_PIN 3U
#define CYBSP_QSPI_D2_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_D2_PORT_NUM 4U
#define CYBSP_QSPI_D2_PIN 4U
#define CYBSP_QSPI_D3_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_D3_PORT_NUM 4U
#define CYBSP_QSPI_D3_PIN 5U
#define CYBSP_QSPI_D1_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_D1_PORT_NUM 4U
#define CYBSP_QSPI_D1_PIN 6U
#define CYBSP_QSPI_D0_PORT XMC_GPIO_PORT4
#define CYBSP_QSPI_D0_PORT_NUM 4U
#define CYBSP_QSPI_D0_PIN 7U
#define CYBSP_ECAT_LED7_ENABLED 1U
#define CYBSP_ECAT_LED7_PORT XMC_GPIO_PORT5
#define CYBSP_ECAT_LED7_PORT_NUM 5U
#define CYBSP_ECAT_LED7_PIN 3U
#ifndef ioss_0_port_5_pin_3_ALT
    #define ioss_0_port_5_pin_3_ALT 0U
#endif
#define CYBSP_ECAT_LED7_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_5_pin_3_ALT)
#ifndef ioss_0_port_5_pin_3_HWO
    #define ioss_0_port_5_pin_3_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_ECAT_LED7_HWO ioss_0_port_5_pin_3_HWO
#define CYBSP_USER_LED2_ENABLED 1U
#define CYBSP_USER_LED2_PORT XMC_GPIO_PORT5
#define CYBSP_USER_LED2_PORT_NUM 5U
#define CYBSP_USER_LED2_PIN 8U
#ifndef ioss_0_port_5_pin_8_ALT
    #define ioss_0_port_5_pin_8_ALT 0U
#endif
#define CYBSP_USER_LED2_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_5_pin_8_ALT)
#ifndef ioss_0_port_5_pin_8_HWO
    #define ioss_0_port_5_pin_8_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_USER_LED2_HWO ioss_0_port_5_pin_8_HWO
#define CYBSP_USER_LED_ENABLED 1U
#define CYBSP_USER_LED1_ENABLED CYBSP_USER_LED_ENABLED
#define CYBSP_USER_LED_PORT XMC_GPIO_PORT5
#define CYBSP_USER_LED1_PORT CYBSP_USER_LED_PORT
#define CYBSP_USER_LED_PORT_NUM 5U
#define CYBSP_USER_LED1_PORT_NUM CYBSP_USER_LED_PORT_NUM
#define CYBSP_USER_LED_PIN 9U
#define CYBSP_USER_LED1_PIN CYBSP_USER_LED_PIN
#ifndef ioss_0_port_5_pin_9_ALT
    #define ioss_0_port_5_pin_9_ALT 0U
#endif
#define CYBSP_USER_LED_MODE (XMC_GPIO_MODE_OUTPUT_PUSH_PULL | ioss_0_port_5_pin_9_ALT)
#define CYBSP_USER_LED1_MODE CYBSP_USER_LED_MODE
#ifndef ioss_0_port_5_pin_9_HWO
    #define ioss_0_port_5_pin_9_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_USER_LED_HWO ioss_0_port_5_pin_9_HWO
#define CYBSP_USER_LED1_HWO CYBSP_USER_LED_HWO
#define CYBSP_A0_PORT XMC_GPIO_PORT14
#define CYBSP_A0_PORT_NUM 14U
#define CYBSP_A0_PIN 0U
#define CYBSP_A1_PORT XMC_GPIO_PORT14
#define CYBSP_A1_PORT_NUM 14U
#define CYBSP_A1_PIN 1U
#define CYBSP_A2_PORT XMC_GPIO_PORT14
#define CYBSP_A2_PORT_NUM 14U
#define CYBSP_A2_PIN 2U
#define CYBSP_A3_PORT XMC_GPIO_PORT14
#define CYBSP_A3_PORT_NUM 14U
#define CYBSP_A3_PIN 3U
#define CYBSP_A4_PORT XMC_GPIO_PORT14
#define CYBSP_A4_PORT_NUM 14U
#define CYBSP_A4_PIN 4U
#define CYBSP_A5_PORT XMC_GPIO_PORT14
#define CYBSP_A5_PORT_NUM 14U
#define CYBSP_A5_PIN 5U
#define CYBSP_USER_BTN2_ENABLED 1U
#define CYBSP_USER_BTN2_PORT XMC_GPIO_PORT15
#define CYBSP_USER_BTN2_PORT_NUM 15U
#define CYBSP_USER_BTN2_PIN 12U
#ifndef ioss_0_port_15_pin_12_INPUT
    #define ioss_0_port_15_pin_12_INPUT 0U
#endif
#define CYBSP_USER_BTN2_MODE (XMC_GPIO_MODE_INPUT_TRISTATE | ioss_0_port_15_pin_12_INPUT)
#ifndef ioss_0_port_15_pin_12_HWO
    #define ioss_0_port_15_pin_12_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_USER_BTN2_HWO ioss_0_port_15_pin_12_HWO
#define CYBSP_USER_BTN_ENABLED 1U
#define CYBSP_USER_BTN1_ENABLED CYBSP_USER_BTN_ENABLED
#define CYBSP_USER_BTN_PORT XMC_GPIO_PORT15
#define CYBSP_USER_BTN1_PORT CYBSP_USER_BTN_PORT
#define CYBSP_USER_BTN_PORT_NUM 15U
#define CYBSP_USER_BTN1_PORT_NUM CYBSP_USER_BTN_PORT_NUM
#define CYBSP_USER_BTN_PIN 13U
#define CYBSP_USER_BTN1_PIN CYBSP_USER_BTN_PIN
#ifndef ioss_0_port_15_pin_13_INPUT
    #define ioss_0_port_15_pin_13_INPUT 0U
#endif
#define CYBSP_USER_BTN_MODE (XMC_GPIO_MODE_INPUT_TRISTATE | ioss_0_port_15_pin_13_INPUT)
#define CYBSP_USER_BTN1_MODE CYBSP_USER_BTN_MODE
#ifndef ioss_0_port_15_pin_13_HWO
    #define ioss_0_port_15_pin_13_HWO XMC_GPIO_HWCTRL_DISABLED
#endif
#define CYBSP_USER_BTN_HWO ioss_0_port_15_pin_13_HWO
#define CYBSP_USER_BTN1_HWO CYBSP_USER_BTN_HWO

extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED_ERR_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED_RUN_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED2_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED4_config;
extern const XMC_GPIO_CONFIG_t CYBSP_DEBUG_UART_RX_config;
extern const XMC_GPIO_CONFIG_t CYBSP_DEBUG_UART_TX_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED5_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED1_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED8_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED6_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED3_config;
extern const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED7_config;
extern const XMC_GPIO_CONFIG_t CYBSP_USER_LED2_config;
extern const XMC_GPIO_CONFIG_t CYBSP_USER_LED_config;

#define CYBSP_USER_LED1_config CYBSP_USER_LED_config

extern const XMC_GPIO_CONFIG_t CYBSP_USER_BTN2_config;
extern const XMC_GPIO_CONFIG_t CYBSP_USER_BTN_config;

#define CYBSP_USER_BTN1_config CYBSP_USER_BTN_config

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_PINS_H */
