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
 * $Date:        27. February 2023
 * $Revision:    V1.6
 *
 * Driver:       Driver_ETH_MAC0
 *
 * Configured:   via CubeMX
 * Project:      Ethernet Media Access (MAC) Driver for STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_emac Ethernet MAC

# Revision History

- Version 1.6
  - Updated receive functions for HAL zero-copy concept
  - Corrected multicast address filtering
- Version 1.5
  - Address of a Tx_Buff is specified with a section in a linker scatter file (for Arm Compiler 6)
- Version 1.4
  - Removed Data Cache maintenance (ETH DMA descriptors and frame buffers must be positioned in Non-cacheable memory).
- Version 1.3
  - Added data cache enable bit check
- Version 1.2
  - Updated GetRxFrameSize and ReadFrame functions to correctly use HAL
- Version 1.1
  - Updated __MEMORY_AT macro
- Version 1.0
  - Initial release

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_ETH_MAC0 | EMAC

# Limitations

Descriptor and data buffers location:
 - Memory for ETH DMA Descriptors (DMARxDscrTab and DMATxDscrTab) must be configured as:
   non-cacheable, non-shareable device memory, execute never
 - Memory for ETH data buffers (Rx_Buff and Tx_Buff) must be configured as:
   non-cacheable, non-shareable normal memory, execute never

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                 | Default value | Value | Description
:--------------------------|:-------------:|:-----:|:-----------
EMAC_CHECKSUM_OFFLOAD      |     **1**     |   1   | Receive/transmit Checksum offload: **enabled**
^                          |       ^       |   0   | Receive/transmit Checksum offload: **disabled**
EMAC_TIME_STAMP            |     **0**     |   1   | IEEE 1588 time stamping (PTP): **enabled**
^                          |       ^       |   0   | IEEE 1588 time stamping (PTP): **disabled**

## STM32CubeMX

The CMSIS-Driver EMAC requires:
  - AHB1 clock cofigured to 200MHz or lower
  - ETH peripheral configured in MII or RMII mode
  - NVIC configured to enabled Ethernet global interrupt
  - DMA descriptors and data buffers located AXI SRAM (D1) or AHB SRAM (D2 or D3)
  - MPU cofigured to disable CPU DCache when accessing Ethernet DMA descriptors

