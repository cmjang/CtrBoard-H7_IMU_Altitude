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
 * $Revision:    V2.3
 *
 * Driver:       Driver_USART1/2/3/6/10
 *
 * Configured:   via CubeMX
 * Project:      USART Driver for ST STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_usart_smart USART (SmartCard mode)

# Revision History

- Version 2.3
  - Updated DMA and Data Cache handling (improved code readability and maintainability)
  - Made variable status volatile (solved potential LTO problems)
- Version 2.2
  - Corrected GetStatus function and status flags handling
  - Removed Rx Timeout capability and event signalization
- Version 2.1
  - Corrected GetTxCount/GetRxCount implementation
- Version 2.0
  - Added USART10 interface accessed by Driver_USART10
- Version 1.3
  - Corrected Data Cache handling (checking of data alignment and size is configurable)
- Version 1.2
  - Updated Get Count functionality
- Version 1.1
  - Corrected UARTx/USARTx related typing mistakes
- Version 1.0
  - Initial release

# Capabilities

This driver supports **SmartCard** mode.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_USART1   | USART1
Driver_USART2   | USART2
Driver_USART3   | USART3
Driver_USART6   | USART6
Driver_USART10  | USART10

\note
The **CMSIS Driver:USART (API):USART** component in the **Manage Run-Time Environment** dialog adds multiple C source files to the project.\n
The interface selection in STM32CubeMX selects the actual implementation that is compiled: IrDA, SmartCard, UART or USART.

# Limitations

STM32 HAL limitations:
 - Rx Timeout event disables further reception
 - No callback available for Rx Idle event

DMA usage limitations:
 - The size of DCache line on Cortex M7 is 32 Bytes. To safely perform
   DCache maintenance operations, data must be aligned to a 32 Byte boundary
   and data size must be n*32 Bytes. By default, the alignment and size of
   provided data are checked in SMARTCARD transfer functions. If data is not
   aligned to a 32 Byte boundary or data size is not n*32 Bytes, DMA is
   not used and IRQ transfer is performed.
 - Advanced user can disable data alignment and/or data size checking by
   setting SMARTCARD_DCACHE_DATA_xX_ALIGNMENT and/or SMARTCARD_DCACHE_DATA_xX_SIZE to 0.
   The user must be aware that performing DCache maintenance operations on
   data that is not aligned to a 32 Byte boundary or that is not n*32 Bytes
   in size can be dangerous and can corrupt other data that is also maintained
   by the same 32 Byte DCache line.

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                         | Default value | Value | Description
:----------------------------------|:-------------:|:-----:|:-----------
SMARTCARD_DCACHE_MAINTENANCE       |     **1**     |   1   | SMARTCARD DCache maintenance operations: **enabled**
^                                  |       ^       |   0   | SMARTCARD DCache maintenance operations: **disabled**
SMARTCARD_DCACHE_DATA_RX_ALIGNMENT |     **1**     |   1   | SMARTCARD DCache data Rx alignment check: **enabled**
^                                  |       ^       |   0   | SMARTCARD DCache data Rx alignment check: **disabled**
SMARTCARD_DCACHE_DATA_RX_SIZE      |     **1**     |   1   | SMARTCARD DCache data Rx size check: **enabled**
^                                  |       ^       |   0   | SMARTCARD DCache data Rx size check: **disabled**
SMARTCARD_DCACHE_DATA_TX_ALIGNMENT |     **1**     |   1   | SMARTCARD DCache data Tx alignment check: **enabled**
^                                  |       ^       |   0   | SMARTCARD DCache data Tx alignment check: **disabled**
SMARTCARD_DCACHE_DATA_TX_SIZE      |     **1**     |   1   | SMARTCARD DCache data Tx size check: **enabled**
^                                  |       ^       |   0   | SMARTCARD DCache data Tx size check: **disabled**

## STM32CubeMX

The USART driver requires:
  - **USART peripheral clock**
  - **USART** peripheral configured as **SmartCard with Card Clock**
  - **USART TX** and **CK pins**
  - optional **DMA** configuration for transfers

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
USART1     | SmartCard with Card Clock    | USART1 Controller
PB14       | Alternate Function Push Pull | USART1 TX Pin
PA8        | Alternate Function Push Pull | USART1 CK Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PB14     | **USART1_TX**
        PA8      | **USART1_CK**
       \n

  2. Under **Categories**: **Connectivity** select **USART1**:

     __Mode__:
       - Mode: **SmartCard with Card Clock**
       - Hardware Flow Control (RS232): Disable
       - Hardware Flow Control (RS485): unchecked
       - Slave Select(NSS) Management: Disable

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - DMA Settings: configured in later step (under Category: System Core: DMA)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PB14     | USART1_TX     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
            PA8      | USART1_CK     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
       \n

  3. Under **Categories**: **System Core** select **DMA**:

     __Configuration__:
       - DMA1, DMA2:
       \n
         Click **Add** button and add DMA Requests as in table below:
            DMA Request | Stream         | Direction            | Priority
            :----------:|:--------------:|:---------------------|:--------
            USART1_RX   | DMA1 Stream 0  | Peripheral to Memory | Low
            USART1_TX   | DMA1 Stream 1  | Memory to Peripheral | Low
       \n

  4. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
            :---------------------------------|:-----------:|:-------------------:|:------------:
            DMA1 stream0 global interrupt     | **checked** | 0                   | 0
            DMA1 stream1 global interrupt     | **checked** | 0                   | 0
            USART1 global interrupt           | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            DMA1 stream0 global interrupt     | unchecked   | checked             | checked          | checked
            DMA1 stream1 global interrupt     | unchecked   | checked             | checked          | checked
            USART1 global interrupt           | unchecked   | checked             | checked          | checked

