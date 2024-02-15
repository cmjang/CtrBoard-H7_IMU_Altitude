/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2023 Arm Limited (or its affiliates).
 * All rights reserved.
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
 * Project:      MCI Driver Definitions for ST STM32H7xx
 * -------------------------------------------------------------------------- */

#ifndef __MCI_STM32H7XX_H
#define __MCI_STM32H7XX_H

#include "Driver_MCI.h"
#include "stm32h7xx_hal.h"

#include "RTE_Components.h"
#include "MX_Device.h"

#include <string.h>

#if ((defined(RTE_Drivers_MCI0) || \
      defined(RTE_Drivers_MCI1))   \
      && (!defined (MX_SDMMC1))    \
      && (!defined (MX_SDMMC2)))
  #error "SDMMC not configured in STM32CubeMX!"
#endif

#define EXPAND_SYMBOL(pin, ext)         MX_##pin##_##ext
#define MX_SYM(pin, ext)                EXPAND_SYMBOL(pin, ext)

#if defined(MX_MemoryCard_CD0)
  #define MX_MemoryCard_CD0_GPIOx      MX_SYM(MX_MemoryCard_CD0, GPIOx)
  #define MX_MemoryCard_CD0_GPIO_Pin   MX_SYM(MX_MemoryCard_CD0, GPIO_Pin)
  #define MX_MemoryCard_CD0_GPIO_PuPd  MX_SYM(MX_MemoryCard_CD0, GPIO_PuPd)
  #define MX_MemoryCard_CD0_GPIO_Mode  MX_SYM(MX_MemoryCard_CD0, GPIO_Mode)
#endif

#if defined(MX_MemoryCard_CD1)
  #define MX_MemoryCard_CD1_GPIOx      MX_SYM(MX_MemoryCard_CD1, GPIOx)
  #define MX_MemoryCard_CD1_GPIO_Pin   MX_SYM(MX_MemoryCard_CD1, GPIO_Pin)
  #define MX_MemoryCard_CD1_GPIO_PuPd  MX_SYM(MX_MemoryCard_CD1, GPIO_PuPd)
  #define MX_MemoryCard_CD1_GPIO_Mode  MX_SYM(MX_MemoryCard_CD1, GPIO_Mode)
#endif

#if defined(MX_MemoryCard_WP0)
  #define MX_MemoryCard_WP0_GPIOx      MX_SYM(MX_MemoryCard_WP0, GPIOx)
  #define MX_MemoryCard_WP0_GPIO_Pin   MX_SYM(MX_MemoryCard_WP0, GPIO_Pin)
  #define MX_MemoryCard_WP0_GPIO_PuPd  MX_SYM(MX_MemoryCard_WP0, GPIO_PuPd)
  #define MX_MemoryCard_WP0_GPIO_Mode  MX_SYM(MX_MemoryCard_WP0, GPIO_Mode)
#endif

#if defined(MX_MemoryCard_WP1)
  #define MX_MemoryCard_WP1_GPIOx      MX_SYM(MX_MemoryCard_WP1, GPIOx)
  #define MX_MemoryCard_WP1_GPIO_Pin   MX_SYM(MX_MemoryCard_WP1, GPIO_Pin)
  #define MX_MemoryCard_WP1_GPIO_PuPd  MX_SYM(MX_MemoryCard_WP1, GPIO_PuPd)
  #define MX_MemoryCard_WP1_GPIO_Mode  MX_SYM(MX_MemoryCard_WP1, GPIO_Mode)
#endif

/* MCI0: Define 4-bit data bus width */
#if defined(MX_SDMMC1_D0_Pin) && defined(MX_SDMMC1_D1_Pin) && defined(MX_SDMMC1_D2_Pin) && defined(MX_SDMMC1_D3_Pin)
  #define MCI0_BUS_WIDTH_4   1U
#else
  #define MCI0_BUS_WIDTH_4   0U
#endif