\note The User Label name is used to connect the CMSIS-Driver to the GPIO pin.

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
ETH        | RMII                         | ETH Controller
PA1        | Alternate Function Push Pull | ETH REF_CLK Pin
PA2        | Alternate Function Push Pull | ETH MDIO Pin
PA7        | Alternate Function Push Pull | ETH CRS_DV Pin
PC1        | Alternate Function Push Pull | ETH MDC Pin
PC4        | Alternate Function Push Pull | ETH RXD0 Pin
PC5        | Alternate Function Push Pull | ETH RXD1 Pin
PG11       | Alternate Function Push Pull | ETH TX_EN Pin
PG13       | Alternate Function Push Pull | ETH TXD0 Pin
PG14       | Alternate Function Push Pull | ETH TXD1 Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PA1      | **ETH_REF_CLK**
        PA2      | **ETH_MDIO**
        PA7      | **ETH_CRS_DV**
        PC1      | **ETH_MDC**
        PC4      | **ETH_RXD0**
        PC5      | **ETH_RXD1**
        PG11     | **ETH_TX_EN**
        PG12     | **ETH_TXD1**
        PG13     | **ETH_TXD0**
       \n

  2. Under **Categories**: **Connectivity** select **ETH**:

     __Mode__:
       - Mode: **RMII**
       - Activate Rx Err signal: unchecked

     __Configuration__:
       - Parameter Settings:
            General: Ethernet Configuration | Value
            :-------------------------------|:-------:
            Ethernet MAC Address            | unused
            Tx Descriptor Length            | **4**
            First Tx Descriptor Address     | **0x30040060**
            Rx Descriptor Length            | **4**
            First Rx Descriptor Address     | **0x30040000**
            Rx Buffers Length               | **1524**
       \n
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - DMA Settings: configured in later step (under Category: System Core: DMA)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PA1      | ETH_REF_CLK   | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PA2      | ETH_MDIO      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PA7      | ETH_CRS_DV    | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PC1      | ETH_MDC       | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PC4      | ETH_RXD0      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PC5      | ETH_RXD1      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PG11     | ETH_TX_EN     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PG12     | ETH_TXD1      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
            PG13     | ETH_TXD0      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       |.
       \n

  3. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
            :---------------------------------|:-----------:|:-------------------:|:------------:
            Ethernet global interrupt         | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            Ethernet global interrupt         | unchecked   | checked             | checked          | checked
       \n


  4. Under **Categories**: **System Core** select **CORTEX_M7**:

     __Configuration__:
       - Parameter Settings:
            Speculation default mode Settings               | Value
            :-----------------------------------------------|:-------------------------:
            Speculation default mode                        | Disabled
       \n
            Cortex Interface Settings                       | Value
            :-----------------------------------------------|:-------------------------:
            CPU ICache                                      | Enabled
            CPU DCache Length                               | Enabled
       \n
            Cortex Memory Protection Unit Control Settings  | Value
            :-----------------------------------------------|:-------------------------:
            MPU Control Mode                                | Background Region Privileged accesses only + MPU disabled during hard fault
       \n
            Cortex Memory Protection Unit Region 0 Settings | Value
            :-----------------------------------------------|:-------------------------:
            MPU Region                                      | **Enabled**
            MPU Region Base Address                         | **0x24000000**
            MPU Region Size                                 | **512kB**
            MPU SubRegion Disable                           | **0x0**
            MPU TEX field level                             | **level 1**
            MPU Access Permission                           | **ALL ACCESS PERMITTED**
            MPU Instruction Access                          | **DISABLE**
            MPU Shareability Permission                     | **DISABLE**
            MPU Cacheable Permission                        | **ENABLE**
            MPU Bufferable Permission                       | **ENABLE**
       \n
            Cortex Memory Protection Unit Region 1 Settings | Value
            :-----------------------------------------------|:-------------------------:
            MPU Region                                      | **Enabled**
            MPU Region Base Address                         | **0x30000000**
            MPU Region Size                                 | **512kB**
            MPU SubRegion Disable                           | **0x0**
            MPU TEX field level                             | **level 1**
            MPU Access Permission                           | **ALL ACCESS PERMITTED**
            MPU Instruction Access                          | **DISABLE**
            MPU Shareability Permission                     | **DISABLE**
            MPU Cacheable Permission                        | **DISABLE**
            MPU Bufferable Permission                       | **DISABLE**
       \n
            Cortex Memory Protection Unit Region 2 Settings | Value
            :-----------------------------------------------|:-------------------------:
            MPU Region                                      | **Enabled**
            MPU Region Base Address                         | **0x30040000**
            MPU Region Size                                 | **256B**
            MPU SubRegion Disable                           | **0x0**
            MPU TEX field level                             | **level 0**
            MPU Access Permission                           | **ALL ACCESS PERMITTED**
            MPU Instruction Access                          | **DISABLE**
            MPU Shareability Permission                     | **DISABLE**
            MPU Cacheable Permission                        | **DISABLE**
            MPU Bufferable Permission                       | **DISABLE**
       \n

### Clock Configuration tab

  1. Configure **To AHB1,2 Peripheral Clocks (MHz)**: **200**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_ETH_Init                 | ETH              | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.

## Source Code

Add **RxDecripSection**, **TxDecripSection**, **RxArraySection** and **TxArraySection** 
sections to the Scatter file if GNU Compiler or Arm Compiler 6 is used.

Example:
~~~
RW_ETH_RX_DESC 0x30040000 0x00000060 {
  *(.RxDecripSection)
}
RW_ETH_TX_DESC 0x30040060 0x00000060 {
  *(.TxDecripSection)
}
RW_ETH_RX_BUF  0x30040100 0x00001800 {
  *(.RxArraySection)
}
RW_ETH_TX_BUF  0x30041900 0x00001800 {
  *(.TxArraySection)
}
~~~
*/

/*! \cond */

/* Receive/transmit Checksum offload enable */
#ifndef EMAC_CHECKSUM_OFFLOAD
#define EMAC_CHECKSUM_OFFLOAD   1
#endif

/* IEEE 1588 time stamping enable (PTP) */
#ifndef EMAC_TIME_STAMP
#define EMAC_TIME_STAMP         0
#endif

#include "EMAC_STM32H7xx.h"