### Clock Configuration tab

  1. Configure **To USART1 (MHz)**: **100**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_USART1_SMARTCARD_Init    | USART1           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "SmartCard_STM32H7xx.h"
#include "DCache_STM32H7xx.h"

#ifdef USARTx_MODE_SMARTCARD

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,3)

#ifndef SMARTCARD_DCACHE_MAINTENANCE
#define SMARTCARD_DCACHE_MAINTENANCE           (1U)
#endif
#ifndef SMARTCARD_DCACHE_DATA_RX_ALIGNMENT
#define SMARTCARD_DCACHE_DATA_RX_ALIGNMENT     (1U)
#endif
#ifndef SMARTCARD_DCACHE_DATA_RX_SIZE
#define SMARTCARD_DCACHE_DATA_RX_SIZE          (1U)
#endif
#ifndef SMARTCARD_DCACHE_DATA_TX_ALIGNMENT
#define SMARTCARD_DCACHE_DATA_TX_ALIGNMENT     (1U)
#endif
#ifndef SMARTCARD_DCACHE_DATA_TX_SIZE
#define SMARTCARD_DCACHE_DATA_TX_SIZE          (1U)
#endif

#define DRIVER_DCACHE_MAINTENANCE              (SMARTCARD_DCACHE_MAINTENANCE)
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((SMARTCARD_DCACHE_DATA_RX_ALIGNMENT == 0U) || (SMARTCARD_DCACHE_DATA_RX_SIZE == 0U))
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (0U)
#endif
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((SMARTCARD_DCACHE_DATA_TX_ALIGNMENT == 0U) || (SMARTCARD_DCACHE_DATA_TX_SIZE == 0U))
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (0U)
#endif

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

static const ARM_USART_CAPABILITIES Capabilities = {
    0,      ///< supports UART (Asynchronous) mode
    0,      ///< supports Synchronous Master mode
    0,      ///< supports Synchronous Slave mode
    0,      ///< supports UART Single-wire mode
    0,      ///< supports UART IrDA mode
    1,      ///< supports UART Smart Card mode
    0,      ///< Smart Card Clock generator available
    0,      ///< RTS Flow Control available
    0,      ///< CTS Flow Control available
    1,      ///< Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,      ///< Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,      ///< RTS Line: 0=not available, 1=available
    0,      ///< CTS Line: 0=not available, 1=available
    0,      ///< DTR Line: 0=not available, 1=available
    0,      ///< DSR Line: 0=not available, 1=available
    0,      ///< DCD Line: 0=not available, 1=available
    0,      ///< RI Line: 0=not available, 1=available
    0,      ///< Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,      ///< Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,      ///< Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0       ///< Signal RI change event: \ref ARM_USART_EVENT_RI
#if (defined(ARM_USART_API_VERSION) && (ARM_USART_API_VERSION >= 0x203U))
  , 0U  // Reserved bits
#endif
  };

// SmartCard1
#ifdef USART1_MODE_SMARTCARD
SMARTCARDx_RESOURCE_ALLOC(1);
#endif

// SmartCard2
#ifdef USART2_MODE_SMARTCARD
SMARTCARDx_RESOURCE_ALLOC(2);
#endif

// SmartCard3
#ifdef USART3_MODE_SMARTCARD
SMARTCARDx_RESOURCE_ALLOC(3);
#endif

// SmartCard6
#ifdef USART6_MODE_SMARTCARD
SMARTCARDx_RESOURCE_ALLOC(6);
#endif

// SmartCard10
#ifdef USART10_MODE_SMARTCARD
SMARTCARDx_RESOURCE_ALLOC(10);
#endif