/* MCI1: Define 4-bit data bus width */
#if defined(MX_SDMMC2_D0_Pin) && defined(MX_SDMMC2_D1_Pin) && defined(MX_SDMMC2_D2_Pin) && defined(MX_SDMMC2_D3_Pin)
  #define MCI1_BUS_WIDTH_4   1U
#else
  #define MCI1_BUS_WIDTH_4   0U
#endif

/* MCI0: Define 8-bit data bus width */
#if defined(MX_SDMMC1_D0_Pin) && defined(MX_SDMMC1_D1_Pin) && defined(MX_SDMMC1_D2_Pin) && defined(MX_SDMMC1_D3_Pin) && \
    defined(MX_SDMMC1_D4_Pin) && defined(MX_SDMMC1_D5_Pin) && defined(MX_SDMMC1_D6_Pin) && defined(MX_SDMMC1_D7_Pin)
  #define MCI0_BUS_WIDTH_8   1U
#else
  #define MCI0_BUS_WIDTH_8   0U
#endif

/* MCI1: Define 8-bit data bus width */
#if defined(MX_SDMMC2_D0_Pin) && defined(MX_SDMMC2_D1_Pin) && defined(MX_SDMMC2_D2_Pin) && defined(MX_SDMMC2_D3_Pin) && \
    defined(MX_SDMMC2_D4_Pin) && defined(MX_SDMMC2_D5_Pin) && defined(MX_SDMMC2_D6_Pin) && defined(MX_SDMMC2_D7_Pin)
  #define MCI1_BUS_WIDTH_8   1U
#else
  #define MCI1_BUS_WIDTH_8   0U
#endif

/* MCI0: Define Card Detect pin existence */
#if defined(MX_MemoryCard_CD0_Pin)
  #define MCI0_CD_PIN        1U
#else
  #define MCI0_CD_PIN        0U
#endif

/* MCI1: Define Card Detect pin existence */
#if defined(MX_MemoryCard_CD1_Pin)
  #define MCI1_CD_PIN        1U
#else
  #define MCI1_CD_PIN        0U
#endif

/* MCI0: Define Write Protect pin existence */
#if defined(MX_MemoryCard_WP0_Pin)
  #define MCI0_WP_PIN        1U
#else
  #define MCI0_WP_PIN        0U
#endif

/* MCI1: Define Write Protect pin existence */
#if defined(MX_MemoryCard_WP1_Pin)
  #define MCI1_WP_PIN        1U
#else
  #define MCI1_WP_PIN        0U
#endif


/* Interrupt clear mask */
#define SDMMC_ICR_BIT_Msk     (SDMMC_IT_CCRCFAIL   | \
                               SDMMC_IT_DCRCFAIL   | \
                               SDMMC_IT_CTIMEOUT   | \
                               SDMMC_IT_DTIMEOUT   | \
                               SDMMC_IT_TXUNDERR   | \
                               SDMMC_IT_RXOVERR    | \
                               SDMMC_IT_CMDREND    | \
                               SDMMC_IT_CMDSENT    | \
                               SDMMC_IT_DATAEND    | \
                               SDMMC_IT_DHOLD      | \
                               SDMMC_IT_DBCKEND    | \
                               SDMMC_IT_DABORT     | \
                               SDMMC_IT_TXFIFOHE   | \
                               SDMMC_IT_RXFIFOHF   | \
                               SDMMC_IT_RXFIFOF    | \
                               SDMMC_IT_TXFIFOE    | \
                               SDMMC_IT_BUSYD0END  | \
                               SDMMC_IT_SDIOIT     | \
                               SDMMC_IT_ACKFAIL    | \
                               SDMMC_IT_ACKTIMEOUT | \
                               SDMMC_IT_VSWEND     | \
                               SDMMC_IT_CKSTOP     | \
                               SDMMC_IT_IDMABTC)