/* Driver version */
#define ARM_ETH_MAC_DRV_VERSION         ARM_DRIVER_VERSION_MAJOR_MINOR(1,6)

/* External HAL variables */
extern ETH_HandleTypeDef    heth;
extern ETH_DMADescTypeDef   DMARxDscrTab[ETH_RX_DESC_CNT];
extern ETH_DMADescTypeDef   DMATxDscrTab[ETH_TX_DESC_CNT];

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_ETH_MAC_API_VERSION,
  ARM_ETH_MAC_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_ETH_MAC_CAPABILITIES DriverCapabilities = {
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_rx_ip4  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_rx_ip6  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_rx_udp  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_rx_tcp  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_rx_icmp */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_tx_ip4  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_tx_ip6  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_tx_udp  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_tx_tcp  */
  (EMAC_CHECKSUM_OFFLOAD != 0) ? 1U : 0U,   /* checksum_offload_tx_icmp */
  (ETH_MII != 0) ?
  ARM_ETH_INTERFACE_MII :
  ARM_ETH_INTERFACE_RMII,                   /* media_interface          */
  0U,                                       /* mac_address              */
  1U,                                       /* event_rx_frame           */
  1U,                                       /* event_tx_frame           */
  1U,                                       /* event_wakeup             */
  0U                                        /* precision_timer          */
#if (defined(ARM_ETH_MAC_API_VERSION) && (ARM_ETH_MAC_API_VERSION >= 0x201U))
, 0U                                        /* reserved bits            */
#endif
};

/* Local variables */
static EMAC_CTRL Emac;
static ETH_MACConfigTypeDef       MAC_Config;
static ETH_MACFilterConfigTypeDef MAC_Filter;
static ETH_TxPacketConfig         TX_Config;

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
__attribute__((at(0x30040100))) static uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffers */
__attribute__((at(0x30041900))) static uint8_t Tx_Buff[ETH_TX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Transmit Buffers */
#elif defined ( __GNUC__ ) /* GNU Compiler */
static uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".RxArraySection"))); /* Ethernet Receive Buffers */
static uint8_t Tx_Buff[ETH_TX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".TxArraySection"))); /* Ethernet Transmit Buffers */
#endif

/**
  \fn          uint32_t crc32_8bit_rev (uint32_t crc32, uint8_t val)
  \brief       Calculate 32-bit CRC (Polynomial: 0x04C11DB7, data bit-reversed).
  \param[in]   crc32  CRC initial value
  \param[in]   val    Input value
  \return      Calculated CRC value
*/
static uint32_t crc32_8bit_rev (uint32_t crc32, uint8_t val) {
  uint32_t n;

  crc32 ^= __RBIT (val);
  for (n = 8; n; n--) {
    if (crc32 & 0x80000000U) {
      crc32 <<= 1;
      crc32  ^= 0x04C11DB7U;
    } else {
      crc32 <<= 1;
    }
  }
  return (crc32);
}

/**
  \fn          uint32_t crc32_data (const uint8_t *data, uint32_t len)
  \brief       Calculate standard 32-bit Ethernet CRC.
  \param[in]   data  Pointer to buffer containing the data
  \param[in]   len   Data length in bytes
  \return      Calculated CRC value
*/
static uint32_t crc32_data (const uint8_t *data, uint32_t len) {
  uint32_t cnt, crc;

  crc = 0xFFFFFFFFU;
  for (cnt = len; cnt; cnt--) {
    crc = crc32_8bit_rev (crc, *data++);
  }
  return (crc ^ 0xFFFFFFFFU);
}

/* Ethernet Driver functions */

/**
  \fn          ARM_DRIVER_VERSION GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_ETH_MAC_CAPABILITIES GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_ETH_MAC_CAPABILITIES
*/
static ARM_ETH_MAC_CAPABILITIES GetCapabilities (void) {
  return DriverCapabilities;
}