/**
  \fn          const SMARTCARD_RESOURCES SMARTCARD_Resources (SMARTCARD_HandleTypeDef *hsmartcard)
  \brief       Get UART_RESOURCES structure from SMARTCARD_HandleTypeDef
*/
static const SMARTCARD_RESOURCES * SMARTCARD_Resources (SMARTCARD_HandleTypeDef *hsmartcard) {
  const SMARTCARD_RESOURCES *smartcard = NULL;

#ifdef USART1_MODE_SMARTCARD
  if (hsmartcard->Instance == USART1) { smartcard = &SMARTCARD1_Resources; }
#endif
#ifdef USART2_MODE_SMARTCARD
  if (hsmartcard->Instance == USART2) { smartcard = &SMARTCARD2_Resources; }
#endif
#ifdef USART3_MODE_SMARTCARD
  if (hsmartcard->Instance == USART3) { smartcard = &SMARTCARD3_Resources; }
#endif
#ifdef USART6_MODE_SMARTCARD
  if (hsmartcard->Instance == USART6) { smartcard = &SMARTCARD6_Resources; }
#endif
#ifdef USART10_MODE_SMARTCARD
  if (hsmartcard->Instance == USART10) { smartcard = &SMARTCARD10_Resources; }
#endif

  return smartcard;
}

/**
  \fn          void SMARTCARD_PeripheralReset (USART_TypeDef *usart)
  \brief       SMARTCARD Reset
*/
static void SMARTCARD_PeripheralReset (USART_TypeDef *usart) {

#ifdef USART1_MODE_SMARTCARD
  if (usart == USART1) { __HAL_RCC_USART1_FORCE_RESET(); }
#endif
#ifdef USART2_MODE_SMARTCARD
  if (usart == USART2) { __HAL_RCC_USART2_FORCE_RESET(); }
#endif
#ifdef USART3_MODE_SMARTCARD
  if (usart == USART3) { __HAL_RCC_USART3_FORCE_RESET(); }
#endif
#ifdef USART6_MODE_SMARTCARD
  if (usart == USART6) { __HAL_RCC_USART6_FORCE_RESET(); }
#endif
#ifdef USART10_MODE_SMARTCARD
  if (usart == USART10) { __HAL_RCC_USART10_FORCE_RESET(); }
#endif

      __NOP(); __NOP(); __NOP(); __NOP();

#ifdef USART1_MODE_SMARTCARD
  if (usart == USART1) { __HAL_RCC_USART1_RELEASE_RESET(); }
#endif
#ifdef USART2_MODE_SMARTCARD
  if (usart == USART2) { __HAL_RCC_USART2_RELEASE_RESET(); }
#endif
#ifdef USART3_MODE_SMARTCARD
  if (usart == USART3) { __HAL_RCC_USART3_RELEASE_RESET(); }
#endif
#ifdef USART6_MODE_SMARTCARD
  if (usart == USART6) { __HAL_RCC_USART6_RELEASE_RESET(); }
#endif
#ifdef USART10_MODE_SMARTCARD
  if (usart == USART10) { __HAL_RCC_USART10_RELEASE_RESET(); }
#endif
}

/**
  Calculate number of bytes for provided number of items, for selected DMA.
  \param[in]    hdma   DMA handle
  \param[in]    num    Number of items
  \return       number of bytes
*/
__STATIC_INLINE uint32_t CalcNumOfBytes (DMA_HandleTypeDef *hdma, uint32_t num) {
  uint32_t num_of_bytes = num;

  if (hdma != NULL) {
    if (hdma->Init.MemDataAlignment == DMA_MDATAALIGN_WORD) {
      num_of_bytes = num * 4U;
    } else if (hdma->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD) {
      num_of_bytes = num * 2U;
    }
  }

  return num_of_bytes;
}

/**
  Check if DMA can be used for transmission, and Flush Data Cache if necessary.
  \param[in]    hdma   Pointer to DMA for transmission
  \param[in]    data   Pointer to data for transmission
  \param[in]    num    Number of items for transmission
  \return       result of the determination
                0 = DMA cannot be used for transmission
                1 = DMA can be used for transmission
*/
__STATIC_INLINE uint8_t CheckDmaForTx (DMA_HandleTypeDef *hdma, const void *data, uint32_t num) {
  uint8_t dma_can_be_used = 0U;

  (void)hdma;
  (void)data;
  (void)num;

#if (DRIVER_DCACHE_MAINTENANCE == 0U)           // If Data Cache maintenance is not enabled in the driver
  dma_can_be_used = 1U;                         // can use DMA for Tx
#else                                           // Else if Data Cache maintenance is enabled in the driver
  if (DCache_IsEnabled() == 0U) {               // If Data Cache is not enabled in the MCU
    dma_can_be_used = 1U;                       // can use DMA for Tx
  } else {                                      // Else if Data Cache is enabled in the MCU
    uint32_t num_of_bytes = CalcNumOfBytes(hdma, num);
  #if (DRIVER_DCACHE_TX_USER_GUARANTEED == 1U)  // If the user guarantees that Data Cache operations can be performed on the buffer
    dma_can_be_used = 1U;                       // can use DMA for Tx
    DCache_Flush(data, num_of_bytes);           // Flush the Data Cache to Memory
  #else                                         // Else if the user does not guarantee that Data Cache operations can be performed on the buffer
    if (DCache_IsAligned(data, num_of_bytes) != 0U) {   // If cache operations can be performed on the requested memory area
      dma_can_be_used = 1U;                     // can use DMA for Tx
      DCache_Flush(data, num_of_bytes);         // Flush the Data Cache to Memory
    }
  #endif
  }
#endif

  return dma_can_be_used;
}

