/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2023 Arm Limited (or its affiliates). All
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        15. February 2023
 * $Revision:    V1.6
 *
 * Project:      Ethernet Media Access (MAC) Definitions for STM32H7xx
 * -------------------------------------------------------------------------- */

#ifndef __EMAC_STM32F7XX_H
#define __EMAC_STM32F7XX_H

#include <string.h>

#include "cmsis_compiler.h"

#include "Driver_ETH_MAC.h"
#include "stm32h7xx_hal.h"

#include "RTE_Components.h"
#include "MX_Device.h"

#ifndef      __MEMORY_AT
  #if     (defined (__CC_ARM))
    #define  __MEMORY_AT(x)     __attribute__((at(x)))
  #elif   (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    #define  __MEMORY_AT__(x)   __attribute__((section(".bss.ARM.__at_"#x)))
    #define  __MEMORY_AT(x)     __MEMORY_AT__(x)
  #else
    #define  __MEMORY_AT(x)
    #warning Position memory containing __MEMORY_AT macro at absolute address!
  #endif
#endif

/* EMAC Media Interface definition */
#if defined(MX_ETH_TXD2_Pin)   && defined(MX_ETH_TXD3_Pin)   && \
    defined(MX_ETH_RXD2_Pin)   && defined(MX_ETH_RXD3_Pin)   && \
    defined(MX_ETH_TX_CLK_Pin) && defined(MX_ETH_RX_CLK_Pin)
  /* MII pins are defined */
  #define ETH_MII             1
#else
  #define ETH_MII             0
#endif

/* EMAC Driver state flags */
#define EMAC_FLAG_INIT      (1 << 0)    // Driver initialized
#define EMAC_FLAG_POWER     (1 << 1)    // Driver power on

/* EMAC driver control structure */
typedef struct {
  ARM_ETH_MAC_SignalEvent_t cb_event;   // Event callback
  uint8_t       flags;                  // Control and state flags
  uint8_t       alloc_idx;              // Buffer allocation index
  uint16_t      reserved;               // reserved
  ETH_BufferTypeDef tx_buf;             // Transmit buffer pointers
  ETH_BufferTypeDef rx_buf;             // Receive buffer pointers
  ETH_HandleTypeDef *h;                 // Ethernet handle
} EMAC_CTRL;


/* Exported driver */
extern ARM_DRIVER_ETH_MAC Driver_ETH_MAC0;

#endif /* __EMAC_STM32H7XX_H */
