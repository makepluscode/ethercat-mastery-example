/*******************************************************************************
 * File Name: cycfg_pins.c
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

#include "cycfg_pins.h"

const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED_ERR_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED_ERR_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED_RUN_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED_RUN_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED2_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED2_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED4_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED4_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_DEBUG_UART_RX_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_DEBUG_UART_RX_MODE,
};
const XMC_GPIO_CONFIG_t CYBSP_DEBUG_UART_TX_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_DEBUG_UART_TX_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED5_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED5_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED1_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED1_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED8_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED8_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED6_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED6_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED3_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED3_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_ECAT_LED7_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_ECAT_LED7_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE,
};
const XMC_GPIO_CONFIG_t CYBSP_USER_LED2_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_USER_LED2_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};
const XMC_GPIO_CONFIG_t CYBSP_USER_LED_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_USER_LED_MODE,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};
const XMC_GPIO_CONFIG_t CYBSP_USER_BTN2_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_USER_BTN2_MODE,
};
const XMC_GPIO_CONFIG_t CYBSP_USER_BTN_config =
{
    .mode = (XMC_GPIO_MODE_t)CYBSP_USER_BTN_MODE,
};

void init_cycfg_pins(void)
{
    XMC_GPIO_Init(CYBSP_ECAT_LED_ERR_PORT, CYBSP_ECAT_LED_ERR_PIN, &CYBSP_ECAT_LED_ERR_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED_ERR_PORT, CYBSP_ECAT_LED_ERR_PIN, CYBSP_ECAT_LED_ERR_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED_RUN_PORT, CYBSP_ECAT_LED_RUN_PIN, &CYBSP_ECAT_LED_RUN_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED_RUN_PORT, CYBSP_ECAT_LED_RUN_PIN, CYBSP_ECAT_LED_RUN_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED2_PORT, CYBSP_ECAT_LED2_PIN, &CYBSP_ECAT_LED2_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED2_PORT, CYBSP_ECAT_LED2_PIN, CYBSP_ECAT_LED2_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED4_PORT, CYBSP_ECAT_LED4_PIN, &CYBSP_ECAT_LED4_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED4_PORT, CYBSP_ECAT_LED4_PIN, CYBSP_ECAT_LED4_HWO);
    XMC_GPIO_Init(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_PIN, &CYBSP_DEBUG_UART_RX_config);
    XMC_GPIO_SetHardwareControl(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_PIN, CYBSP_DEBUG_UART_RX_HWO);
    XMC_GPIO_Init(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_PIN, &CYBSP_DEBUG_UART_TX_config);
    XMC_GPIO_SetHardwareControl(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_PIN, CYBSP_DEBUG_UART_TX_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED5_PORT, CYBSP_ECAT_LED5_PIN, &CYBSP_ECAT_LED5_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED5_PORT, CYBSP_ECAT_LED5_PIN, CYBSP_ECAT_LED5_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED1_PORT, CYBSP_ECAT_LED1_PIN, &CYBSP_ECAT_LED1_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED1_PORT, CYBSP_ECAT_LED1_PIN, CYBSP_ECAT_LED1_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED8_PORT, CYBSP_ECAT_LED8_PIN, &CYBSP_ECAT_LED8_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED8_PORT, CYBSP_ECAT_LED8_PIN, CYBSP_ECAT_LED8_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED6_PORT, CYBSP_ECAT_LED6_PIN, &CYBSP_ECAT_LED6_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED6_PORT, CYBSP_ECAT_LED6_PIN, CYBSP_ECAT_LED6_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED3_PORT, CYBSP_ECAT_LED3_PIN, &CYBSP_ECAT_LED3_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED3_PORT, CYBSP_ECAT_LED3_PIN, CYBSP_ECAT_LED3_HWO);
    XMC_GPIO_Init(CYBSP_ECAT_LED7_PORT, CYBSP_ECAT_LED7_PIN, &CYBSP_ECAT_LED7_config);
    XMC_GPIO_SetHardwareControl(CYBSP_ECAT_LED7_PORT, CYBSP_ECAT_LED7_PIN, CYBSP_ECAT_LED7_HWO);
    XMC_GPIO_Init(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, &CYBSP_USER_LED2_config);
    XMC_GPIO_SetHardwareControl(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, CYBSP_USER_LED2_HWO);
    XMC_GPIO_Init(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN, &CYBSP_USER_LED_config);
    XMC_GPIO_SetHardwareControl(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN, CYBSP_USER_LED_HWO);
    XMC_GPIO_Init(CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN, &CYBSP_USER_BTN2_config);
    XMC_GPIO_SetHardwareControl(CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN, CYBSP_USER_BTN2_HWO);
    XMC_GPIO_Init(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_PIN, &CYBSP_USER_BTN_config);
    XMC_GPIO_SetHardwareControl(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_PIN, CYBSP_USER_BTN_HWO);
}