/**
  Check if DMA can be used for reception.
  \param[in]    hdma   Pointer to DMA for reception
  \param[in]    data   Pointer to where data should be received
  \param[in]    num    Number of items expected in reception
  \return       result of the determination
                0 = DMA cannot be used for reception
                1 = DMA can be used for reception
                3 = DMA can be used for reception but Data Cache needs to be Refreshed after reception
*/
__STATIC_INLINE uint8_t CheckDmaForRx (DMA_HandleTypeDef *hdma, const void *data, uint32_t num) {
  uint8_t dma_can_be_used = 0U;

  (void)hdma;
  (void)data;
  (void)num;

#if (DRIVER_DCACHE_MAINTENANCE == 0U)           // If Data Cache maintenance is not enabled in the driver
  dma_can_be_used = 1U;                         // can use DMA for Rx
#else                                           // Else if Data Cache maintenance is enabled in the driver
  if (DCache_IsEnabled() == 0U) {               // If Data Cache is not enabled in the MCU
    dma_can_be_used = 1U;                       // can use DMA for Rx
  } else {                                      // Else if Data Cache is enabled in the MCU
  #if (DRIVER_DCACHE_RX_USER_GUARANTEED == 1U)  // If the user guarantees that Data Cache operations can be performed on the buffer
    dma_can_be_used = 3U;                       // can use DMA for Rx but cache needs to be Refreshed after reception
  #else                                         // Else if the user does not guarantee that Data Cache operations can be performed on the buffer
    uint32_t num_of_bytes = CalcNumOfBytes(hdma, num);
    if (DCache_IsAligned(data, num_of_bytes) != 0U) {   // If cache operations can be performed on the requested memory area
      dma_can_be_used = 3U;                     // can use DMA for Rx but cache needs to be Refreshed after reception
    }
  #endif
  }
#endif

  return dma_can_be_used;
}

/**
  Refresh Data Cache after reception.
  \param[in]    hdma   Pointer to DMA used for reception
  \param[in]    data   Pointer to data buffer to Refresh
  \param[in]    num    Number of items to Refresh
*/
__STATIC_INLINE void RefreshDCacheAfterDmaRx (DMA_HandleTypeDef *hdma, void *data, uint32_t num) {

  (void)hdma;
  (void)data;
  (void)num;

#if (DRIVER_DCACHE_MAINTENANCE == 1U)           // If Data Cache maintenance is enabled in the driver
  if (DCache_IsEnabled() == 1U) {               // If Data Cache is enabled in the MCU
    uint32_t num_of_bytes = CalcNumOfBytes(hdma, num);
  #if (DRIVER_DCACHE_RX_USER_GUARANTEED == 1U)  // If the user guarantees that Data Cache operations can be performed on the buffer
    DCache_Refresh(data, num_of_bytes);         // Refresh the Data Cache from Memory
  #else                                         // Else if the user does not guarantee that Data Cache operations can be performed on the buffer
    if (DCache_IsAligned(data, num_of_bytes) != 0U) {   // If cache operations can be performed on the requested memory area, 
      DCache_Refresh(data, num_of_bytes);       // Refresh the Data Cache from Memory
    }
  #endif
  }
#endif
}

// SmartCard Driver functions