/**
  \fn          int32_t Initialize (ARM_ETH_MAC_SignalEvent_t cb_event)
  \brief       Initialize Ethernet MAC Device.
  \param[in]   cb_event  Pointer to \ref ARM_ETH_MAC_SignalEvent
  \return      \ref execution_status
*/
static int32_t Initialize (ARM_ETH_MAC_SignalEvent_t cb_event) {
  static const uint8_t mac_def[6] = { 2, 0, 0, 0, 0, 0 };

  heth.Instance = ETH;
  heth.Init.TxDesc         = DMATxDscrTab;
  heth.Init.RxDesc         = DMARxDscrTab;
  heth.Init.RxBuffLen      = ETH_MAX_PACKET_SIZE;
  heth.Init.MACAddr        = (uint8_t *)(uint32_t)mac_def;
  heth.Init.MediaInterface = (ETH_MII != 0) ? HAL_ETH_MII_MODE : HAL_ETH_RMII_MODE;

  /* Set Tx packet config common parameters */
  memset (&TX_Config, 0 , sizeof(ETH_TxPacketConfig));
  TX_Config.Attributes     = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TX_Config.ChecksumCtrl   = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TX_Config.CRCPadCtrl     = ETH_CRC_PAD_INSERT;

  /* Clear control structure */
  memset (&Emac, 0, sizeof (EMAC_CTRL));
  Emac.h        = &heth;
  Emac.cb_event = cb_event;
  Emac.flags    = EMAC_FLAG_INIT;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t Uninitialize (void)
  \brief       De-initialize Ethernet MAC Device.
  \return      \ref execution_status
*/
static int32_t Uninitialize (void) {

  Emac.flags &= ~EMAC_FLAG_INIT;
  heth.Instance = NULL;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t PowerControl (ARM_POWER_STATE state)
  \brief       Control Ethernet MAC Device Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t PowerControl (ARM_POWER_STATE state) {

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  switch (state) {
    case ARM_POWER_OFF:

      if (heth.Instance != NULL) {
        HAL_ETH_DeInit(&heth);
      }

      Emac.flags &= ~EMAC_FLAG_POWER;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((Emac.flags & EMAC_FLAG_INIT)  == 0U) {
        /* Driver not initialized */
        return ARM_DRIVER_ERROR;
      }
      if ((Emac.flags & EMAC_FLAG_POWER) != 0U) {
        /* Driver already powered */
        break;
      }

      if (HAL_ETH_Init (&heth) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      Emac.tx_buf.len = 0;
      Emac.flags     |= EMAC_FLAG_POWER;
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t GetMacAddress (ARM_ETH_MAC_ADDR *ptr_addr)
  \brief       Get Ethernet MAC Address.
  \param[in]   ptr_addr  Pointer to address
  \return      \ref execution_status
*/
static int32_t GetMacAddress (ARM_ETH_MAC_ADDR *ptr_addr) {
  uint32_t val;

  if (ptr_addr == NULL) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  val = ETH->MACA0HR;
  ptr_addr->b[5] = (uint8_t)(val >>  8);
  ptr_addr->b[4] = (uint8_t)(val);
  val = ETH->MACA0LR;
  ptr_addr->b[3] = (uint8_t)(val >> 24);
  ptr_addr->b[2] = (uint8_t)(val >> 16);
  ptr_addr->b[1] = (uint8_t)(val >>  8);
  ptr_addr->b[0] = (uint8_t)(val);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SetMacAddress (const ARM_ETH_MAC_ADDR *ptr_addr)
  \brief       Set Ethernet MAC Address.
  \param[in]   ptr_addr  Pointer to address
  \return      \ref execution_status
*/
static int32_t SetMacAddress (const ARM_ETH_MAC_ADDR *ptr_addr) {

  if (ptr_addr == NULL) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  /* Set Ethernet MAC Address registers */
  ETH->MACA0HR = ((uint32_t)ptr_addr->b[5] <<  8) |  (uint32_t)ptr_addr->b[4];
  ETH->MACA0LR = ((uint32_t)ptr_addr->b[3] << 24) | ((uint32_t)ptr_addr->b[2] << 16) |
                 ((uint32_t)ptr_addr->b[1] <<  8) |  (uint32_t)ptr_addr->b[0];

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SetAddressFilter (const ARM_ETH_MAC_ADDR *ptr_addr,
                                               uint32_t          num_addr)
  \brief       Configure Address Filter.
  \param[in]   ptr_addr  Pointer to addresses
  \param[in]   num_addr  Number of addresses to configure
  \return      \ref execution_status
*/
static int32_t SetAddressFilter (const ARM_ETH_MAC_ADDR *ptr_addr, uint32_t num_addr) {
  uint32_t crc;

  if ((ptr_addr == NULL) && (num_addr != 0)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  /* Use unicast address filtering for first 3 MAC addresses */
  ETH->MACPFR &= ~(ETH_MACPFR_HPF | ETH_MACPFR_HMC);
  ETH->MACHT0R = 0U; ETH->MACHT1R = 0U;

  if (num_addr == 0U) {
    ETH->MACA1HR = 0U; ETH->MACA1LR = 0U;
    ETH->MACA2HR = 0U; ETH->MACA2LR = 0U;
    ETH->MACA3HR = 0U; ETH->MACA3LR = 0U;
    return ARM_DRIVER_OK;
  }

  ETH->MACA1HR = ((uint32_t)ptr_addr->b[5] <<  8) |  (uint32_t)ptr_addr->b[4] | ETH_MACAHR_AE;
  ETH->MACA1LR = ((uint32_t)ptr_addr->b[3] << 24) | ((uint32_t)ptr_addr->b[2] << 16) |
                 ((uint32_t)ptr_addr->b[1] <<  8) |  (uint32_t)ptr_addr->b[0];
  num_addr--;
  if (num_addr == 0U) {
    ETH->MACA2HR = 0U; ETH->MACA2LR = 0U;
    ETH->MACA3HR = 0U; ETH->MACA3LR = 0U;
    return ARM_DRIVER_OK;
  }
  ptr_addr++;

  ETH->MACA2HR = ((uint32_t)ptr_addr->b[5] <<  8) |  (uint32_t)ptr_addr->b[4] | ETH_MACAHR_AE;
  ETH->MACA2LR = ((uint32_t)ptr_addr->b[3] << 24) | ((uint32_t)ptr_addr->b[2] << 16) |
                 ((uint32_t)ptr_addr->b[1] <<  8) |  (uint32_t)ptr_addr->b[0];
  num_addr--;
  if (num_addr == 0U) {
    ETH->MACA3HR = 0U; ETH->MACA3LR = 0U;
    return ARM_DRIVER_OK;
  }
  ptr_addr++;

  ETH->MACA3HR = ((uint32_t)ptr_addr->b[5] <<  8) |  (uint32_t)ptr_addr->b[4] | ETH_MACAHR_AE;
  ETH->MACA3LR = ((uint32_t)ptr_addr->b[3] << 24) | ((uint32_t)ptr_addr->b[2] << 16) |
                 ((uint32_t)ptr_addr->b[1] <<  8) |  (uint32_t)ptr_addr->b[0];
  num_addr--;
  if (num_addr == 0U) {
    return ARM_DRIVER_OK;
  }
  ptr_addr++;

  /* Calculate 64-bit Hash table for remaining MAC addresses */
  for ( ; num_addr; ptr_addr++, num_addr--) {
    crc = crc32_data (&ptr_addr->b[0], 6U) >> 26;
    if (crc & 0x20U) {
      ETH->MACHT1R |= (1U << (crc & 0x1FU));
    }
    else {
      ETH->MACHT0R |= (1U << crc);
    }
  }
  /* Enable both, unicast and hash address filtering */
  ETH->MACPFR |= ETH_MACPFR_HPF | ETH_MACPFR_HMC;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SendFrame (const uint8_t *frame, uint32_t len, uint32_t flags)
  \brief       Send Ethernet frame.
  \param[in]   frame  Pointer to frame buffer with data to send
  \param[in]   len    Frame buffer length in bytes
  \param[in]   flags  Frame transmit flags (see ARM_ETH_MAC_TX_FRAME_...)
  \return      \ref execution_status
*/
static int32_t SendFrame (const uint8_t *frame, uint32_t len, uint32_t flags) {
  ETH_DMADescTypeDef *tx_desc;
  uint32_t tx_index;

  if ((frame == NULL) || (len == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if (Emac.tx_buf.len == 0) {
    /* Start of a new transmit frame */
    tx_index = heth.TxDescList.CurTxDesc;
    tx_desc  = (ETH_DMADescTypeDef *)heth.TxDescList.TxDesc[tx_index];
    if (tx_desc->DESC3 & ETH_DMATXNDESCWBF_OWN) {
      /* Transmitter is busy, wait */
      return ARM_DRIVER_ERROR_BUSY;
    }
    Emac.tx_buf.buffer = Tx_Buff[tx_index];
    Emac.tx_buf.next   = NULL;
  }
  /* Copy data fragments to ETH-DMA buffer */
  memcpy (Emac.tx_buf.buffer + Emac.tx_buf.len, frame, len);
  Emac.tx_buf.len += len;

  if (flags & ARM_ETH_MAC_TX_FRAME_FRAGMENT) {
    /* More data to come, remember current write position */
    return ARM_DRIVER_OK;
  }

  /* Last fragment, send the packet now */
  TX_Config.TxBuffer = &Emac.tx_buf;
  TX_Config.Length   =  Emac.tx_buf.len;

  HAL_ETH_Transmit_IT (&heth, &TX_Config);

  Emac.tx_buf.len = 0;
  return ARM_DRIVER_OK;
}

/**
  \brief       Rx Allocate callback.
  \param[in]   buff  pointer to allocated buffer
  \return      None
*/
void HAL_ETH_RxAllocateCallback(uint8_t **buff) {
  /* Allocate one of the RX-DMA buffers sequentially */
  *buff = &Rx_Buff[Emac.alloc_idx][ETH_MAX_PACKET_SIZE];
  if (++Emac.alloc_idx >= ETH_RX_DESC_CNT) {
    Emac.alloc_idx = 0;
  }
}

/**
  \brief       Rx Link callback.
  \param[in]   pStart  pointer to packet start
  \param[in]   pStart  pointer to packet end
  \param[in]   buff    pointer to received data
  \param[in]   Length  received data length
  \return      None
*/
void HAL_ETH_RxLinkCallback(void **pStart, void **pEnd, uint8_t *buff, uint16_t Length) {
  (void)pStart;
  (void)pEnd;

  Emac.rx_buf.buffer = buff;
  Emac.rx_buf.len    = Length;
}

/**
  \fn          int32_t ReadFrame (uint8_t *frame, uint32_t len)
  \brief       Read data of received Ethernet frame.
  \param[in]   frame  Pointer to frame buffer for data to read into
  \param[in]   len    Frame buffer length in bytes
  \return      number of data bytes read or execution status
                 - value >= 0: number of data bytes read
                 - value < 0: error occurred, value is execution status as defined with \ref execution_status
*/
static int32_t ReadFrame (uint8_t *frame, uint32_t len) {

  if ((frame == NULL) && (len != 0U)) {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    /* Driver not yet powered */
    return ARM_DRIVER_ERROR;
  }

  if ((frame != NULL) && (Emac.rx_buf.buffer != NULL)) {
    memcpy (frame, Emac.rx_buf.buffer, len);
    Emac.rx_buf.buffer = NULL;
  }
  return ((int32_t)len);
}

/**
  \fn          uint32_t GetRxFrameSize (void)
  \brief       Get size of received Ethernet frame.
  \return      number of bytes in received frame
*/
static uint32_t GetRxFrameSize (void) {
  void *dummy;

  if (HAL_ETH_ReadData (Emac.h, &dummy) == HAL_OK) {
    /* Length returned in a Link callback function */
    return ((volatile uint32_t)Emac.rx_buf.len);
  }
  /* No data available */
  return (0);
}

/**
  \fn          int32_t GetRxFrameTime (ARM_ETH_MAC_TIME *time)
  \brief       Get time of received Ethernet frame.
  \param[in]   time  Pointer to time structure for data to read into
  \return      \ref execution_status
*/
static int32_t GetRxFrameTime (ARM_ETH_MAC_TIME *time) {
  (void)time;
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t GetTxFrameTime (ARM_ETH_MAC_TIME *time)
  \brief       Get time of transmitted Ethernet frame.
  \param[in]   time  Pointer to time structure for data to read into
  \return      \ref execution_status
*/
static int32_t GetTxFrameTime (ARM_ETH_MAC_TIME *time) {
  (void)time;
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t ControlTimer (uint32_t control, ARM_ETH_MAC_TIME *time)
  \brief       Control Precision Timer.
  \param[in]   control  operation
  \param[in]   time     Pointer to time structure
  \return      \ref execution_status
*/
static int32_t ControlTimer (uint32_t control, ARM_ETH_MAC_TIME *time) {
  (void)control;
  (void)time;
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t Control (uint32_t control, uint32_t arg)
  \brief       Control Ethernet Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \return      \ref execution_status
*/
static int32_t Control (uint32_t control, uint32_t arg) {

  if ((Emac.flags & EMAC_FLAG_POWER) == 0U) {
    return ARM_DRIVER_ERROR;
  }
  if ((control != ARM_ETH_MAC_CONFIGURE)   &&
      (control != ARM_ETH_MAC_CONTROL_TX)  &&
      (control != ARM_ETH_MAC_CONTROL_RX)  &&
      (control != ARM_ETH_MAC_FLUSH)       &&
      (control != ARM_ETH_MAC_SLEEP)       &&
      (control != ARM_ETH_MAC_VLAN_FILTER)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (control) {
    case ARM_ETH_MAC_CONFIGURE:
      /* Read device configuration first */
      HAL_ETH_GetMACConfig       (Emac.h, &MAC_Config);
      HAL_ETH_GetMACFilterConfig (Emac.h, &MAC_Filter);

      /* Configure 100MBit/10MBit mode */
      switch (arg & ARM_ETH_MAC_SPEED_Msk) {
        case ARM_ETH_MAC_SPEED_10M:
          MAC_Config.Speed = ETH_SPEED_10M;
          break;
        case ARM_ETH_SPEED_100M:
          MAC_Config.Speed = ETH_SPEED_100M;
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      /* Confige Half/Full duplex mode */
      switch (arg & ARM_ETH_MAC_DUPLEX_Msk) {
        case ARM_ETH_MAC_DUPLEX_FULL:
          MAC_Config.DuplexMode = ETH_FULLDUPLEX_MODE;
          break;
        case ARM_ETH_MAC_DUPLEX_HALF:
          MAC_Config.DuplexMode = ETH_HALFDUPLEX_MODE;
          break;
        default:
          return ARM_DRIVER_ERROR;
      }

      if ((arg & ARM_ETH_MAC_LOOPBACK) != 0U) {
        /* Enable loopback mode */
        MAC_Config.LoopbackMode = ENABLE;
      } else {
        MAC_Config.LoopbackMode = DISABLE;
      }

#if (EMAC_CHECKSUM_OFFLOAD != 0)
      if ((arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_RX) != 0U) {
        /* Enable rx checksum verification */
        MAC_Config.ChecksumOffload = ENABLE;
      } else {
        MAC_Config.ChecksumOffload = DISABLE;
      }

      /* Enable tx checksum generation */
      if ((arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_TX) != 0U) {
      } else {
      }
#else
      if ((arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_RX) ||
          (arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_TX)) {
        /* Checksum offload is disabled in the driver */
        return ARM_DRIVER_ERROR;
      }
#endif

      if ((arg & ARM_ETH_MAC_ADDRESS_BROADCAST) != 0U) {
        /* Enable broadcast frame receive */
        MAC_Filter.BroadcastFilter = ENABLE;
      } else {
        MAC_Filter.BroadcastFilter = DISABLE;
      }

      if ((arg & ARM_ETH_MAC_ADDRESS_MULTICAST) != 0U) {
        /* Enable all multicast frame receive */
        MAC_Filter.PassAllMulticast = ENABLE;
      } else {
        MAC_Filter.PassAllMulticast = DISABLE;
      }

      if ((arg & ARM_ETH_MAC_ADDRESS_ALL) != 0U) {
        /* Enable promiscuous mode (no filtering) */
        MAC_Filter.PromiscuousMode = ENABLE;
      } else {
        MAC_Filter.PromiscuousMode = DISABLE;
      }
      HAL_ETH_SetMACConfig       (Emac.h, &MAC_Config);
      HAL_ETH_SetMACFilterConfig (Emac.h, &MAC_Filter);
      break;

    case ARM_ETH_MAC_CONTROL_TX:
      if (arg != 0U) {
        /* Enable MAC transmitter */
        // ETH->MACCR   |=  ETH_MACCR_TE;
      }
      else {
        /* Disable MAC transmitter */
        // ETH->MACCR   &= ~ETH_MACCR_TE;
      }
      return ARM_DRIVER_OK;

    case ARM_ETH_MAC_CONTROL_RX:
      if (arg != 0U) {
        /* Enable MAC receiver */
        HAL_ETH_Start_IT (Emac.h);
      } else {
        /* Disable MAC receiver */
        HAL_ETH_Stop_IT (Emac.h);
      }
      return ARM_DRIVER_OK;

    case ARM_ETH_MAC_FLUSH:
      /* Flush tx and rx buffers */
      if ((arg & ARM_ETH_MAC_FLUSH_RX) != 0U) {
        /* Flush the receive buffer */
        // Disable Rx if enabled
        // if (rx_en)
        //   ETH->MACCR &= ~ETH_MACCR_RE;
        //
        // Clear rx descriptors here
        //
        // Enable Rx if was disabled above
        // if (rx_en)
        //   ETH->MACCR |=  ETH_MACCR_RE;
      }

      if (arg & ARM_ETH_MAC_FLUSH_TX) {
        /* Flush the transmit buffer */
        // Disable Tx if enabled
        // if (tx_en)
        //   ETH->MACCR &= ~ETH_MACCR_TE;
        //
        // Clear tx descriptors here
        //
        // Enable Tx if was disabled above
        // if (tx_en)
        //   ETH->MACCR |=  ETH_MACCR_TE;
      }
      break;

    case ARM_ETH_MAC_VLAN_FILTER:
      /* Configure VLAN filter */
      if (arg == 0) {
        /* Disable VLAN filter */
      }
      else {
        /* arg bits [0-15] are VLAN tag value */
        if ((arg & ARM_ETH_MAC_VLAN_FILTER_ID_ONLY) != 0U) {
          /* Compare only the 12-bit VLAN identifier */
          HAL_ETH_SetRxVLANIdentifier (Emac.h, ETH_VLANTAGCOMPARISON_12BIT, (arg & 0xFFFF));
        } else {
          /* Compare the complete 16-bit VLAN tag value */
          HAL_ETH_SetRxVLANIdentifier (Emac.h, ETH_VLANTAGCOMPARISON_16BIT, (arg & 0xFFFF));
        }
      }
      break;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t PHY_Read (uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
  \brief       Read Ethernet PHY Register through Management Interface.
  \param[in]   phy_addr  5-bit device address
  \param[in]   reg_addr  5-bit register address
  \param[out]  data      Pointer where the result is written to
  \return      \ref execution_status
*/
static int32_t PHY_Read (uint8_t phy_addr, uint8_t reg_addr, uint16_t *data) {
  uint32_t val;

  if (HAL_ETH_ReadPHYRegister (Emac.h, phy_addr, reg_addr, &val) != HAL_OK) {
    return ARM_DRIVER_ERROR;
  }
  *data = (uint16_t)val;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t PHY_Write (uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
  \brief       Write Ethernet PHY Register through Management Interface.
  \param[in]   phy_addr  5-bit device address
  \param[in]   reg_addr  5-bit register address
  \param[in]   data      16-bit data to write
  \return      \ref execution_status
*/
static int32_t PHY_Write (uint8_t phy_addr, uint8_t reg_addr, uint16_t data) {

  if (HAL_ETH_WritePHYRegister (Emac.h, phy_addr, reg_addr, data) != HAL_OK) {
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}


/* HAL callback: Tx transfer completed (frame sent) */
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *h_eth) {
  (void)h_eth;
  if (Emac.cb_event != NULL) {
    Emac.cb_event (ARM_ETH_MAC_EVENT_TX_FRAME);
  }
}

/* HAL callback: Rx transfer completed (frame received) */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *h_eth) {
  (void)h_eth;
  if (Emac.cb_event != NULL) {
    Emac.cb_event (ARM_ETH_MAC_EVENT_RX_FRAME);
  }
}

/* HAL callback: Power management callback (Magic/WOL packet received) */
void HAL_ETH_PMTCallback(ETH_HandleTypeDef *h_eth) {
  (void)h_eth;
  if (Emac.cb_event != NULL) {
    Emac.cb_event (ARM_ETH_MAC_EVENT_WAKEUP);
  }
}


/* MAC Driver Control Block */
ARM_DRIVER_ETH_MAC Driver_ETH_MAC0 = {
  GetVersion,
  GetCapabilities,
  Initialize,
  Uninitialize,
  PowerControl,
  GetMacAddress,
  SetMacAddress,
  SetAddressFilter,
  SendFrame,
  ReadFrame,
  GetRxFrameSize,
  GetRxFrameTime,
  GetTxFrameTime,
  ControlTimer,
  Control,
  PHY_Read,
  PHY_Write
};

/*! \endcond */