/* Error interrupt mask */
#define SDMMC_STA_ERR_BIT_Msk (SDMMC_STA_CCRCFAIL   | \
                               SDMMC_STA_DCRCFAIL   | \
                               SDMMC_STA_CTIMEOUT   | \
                               SDMMC_STA_DTIMEOUT   | \
                               SDMMC_STA_ACKFAIL    | \
                               SDMMC_STA_ACKTIMEOUT | \
                               SDMMC_STA_IDMATE)

/* Driver flag definitions */
#define MCI_INIT      ((uint32_t)0x0001)  /* MCI initialized               */
#define MCI_POWER     ((uint32_t)0x0002)  /* MCI powered on                */
#define MCI_SETUP     ((uint32_t)0x0004)  /* MCI configured                */
#define MCI_RESP_LONG ((uint32_t)0x0008)  /* Long response expected        */
#define MCI_RESP_CRC  ((uint32_t)0x0010)  /* Check response CRC error      */
#define MCI_DATA_XFER ((uint32_t)0x0020)  /* Transfer data                 */
#define MCI_DATA_READ ((uint32_t)0x0040)  /* Read transfer                 */
#define MCI_READ_WAIT ((uint32_t)0x0080)  /* Read wait operation start     */
#define MCI_BUS_HS    ((uint32_t)0x0100)  /* High speed bus mode is active */
#define MCI_BUS_SDR12 ((uint32_t)0x0200)  /* SDR12 bus mode is active      */

#define MCI_RESPONSE_EXPECTED_Msk (ARM_MCI_RESPONSE_SHORT      | \
                                   ARM_MCI_RESPONSE_SHORT_BUSY | \
                                   ARM_MCI_RESPONSE_LONG)

/* MCI Input/Output Definition */
typedef const struct {
  GPIO_TypeDef         *port;           /* IO port                            */
  uint16_t              pin;            /* IO pin                             */
  uint16_t              reserved;       /* Reserved                           */
  uint32_t              pull;           /* IO pin pull resistor               */
  uint32_t              af;             /* IO alternate function              */
} MCI_IO;

/* MCI Driver State Definition */
typedef struct {
  ARM_MCI_SignalEvent_t     cb_event;   /* Driver event callback function     */
  ARM_MCI_STATUS volatile   status;     /* Driver status                      */
  uint32_t                 *response;   /* Pointer to response buffer         */
  uint32_t                  dctrl;      /* Data control register value        */
  uint32_t                  dlen;       /* Data length register value         */
  uint32_t                  ker_clk;    /* SDMMC kernel clock frequency       */
  uint32_t volatile         flags;      /* Driver state flags                 */
} MCI_INFO;

/* MCI Resources Definition */
typedef struct {
  void          *h;                     /* SD_HandleTypeDef/MMC_HandleTypeDef */
  SDMMC_TypeDef *reg;                   /* SDMMC peripheral register interface*/
  MCI_IO        *io_cd;                 /* I/O config: card detect            */
  MCI_IO        *io_wp;                 /* I/O config: write protect line     */
  MCI_INFO      *info;                  /* Run-Time information               */
} const MCI_RESOURCES;

/* Global functions and variables */
#ifdef MX_SDMMC1
extern void SDMMC1_IRQHandler        (void);
extern void MX_SDMMC1_RX_DMA_Handler (void);
extern void MX_SDMMC1_TX_DMA_Handler (void);

extern ARM_DRIVER_MCI Driver_MCI0;
#endif

#ifdef MX_SDMMC2
extern void SDMMC2_IRQHandler        (void);
extern void MX_SDMMC2_RX_DMA_Handler (void);
extern void MX_SDMMC2_TX_DMA_Handler (void);

extern ARM_DRIVER_MCI Driver_MCI1;
#endif

extern int32_t MCI_ReadCD (uint32_t instance);
extern int32_t MCI_ReadWP (uint32_t instance);

#endif /* __MCI_STM32H7XX_H */