/**
  \fn          ARM_DRIVER_VERSION SMARTCARD_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION SMARTCARD_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES SMARTCARD_GetCapabilities (void)
  \brief       Get driver capabilities
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES SMARTCARD_GetCapabilities (void) {
  return Capabilities;
}

/**
  \fn          int32_t SMARTCARD_Initialize (      ARM_USART_SignalEvent_t   cb_event
                                             const SMARTCARD_RESOURCES      *smartcard)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   smartcard      Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_Initialize (      ARM_USART_SignalEvent_t   cb_event,
                                     const SMARTCARD_RESOURCES      *smartcard) {

  if (smartcard->info->flags & SMARTCARD_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  smartcard->h->Init.Mode = 0;

  // Initialize SMARTCARD Run-time Resources
  smartcard->info->cb_event = cb_event;

  // Clear Status flags
  smartcard->info->status.tx_busy          = 0;
  smartcard->info->status.rx_busy          = 0;
  smartcard->info->status.tx_underflow     = 0;
  smartcard->info->status.rx_overflow      = 0;
  smartcard->info->status.rx_break         = 0;
  smartcard->info->status.rx_framing_error = 0;
  smartcard->info->status.rx_parity_error  = 0;

  // Clear transfer information
  memset(smartcard->xfer, 0, sizeof(SMARTCARD_TRANSFER_INFO));

  smartcard->h->Instance = smartcard->reg;

  smartcard->info->flags = SMARTCARD_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SMARTCARD_Uninitialize (const SMARTCARD_RESOURCES *smartcard)
  \brief       De-initialize SMARTCARD Interface.
  \param[in]   usart     Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_Uninitialize (const SMARTCARD_RESOURCES *smartcard) {

  // Reset SMARTCARD status flags
  smartcard->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SMARTCARD_PowerControl (ARM_POWER_STATE state)
  \brief       Control SMARTCARD Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_PowerControl (      ARM_POWER_STATE       state,
                                       const SMARTCARD_RESOURCES  *smartcard) {

  if ((smartcard->info->flags & SMARTCARD_FLAG_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:

      // SmartCard peripheral reset
      SMARTCARD_PeripheralReset (smartcard->reg);

      if (smartcard->h->Instance != NULL) {
        HAL_SMARTCARD_MspDeInit (smartcard->h);
      }

      // Clear Status flags
      smartcard->info->status.tx_busy          = 0;
      smartcard->info->status.rx_busy          = 0;
      smartcard->info->status.tx_underflow     = 0;
      smartcard->info->status.rx_overflow      = 0;
      smartcard->info->status.rx_break         = 0;
      smartcard->info->status.rx_framing_error = 0;
      smartcard->info->status.rx_parity_error  = 0;

      smartcard->info->flags &= ~SMARTCARD_FLAG_POWERED;
      break;
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((smartcard->info->flags & SMARTCARD_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((smartcard->info->flags & SMARTCARD_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      smartcard->xfer->def_val = 0U;

      // Clear Status flags
      smartcard->info->status.tx_busy          = 0;
      smartcard->info->status.rx_busy          = 0;
      smartcard->info->status.tx_underflow     = 0;
      smartcard->info->status.rx_overflow      = 0;
      smartcard->info->status.rx_break         = 0;
      smartcard->info->status.rx_framing_error = 0;
      smartcard->info->status.rx_parity_error  = 0;

      smartcard->info->flags = SMARTCARD_FLAG_POWERED | SMARTCARD_FLAG_INITIALIZED;

      HAL_SMARTCARD_MspInit (smartcard->h);

      // SmartCard peripheral reset
      SMARTCARD_PeripheralReset (smartcard->reg);

    break;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SMARTCARD_Send (const void                *data,
                                             uint32_t             num,
                                       const SMARTCARD_RESOURCES *smartcard)
  \brief       Start sending data to SMARTCARD transmitter.
  \param[in]   data  Pointer to buffer with data to send to SMARTCARD transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_Send (const void                 *data,
                                     uint32_t              num,
                               const SMARTCARD_RESOURCES  *smartcard) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((smartcard->info->flags & SMARTCARD_FLAG_CONFIGURED) == 0U) {
    // SmartCard is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_SMARTCARD_GetState (smartcard->h)) {
    case HAL_SMARTCARD_STATE_RESET:
    case HAL_SMARTCARD_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_SMARTCARD_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_SMARTCARD_STATE_BUSY:
    case HAL_SMARTCARD_STATE_BUSY_TX:
    case HAL_SMARTCARD_STATE_BUSY_RX:
    case HAL_SMARTCARD_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SMARTCARD_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  smartcard->info->status.tx_underflow = 0;

  // Save buffer info
  smartcard->xfer->tx_num = num;

  smartcard->xfer->tx_dma_flag = 0U;
  if (smartcard->dma_use_tx != 0U) {
    // Determine if DMA should be used for the transfer
    smartcard->xfer->tx_dma_flag = CheckDmaForTx(smartcard->h->hdmatx, data, num);
  }

  if (smartcard->xfer->tx_dma_flag != 0U) {
    // DMA mode
    stat = HAL_SMARTCARD_Transmit_DMA(smartcard->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_SMARTCARD_Transmit_IT (smartcard->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  }

  switch (stat) {
    case HAL_ERROR:
    case HAL_TIMEOUT:
      return ARM_DRIVER_ERROR;

    case HAL_BUSY:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_OK:
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SMARTCARD_Receive (       void                 *data,
                                                 uint32_t              num,
                                           const SMARTCARD_RESOURCES  *smartcard)
  \brief       Start receiving data from SMARTCARD receiver.
  \param[out]  data  Pointer to buffer for data to receive from SMARTCARD receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_Receive (      void                *data,
                                        uint32_t             num,
                                  const SMARTCARD_RESOURCES *smartcard) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((smartcard->info->flags & SMARTCARD_FLAG_CONFIGURED) == 0U) {
    // SmartCard is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_SMARTCARD_GetState (smartcard->h)) {
    case HAL_SMARTCARD_STATE_RESET:
    case HAL_SMARTCARD_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_SMARTCARD_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_SMARTCARD_STATE_BUSY:
    case HAL_SMARTCARD_STATE_BUSY_TX:
    case HAL_SMARTCARD_STATE_BUSY_RX:
    case HAL_SMARTCARD_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SMARTCARD_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  smartcard->info->status.rx_overflow = 0;
  smartcard->info->status.rx_break = 0;
  smartcard->info->status.rx_framing_error = 0;
  smartcard->info->status.rx_parity_error = 0;

  // Save buffer info
  smartcard->xfer->rx_data = data;

  smartcard->xfer->rx_dma_flag = 0U;
  if (smartcard->dma_use_rx != 0U) {
    // Determine if DMA should be used for the transfer
    smartcard->xfer->rx_dma_flag = CheckDmaForRx(smartcard->h->hdmarx, data, num);
  }

  if (smartcard->xfer->rx_dma_flag != 0U) {
    // DMA mode
    stat = HAL_SMARTCARD_Receive_DMA(smartcard->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_SMARTCARD_Receive_IT (smartcard->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  }

  switch (stat) {
    case HAL_ERROR:
    case HAL_TIMEOUT:
      return ARM_DRIVER_ERROR;

    case HAL_BUSY:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_OK:
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SMARTCARD_Transfer (const void                 *data_out,
                                                 void                 *data_in,
                                                 uint32_t              num,
                                           const SMARTCARD_RESOURCES  *smartcard)
  \brief       Start sending/receiving data to/from SMARTCARD transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to SMARTCARD transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from SMARTCARD receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   smartcard      Pointer to SMARTCARD resources
  \return      \ref execution_status
*/
static int32_t SMARTCARD_Transfer (const void                 *data_out,
                                         void                 *data_in,
                                         uint32_t              num,
                                   const SMARTCARD_RESOURCES  *smartcard) {

  UNUSED(data_out);
  UNUSED(data_in);
  UNUSED(num);
  UNUSED(smartcard);

  // Supported only in Synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          uint32_t SMARTCARD_GetTxCount (const SMARTCARD_RESOURCES *smartcard)
  \brief       Get transmitted data count.
  \param[in]   smartcard     Pointer to SMARTCARD resources
  \return      number of data items transmitted
*/
static uint32_t SMARTCARD_GetTxCount (const SMARTCARD_RESOURCES *smartcard) {
  uint32_t cnt;

  if ((smartcard->info->flags & SMARTCARD_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (smartcard->dma_use_tx != 0U) {
    cnt = 0U;
  } else {
    cnt = smartcard->h->TxXferSize - smartcard->h->TxXferCount;
  }

  return cnt;
}

/**
  \fn          uint32_t SMARTCARD_GetRxCount (const SMARTCARD_RESOURCES *smartcard)
  \brief       Get received data count.
  \param[in]   smartcard     Pointer to SMARTCARD resources
  \return      number of data items received
*/
static uint32_t SMARTCARD_GetRxCount (const SMARTCARD_RESOURCES *smartcard) {
  uint32_t cnt;

  if ((smartcard->info->flags & SMARTCARD_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (smartcard->dma_use_rx != 0U) {
    cnt = 0U;
  } else {
    cnt = smartcard->h->RxXferSize - smartcard->h->RxXferCount;
  }

  return cnt;
}

/**
  \fn          int32_t SMARTCARD_Control (      uint32_t              control,
                                                uint32_t              arg,
                                          const SMARTCARD_RESOURCES  *smartcard)
  \brief       Control SMARTCARD Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   smartcard    Pointer to SMARTCARD resources
  \return      common \ref execution_status and driver specific \ref smartcard_execution_status
*/
static int32_t SMARTCARD_Control (     uint32_t              control,
                                       uint32_t              arg,
                                 const SMARTCARD_RESOURCES  *smartcard) {

  PLL2_ClocksTypeDef pll2_clocks;
  PLL3_ClocksTypeDef pll3_clocks;
  HAL_StatusTypeDef  status;
  uint32_t           clksrc, val = 0U;
  uint8_t            i;

  if ((smartcard->info->flags & SMARTCARD_FLAG_POWERED) == 0U) {
    // SmartCard not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
     // Control break
    case ARM_USART_CONTROL_BREAK:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Abort
    case ARM_USART_ABORT_SEND:
      HAL_SMARTCARD_AbortTransmit(smartcard->h);
      smartcard->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;
    case ARM_USART_ABORT_RECEIVE:
      HAL_SMARTCARD_AbortReceive(smartcard->h);
      smartcard->h->RxXferSize = 0U;
      return ARM_DRIVER_OK;
    case ARM_USART_ABORT_TRANSFER:
      HAL_SMARTCARD_Abort(smartcard->h);
      smartcard->h->RxXferSize = 0U;
      smartcard->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;

    // Control TX
    case ARM_USART_CONTROL_TX:
      if (arg) {
        // Transmitter enable
        smartcard->h->Init.Mode |=  SMARTCARD_MODE_TX;
      } else {
        // Transmitter disable
        smartcard->h->Init.Mode &= ~SMARTCARD_MODE_TX;
      }
      status = HAL_SMARTCARD_Init(smartcard->h);
      return SMARTCARD_HAL_STATUS(status);

    // Control RX
    case ARM_USART_CONTROL_RX:
      if (arg) {
        // Receiver enable
        smartcard->h->Init.Mode |= SMARTCARD_MODE_RX;
      } else {
        // Receiver disable
        smartcard->h->Init.Mode &= ~SMARTCARD_MODE_RX;
      }
      status = HAL_SMARTCARD_Init(smartcard->h);
      return SMARTCARD_HAL_STATUS(status);
    default: break;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_IRDA:
    case ARM_USART_MODE_ASYNCHRONOUS:
    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
    case ARM_USART_MODE_SINGLE_WIRE:
      return ARM_USART_ERROR_MODE;

    case ARM_USART_MODE_SMART_CARD:
      break;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      smartcard->xfer->def_val = (uint16_t)arg;
      return ARM_DRIVER_OK;

    // SmartCard guard time
    case ARM_USART_SET_SMART_CARD_GUARD_TIME:
      if (arg > 255U) return ARM_DRIVER_ERROR;
      smartcard->h->Init.GuardTime = arg & 0xFF;
      HAL_SMARTCARD_Init(smartcard->h);
      return ARM_DRIVER_OK;

    // SmartCard clock
    case ARM_USART_SET_SMART_CARD_CLOCK:
      SMARTCARD_GETCLOCKSOURCE(smartcard->h, clksrc);
      switch (clksrc)
      {
        case SMARTCARD_CLOCKSOURCE_D2PCLK1:
          val = HAL_RCC_GetPCLK1Freq();
          break;
        case SMARTCARD_CLOCKSOURCE_D2PCLK2:
          val = HAL_RCC_GetPCLK2Freq();
          break;
        case SMARTCARD_CLOCKSOURCE_PLL2Q:
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          val = pll2_clocks.PLL2_Q_Frequency;
          break;
        case SMARTCARD_CLOCKSOURCE_PLL3Q:
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          val = pll3_clocks.PLL3_Q_Frequency;
          break;
        case SMARTCARD_CLOCKSOURCE_HSI:
         if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIDIV) != 0U) {
          val = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER()>> 3));
         }
         else {
           val = HSI_VALUE;
         }
        break;
        case SMARTCARD_CLOCKSOURCE_CSI:
          val = CSI_VALUE;
          break;
        case SMARTCARD_CLOCKSOURCE_LSE:
          val = LSE_VALUE;
          break;
        case SMARTCARD_CLOCKSOURCE_UNDEFINED:
          return ARM_DRIVER_ERROR;
      }

      // Calculate period in ns
      val = 1000000000U / val;
      for (i = 1U; i <64U; i++) {
        // if in +-2% tolerance
        if (((val * i * 2U * 100U) < (arg * 102U)) &&
            ((val * i * 2U * 100U) > (arg * 98U))    ) {
          break;
        }
      }
      if (i == 64U) { return ARM_DRIVER_ERROR; }
      smartcard->h->Init.Prescaler = i;
      HAL_SMARTCARD_Init(smartcard->h);
      return ARM_DRIVER_OK;

    // SmartCard NACK
    case ARM_USART_CONTROL_SMART_CARD_NACK:
      if (arg != 0U) {
        smartcard->h->Init.NACKEnable = SMARTCARD_NACK_ENABLE;
      } else {
        smartcard->h->Init.NACKEnable = SMARTCARD_NACK_DISABLE;
      }
      HAL_SMARTCARD_Init(smartcard->h);
      return ARM_DRIVER_OK;

    // Unsupported command
    default: { return ARM_DRIVER_ERROR_UNSUPPORTED; }
  }

  // SmartCard Data bits
  if ((control & ARM_USART_DATA_BITS_Msk) == ARM_USART_DATA_BITS_8) {
    smartcard->h->Init.WordLength = SMARTCARD_WORDLENGTH_9B;
  }
  else{
    return ARM_USART_ERROR_DATA_BITS;
  }

  // SmartCard Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_EVEN:
      smartcard->h->Init.Parity = SMARTCARD_PARITY_EVEN;
      break;
    case ARM_USART_PARITY_ODD:
      smartcard->h->Init.Parity = SMARTCARD_PARITY_ODD;
      break;
    case ARM_USART_PARITY_NONE:
    default: return ARM_USART_ERROR_PARITY;
  }

  // SmartCard Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1:
      break;
    case ARM_USART_STOP_BITS_2:
    case ARM_USART_STOP_BITS_1_5:
    case ARM_USART_STOP_BITS_0_5:
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // SmartCard Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
    case ARM_USART_FLOW_CONTROL_CTS:
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // SmartCard Baudrate
  smartcard->h->Init.BaudRate = arg;

  // Set configured flag
  smartcard->info->flags |= SMARTCARD_FLAG_CONFIGURED;

  // Initialize SmartCard
  status = HAL_SMARTCARD_Init(smartcard->h);
  return SMARTCARD_HAL_STATUS(status);
}

/**
  \fn          ARM_USART_STATUS SMARTCARD_GetStatus (const SMARTCARD_RESOURCES *smartcard)
  \brief       Get SMARTCARD status.
  \param[in]   smartcard     Pointer to SMARTCARD resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS SMARTCARD_GetStatus (const SMARTCARD_RESOURCES *smartcard) {
  ARM_USART_STATUS  status = {0};

  switch (HAL_SMARTCARD_GetState (smartcard->h)) {
    case HAL_SMARTCARD_STATE_BUSY:
    case HAL_SMARTCARD_STATE_BUSY_TX_RX:
      status.rx_busy = 1;
      status.tx_busy = 1;
      break;
    case HAL_SMARTCARD_STATE_BUSY_TX:
      status.tx_busy = 1;
      break;
    case HAL_SMARTCARD_STATE_BUSY_RX:
      status.rx_busy = 1;
      break;

    case HAL_SMARTCARD_STATE_TIMEOUT:
    case HAL_SMARTCARD_STATE_READY:
    case HAL_SMARTCARD_STATE_RESET:
    case HAL_SMARTCARD_STATE_ERROR:
      break;
  }

  status.tx_underflow     = smartcard->info->status.tx_underflow;
  status.rx_overflow      = smartcard->info->status.rx_overflow;
  status.rx_break         = smartcard->info->status.rx_break;
  status.rx_framing_error = smartcard->info->status.rx_framing_error;
  status.rx_parity_error  = smartcard->info->status.rx_parity_error;

  return status;
}

/**
  \fn          int32_t SMARTCARD_SetModemControl (ARM_USART_MODEM_CONTROL control
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t SMARTCARD_SetModemControl (ARM_USART_MODEM_CONTROL control) {

  (void) control;

  // No modem control in synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          ARM_USART_MODEM_STATUS SMARTCARD_GetModemStatus (void)
  \brief       Get UART Modem Status lines state.
  \return      modem status \ref ARM_UART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS SMARTCARD_GetModemStatus (void) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}

/**
  * @brief Tx Transfer completed callback.
  * @param hsmartcard: SMARTCARD handle.
  * @retval None
  */
void HAL_SMARTCARD_TxCpltCallback(SMARTCARD_HandleTypeDef *hsmartcard) {
  const SMARTCARD_RESOURCES * smartcard;

  smartcard = SMARTCARD_Resources (hsmartcard);

  if (smartcard->info->cb_event != NULL) {
    smartcard->info->cb_event(ARM_USART_EVENT_TX_COMPLETE | ARM_USART_EVENT_SEND_COMPLETE);
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param hsmartcard: SMARTCARD handle.
  * @retval None
  */
void HAL_SMARTCARD_RxCpltCallback(SMARTCARD_HandleTypeDef *hsmartcard) {
  const SMARTCARD_RESOURCES * smartcard;

  smartcard = SMARTCARD_Resources (hsmartcard);

  if (smartcard->xfer->rx_dma_flag == 3U) {     // If DMA was used for Rx and cache Refresh should be performed
    RefreshDCacheAfterDmaRx(smartcard->h->hdmarx, smartcard->xfer->rx_data, smartcard->h->RxXferSize);
  }

  if (smartcard->info->cb_event != NULL) {
    smartcard->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
  }
}

/**
  * @brief SMARTCARD error callback.
  * @param hsmartcard: SMARTCARD handle.
  * @retval None
  */
void HAL_SMARTCARD_ErrorCallback(SMARTCARD_HandleTypeDef *hsmartcard) {
  const SMARTCARD_RESOURCES * smartcard;
        uint32_t         error;
        uint32_t         event;

  smartcard = SMARTCARD_Resources (hsmartcard);

  error = HAL_SMARTCARD_GetError (smartcard->h);
  event = 0;

  if (error & HAL_SMARTCARD_ERROR_PE) {
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
    smartcard->info->status.rx_parity_error = 1;
  }
  if (error & HAL_SMARTCARD_ERROR_FE) {
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    smartcard->info->status.rx_framing_error = 1;
  }
  if (error & HAL_SMARTCARD_ERROR_ORE) {
    event |= ARM_USART_EVENT_RX_OVERFLOW;
    smartcard->info->status.rx_overflow = 1;
  }

  if ((event != 0) && (smartcard->info->cb_event != NULL)) {
    smartcard->info->cb_event(event);
  }
}

#ifdef USART1_MODE_SMARTCARD
SMARTCARDx_EXPORT_DRIVER(1);
#endif

#ifdef USART2_MODE_SMARTCARD
SMARTCARDx_EXPORT_DRIVER(2);
#endif

#ifdef USART3_MODE_SMARTCARD
SMARTCARDx_EXPORT_DRIVER(3);
#endif

#ifdef USART6_MODE_SMARTCARD
SMARTCARDx_EXPORT_DRIVER(6);
#endif

#ifdef USART10_MODE_SMARTCARD
SMARTCARDx_EXPORT_DRIVER(10);
#endif

#endif

/*! \endcond */
