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

/*! \page stm32h7_usart USART (Synchronous mode)

# Revision History

- Version 2.3
  - Updated DMA and Data Cache handling (improved code readability and maintainability)
  - Made variable status volatile (solved potential LTO problems)
- Version 2.2
  - Corrected GetStatus function and status flags handling
  - Corrected Data Cache maintenance operations in 9-bit mode
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

This driver supports **Synchronous Master** mode.

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
 - Rx Overflow event can be detected only when send/receive/transfer operation is active

DMA usage limitations:
 - The size of DCache line on Cortex M7 is 32 Bytes. To safely perform
   DCache maintenance operations, data must be aligned to a 32 Byte boundary
   and data size must be n*32 Bytes. By default, the alignment and size of
   provided data are checked in USART transfer functions. If data is not
   aligned to a 32 Byte boundary or data size is not n*32 Bytes, DMA is
   not used and IRQ transfer is performed.
 - Advanced user can disable data alignment and/or data size checking by
   setting USART_DCACHE_DATA_xX_ALIGNMENT and/or USART_DCACHE_DATA_xX_SIZE to 0.
   The user must be aware that performing DCache maintenance operations on
   data that is not aligned to a 32 Byte boundary or that is not n*32 Bytes
   in size can be dangerous and can corrupt other data that is also maintained
   by the same 32 Byte DCache line.

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                         | Default value | Value | Description
:----------------------------------|:-------------:|:-----:|:-----------
USART_DCACHE_MAINTENANCE           |     **1**     |   1   | USART DCache maintenance operations: **enabled**
^                                  |       ^       |   0   | USART DCache maintenance operations: **disabled**
USART_DCACHE_DATA_RX_ALIGNMENT     |     **1**     |   1   | USART DCache data Rx alignment check: **enabled**
^                                  |       ^       |   0   | USART DCache data Rx alignment check: **disabled**
USART_DCACHE_DATA_RX_SIZE          |     **1**     |   1   | USART DCache data Rx size check: **enabled**
^                                  |       ^       |   0   | USART DCache data Rx size check: **disabled**
USART_DCACHE_DATA_TX_ALIGNMENT     |     **1**     |   1   | USART DCache data Tx alignment check: **enabled**
^                                  |       ^       |   0   | USART DCache data Tx alignment check: **disabled**
USART_DCACHE_DATA_TX_SIZE          |     **1**     |   1   | USART DCache data Tx size check: **enabled**
^                                  |       ^       |   0   | USART DCache data Tx size check: **disabled**

## STM32CubeMX

The USART driver requires:
  - **USART peripheral clock**
  - **USART** peripheral configured as **Synchronous Master**
  - **USART TX, Rx** and **CK pins**
  - optional **DMA** configuration for transfers

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
USART1     | Synchronous Master           | USART1 Controller
PB14       | Alternate Function Push Pull | USART1 TX Pin
PB15       | Alternate Function Push Pull | USART1 RX Pin
PA8        | Alternate Function Push Pull | USART1 CK Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PB14     | **USART1_TX**
        PB15     | **USART1_RX**
        PA8      | **USART1_CK**
       \n

  2. Under **Categories**: **Connectivity** select **USART1**:

     __Mode__:
       - Mode: **Synchronous Master**
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
            PB15     | USART1_RX     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
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
         checked       | MX_USART1_Init              | USART1           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "USART_STM32H7xx.h"
#include "DCache_STM32H7xx.h"

#ifdef USARTx_MODE_SYNC

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,3)

#ifndef USART_DCACHE_MAINTENANCE
#define USART_DCACHE_MAINTENANCE               (1U)
#endif
#ifndef USART_DCACHE_DATA_RX_ALIGNMENT
#define USART_DCACHE_DATA_RX_ALIGNMENT         (1U)
#endif
#ifndef USART_DCACHE_DATA_RX_SIZE
#define USART_DCACHE_DATA_RX_SIZE              (1U)
#endif
#ifndef USART_DCACHE_DATA_TX_ALIGNMENT
#define USART_DCACHE_DATA_TX_ALIGNMENT         (1U)
#endif
#ifndef USART_DCACHE_DATA_TX_SIZE
#define USART_DCACHE_DATA_TX_SIZE              (1U)
#endif

#define DRIVER_DCACHE_MAINTENANCE              (USART_DCACHE_MAINTENANCE)
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((USART_DCACHE_DATA_RX_ALIGNMENT == 0U) || (USART_DCACHE_DATA_RX_SIZE == 0U))
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (0U)
#endif
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((USART_DCACHE_DATA_TX_ALIGNMENT == 0U) || (USART_DCACHE_DATA_TX_SIZE == 0U))
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (0U)
#endif

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

static const ARM_USART_CAPABILITIES Capabilities = {
    0,      ///< supports UART (Asynchronous) mode
    1,      ///< supports Synchronous Master mode
    0,      ///< supports Synchronous Slave mode
    0,      ///< supports UART Single-wire mode
    0,      ///< supports UART IrDA mode
    0,      ///< supports UART Smart Card mode
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

// USART1
#ifdef USART1_MODE_SYNC
USARTx_RESOURCE_ALLOC(1);
#endif

// USART2
#ifdef USART2_MODE_SYNC
USARTx_RESOURCE_ALLOC(2);
#endif

// USART3
#ifdef USART3_MODE_SYNC
USARTx_RESOURCE_ALLOC(3);
#endif

// USART6
#ifdef USART6_MODE_SYNC
USARTx_RESOURCE_ALLOC(6);
#endif

// USART10
#ifdef USART10_MODE_SYNC
USARTx_RESOURCE_ALLOC(10);
#endif

/**
  \fn          const USART_RESOURCES USART_Resources (USART_HandleTypeDef *husart)
  \brief       Get USART_RESOURCES structure from USART_HandleTypeDef
*/
static const USART_RESOURCES * USART_Resources (USART_HandleTypeDef *husart) {
  const USART_RESOURCES *usart = NULL;

#ifdef USART1_MODE_SYNC
  if (husart->Instance == USART1) { usart = &USART1_Resources; }
#endif
#ifdef USART2_MODE_SYNC
  if (husart->Instance == USART2) { usart = &USART2_Resources; }
#endif
#ifdef USART3_MODE_SYNC
  if (husart->Instance == USART3) { usart = &USART3_Resources; }
#endif
#ifdef USART6_MODE_SYNC
  if (husart->Instance == USART6) { usart = &USART6_Resources; }
#endif
#ifdef USART10_MODE_SYNC
  if (husart->Instance == USART10) { usart = &USART10_Resources; }
#endif

  return usart;
}

/**
  \fn          void USART_PeripheralReset (USART_TypeDef *usart)
  \brief       USART Reset
*/
static void USART_PeripheralReset (USART_TypeDef *usart) {

#ifdef USART1_MODE_SYNC
  if (usart == USART1) { __HAL_RCC_USART1_FORCE_RESET(); }
#endif
#ifdef USART2_MODE_SYNC
  if (usart == USART2) { __HAL_RCC_USART2_FORCE_RESET(); }
#endif
#ifdef USART3_MODE_SYNC
  if (usart == USART3) { __HAL_RCC_USART3_FORCE_RESET(); }
#endif
#ifdef USART6_MODE_SYNC
  if (usart == USART6) { __HAL_RCC_USART6_FORCE_RESET(); }
#endif
#ifdef USART10_MODE_SYNC
  if (usart == USART10) { __HAL_RCC_USART10_FORCE_RESET(); }
#endif

      __NOP(); __NOP(); __NOP(); __NOP();

#ifdef USART1_MODE_SYNC
  if (usart == USART1) { __HAL_RCC_USART1_RELEASE_RESET(); }
#endif
#ifdef USART2_MODE_SYNC
  if (usart == USART2) { __HAL_RCC_USART2_RELEASE_RESET(); }
#endif
#ifdef USART3_MODE_SYNC
  if (usart == USART3) { __HAL_RCC_USART3_RELEASE_RESET(); }
#endif
#ifdef USART6_MODE_SYNC
  if (usart == USART6) { __HAL_RCC_USART6_RELEASE_RESET(); }
#endif
#ifdef USART10_MODE_SYNC
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

// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION USART_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USART_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (void)
  \brief       Get driver capabilities
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (void) {
  return Capabilities;
}

/**
  \fn          int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const USART_RESOURCES         *usart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event,
                                 const USART_RESOURCES         *usart) {

  if (usart->info->flags & USART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  usart->h->Init.Mode = 0;

  // Initialize USART Run-time Resources
  usart->info->cb_event = cb_event;

  // Clear Status flags
  usart->info->status.tx_busy          = 0;
  usart->info->status.rx_busy          = 0;
  usart->info->status.tx_underflow     = 0;
  usart->info->status.rx_overflow      = 0;
  usart->info->status.rx_break         = 0;
  usart->info->status.rx_framing_error = 0;
  usart->info->status.rx_parity_error  = 0;

  // Clear transfer information
  memset(usart->xfer, 0, sizeof(USART_TRANSFER_INFO));

  usart->h->Instance = usart->reg;

  usart->info->flags = USART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (const USART_RESOURCES *usart)
  \brief       De-initialize USART Interface.
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (const USART_RESOURCES *usart) {

  // Reset USART status flags
  usart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (      ARM_POWER_STATE  state,
                                   const USART_RESOURCES *usart) {

  if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:

      // USART peripheral reset
      USART_PeripheralReset (usart->reg);

      if (usart->h->Instance != NULL) {
        HAL_USART_MspDeInit (usart->h);
      }

      // Clear Status flags
      usart->info->status.tx_busy          = 0;
      usart->info->status.rx_busy          = 0;
      usart->info->status.tx_underflow     = 0;
      usart->info->status.rx_overflow      = 0;
      usart->info->status.rx_break         = 0;
      usart->info->status.rx_framing_error = 0;
      usart->info->status.rx_parity_error  = 0;

      usart->info->flags &= ~USART_FLAG_POWERED;
      break;
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((usart->info->flags & USART_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      usart->xfer->def_val = 0U;

      // Clear Status flags
      usart->info->status.tx_busy          = 0;
      usart->info->status.rx_busy          = 0;
      usart->info->status.tx_underflow     = 0;
      usart->info->status.rx_overflow      = 0;
      usart->info->status.rx_break         = 0;
      usart->info->status.rx_framing_error = 0;
      usart->info->status.rx_parity_error  = 0;

      usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

      HAL_USART_MspInit (usart->h);

      // USART peripheral reset
      USART_PeripheralReset (usart->reg);

    break;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void            *data,
                                         uint32_t         num,
                                   const USART_RESOURCES *usart)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Send (const void            *data,
                                 uint32_t         num,
                           const USART_RESOURCES *usart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  usart->info->status.tx_underflow = 0;

  // Save buffer info
  usart->xfer->tx_num = num;

  usart->xfer->dma_flag = 0U;
  if (usart->dma_use != 0U) {
    // Determine if DMA should be used for the transfer
    usart->xfer->dma_flag = CheckDmaForTx(usart->h->hdmatx, data, num);
  }

  if (usart->xfer->dma_flag != 0U) {
    // DMA mode
    stat = HAL_USART_Transmit_DMA(usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_Transmit_IT (usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t USART_Receive (      void            *data,
                                            uint32_t         num,
                                      const USART_RESOURCES *usart)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Receive (      void            *data,
                                    uint32_t         num,
                              const USART_RESOURCES *usart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  usart->info->status.rx_overflow = 0;
  usart->info->status.rx_break = 0;
  usart->info->status.rx_framing_error = 0;
  usart->info->status.rx_parity_error = 0;

  // Save buffer info
  usart->xfer->rx_num = num;
  usart->xfer->rx_data = data;

  usart->xfer->dma_flag = 0U;
  if (usart->dma_use != 0U) {
    // Determine if DMA should be used for the transfer
    usart->xfer->dma_flag = CheckDmaForRx(usart->h->hdmarx, data, num);
  }

  if (usart->xfer->dma_flag != 0U) {
    // DMA mode
    stat = HAL_USART_Receive_DMA(usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_Receive_IT (usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t USART_Transfer (const void             *data_out,
                                             void             *data_in,
                                             uint32_t          num,
                                       const USART_RESOURCES  *usart)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void             *data_out,
                                     void             *data_in,
                                     uint32_t          num,
                               const USART_RESOURCES  *usart) {

  HAL_StatusTypeDef stat;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  usart->xfer->dma_flag = 0U;
  if (usart->dma_use != 0U) {
    // Determine if DMA should be used for the transfer, only if Tx and Rx can use DMA
    if (CheckDmaForTx(usart->h->hdmatx, data_out, num) != 0U) {
      usart->xfer->dma_flag = CheckDmaForRx(usart->h->hdmarx, data_in, num);
    }
  }

  if (usart->xfer->dma_flag != 0U) {
    // DMA mode
    stat = HAL_USART_TransmitReceive_DMA(usart->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_TransmitReceive_IT (usart->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
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
  \fn          uint32_t USART_GetTxCount (const USART_RESOURCES *usart)
  \brief       Get transmitted data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (const USART_RESOURCES *usart) {
  uint32_t cnt;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (usart->dma_use != 0U) {
    cnt = usart->xfer->tx_num - __HAL_DMA_GET_COUNTER(usart->h->hdmatx);
  } else {
    cnt = usart->h->TxXferSize - usart->h->TxXferCount;
  }

  return cnt;
}

/**
  \fn          uint32_t USART_GetRxCount (const USART_RESOURCES *usart)
  \brief       Get received data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (const USART_RESOURCES *usart) {
  uint32_t cnt;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (usart->dma_use != 0U) {
    cnt = usart->xfer->rx_num - __HAL_DMA_GET_COUNTER(usart->h->hdmarx);
  } else {
    cnt = usart->h->RxXferSize - usart->h->RxXferCount;
  }

  return cnt;
}

/**
  \fn          int32_t USART_Control (      uint32_t          control,
                                            uint32_t          arg,
                                      const USART_RESOURCES  *usart)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   usart    Pointer to USART resources
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (      uint32_t          control,
                                    uint32_t          arg,
                              const USART_RESOURCES  *usart) {

  HAL_StatusTypeDef status;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    // USART not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
     // Control break
    case ARM_USART_CONTROL_BREAK:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Abort
    case ARM_USART_ABORT_SEND:
    case ARM_USART_ABORT_RECEIVE:
    case ARM_USART_ABORT_TRANSFER:
      HAL_USART_Abort(usart->h);
      usart->h->RxXferSize = 0U;
      usart->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;

    // Control TX
    case ARM_USART_CONTROL_TX:
      if (arg) {
        // Transmitter enable
        usart->h->Init.Mode |=  USART_MODE_TX;
      } else {
        // Transmitter disable
        usart->h->Init.Mode &= ~USART_MODE_TX;
      }
      status = HAL_USART_Init(usart->h);
      return USART_HAL_STATUS(status);

    // Control RX
    case ARM_USART_CONTROL_RX:
      if (arg) {
        // Receiver enable
        usart->h->Init.Mode |= USART_MODE_RX;
      } else {
        // Receiver disable
        usart->h->Init.Mode &= ~USART_MODE_RX;
      }
      status = HAL_USART_Init(usart->h);
      return USART_HAL_STATUS(status);
    default: break;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS:
    case ARM_USART_MODE_IRDA:
    case ARM_USART_MODE_SMART_CARD:
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
    case ARM_USART_MODE_SINGLE_WIRE:
      return ARM_USART_ERROR_MODE;

    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
      break;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      usart->xfer->def_val = (uint16_t)arg;
      return ARM_DRIVER_OK;

    default: { return ARM_DRIVER_ERROR_UNSUPPORTED; }
  }

  // USART Data bits
  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_6:
      if ((control & ARM_USART_PARITY_Msk) != ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_7B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    case ARM_USART_DATA_BITS_7:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_7B;
      } else {
        usart->h->Init.WordLength = USART_WORDLENGTH_8B;
      }
      break;
    case ARM_USART_DATA_BITS_8:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_8B;
      } else {
        usart->h->Init.WordLength = USART_WORDLENGTH_9B;
      }
      break;
    case ARM_USART_DATA_BITS_9:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_9B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE:
      usart->h->Init.Parity = USART_PARITY_NONE;
      break;
    case ARM_USART_PARITY_EVEN:
      usart->h->Init.Parity = USART_PARITY_EVEN;
      break;
    case ARM_USART_PARITY_ODD:
      usart->h->Init.Parity = USART_PARITY_ODD;
      break;
    default: return ARM_USART_ERROR_PARITY;
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1:
      usart->h->Init.StopBits = USART_STOPBITS_1;
      break;
    case ARM_USART_STOP_BITS_2:
      usart->h->Init.StopBits = USART_STOPBITS_2;
      break;
    case ARM_USART_STOP_BITS_1_5:
      usart->h->Init.StopBits = USART_STOPBITS_1_5;
      break;
    case ARM_USART_STOP_BITS_0_5:
      usart->h->Init.StopBits = USART_STOPBITS_0_5;
      break;
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
    case ARM_USART_FLOW_CONTROL_CTS:
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // Clock setting for synchronous mode
  // Polarity
  switch (control & ARM_USART_CPOL_Msk) {
    case ARM_USART_CPOL0:
      usart->h->Init.CLKPolarity = USART_POLARITY_LOW;
      break;
    case ARM_USART_CPOL1:
      usart->h->Init.CLKPolarity = USART_POLARITY_HIGH;
      break;
    default: return ARM_USART_ERROR_CPOL;
  }

  // Phase
  switch (control & ARM_USART_CPHA_Msk) {
    case ARM_USART_CPHA0:
      usart->h->Init.CLKPhase = USART_PHASE_1EDGE;
      break;
    case ARM_USART_CPHA1:
      usart->h->Init.CLKPhase = USART_PHASE_2EDGE;
      break;
    default: return ARM_USART_ERROR_CPHA;
  }

  // USART Baudrate
  usart->h->Init.BaudRate = arg;

  // Set configured flag
  usart->info->flags |= USART_FLAG_CONFIGURED;

  // Initialize USART
  status = HAL_USART_Init(usart->h);

  // Reconfigure DMA
  if (usart->dma_use != 0U) {
    if (usart->h->hdmatx != NULL) {
      if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
        usart->h->hdmatx->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        usart->h->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      } else {
        usart->h->hdmatx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        usart->h->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      }
      HAL_DMA_Init(usart->h->hdmatx);
    }

    if (usart->h->hdmarx != NULL) {
      if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
        usart->h->hdmarx->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        usart->h->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      } else {
        usart->h->hdmarx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        usart->h->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      }
      HAL_DMA_Init(usart->h->hdmarx);
    }
  }
  return USART_HAL_STATUS(status);
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart)
  \brief       Get USART status.
  \param[in]   usart     Pointer to USART resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart) {
  ARM_USART_STATUS  status = {0U};

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX_RX:
      status.rx_busy = 1;
      status.tx_busy = 1;
      break;
    case HAL_USART_STATE_BUSY_TX:
      status.tx_busy = 1;
      break;
    case HAL_USART_STATE_BUSY_RX:
      status.rx_busy = 1;
      break;

    case HAL_USART_STATE_TIMEOUT:
    case HAL_USART_STATE_READY:
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      break;
  }

  status.tx_underflow     = usart->info->status.tx_underflow;
  status.rx_overflow      = usart->info->status.rx_overflow;
  status.rx_break         = usart->info->status.rx_break;
  status.rx_framing_error = usart->info->status.rx_framing_error;
  status.rx_parity_error  = usart->info->status.rx_parity_error;

  return status;
}

/**
  \fn          int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control) {

  (void) control;

  // No modem control in synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (void)
  \brief       Get USART Modem Status lines state.
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (void) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}

/**
  * @brief Tx Transfer completed callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_TX_COMPLETE | ARM_USART_EVENT_SEND_COMPLETE);
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);

  if (usart->xfer->dma_flag == 3U) {            // If DMA was used for Rx and cache Refresh should be performed
    RefreshDCacheAfterDmaRx(usart->h->hdmarx, usart->xfer->rx_data, usart->h->RxXferSize);
  }

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
  }
}

/**
  * @brief Tx/Rx Transfers completed callback for the non-blocking process.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);

  if (usart->xfer->dma_flag == 3U) {            // If DMA was used for Rx and cache Refresh should be performed
    RefreshDCacheAfterDmaRx(usart->h->hdmarx, usart->xfer->rx_data, usart->h->RxXferSize);
  }

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_TRANSFER_COMPLETE);
  }
}

/**
  * @brief USART error callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;
        uint32_t          error;
        uint32_t          event;

  usart = USART_Resources (husart);

  error = HAL_USART_GetError (usart->h);
  event = 0;

  if (error & HAL_USART_ERROR_PE) {
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
    usart->info->status.rx_parity_error = 1;
  }
  if (error & HAL_USART_ERROR_FE) {
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    usart->info->status.rx_framing_error = 1;
  }
  if (error & HAL_USART_ERROR_ORE) {
    event |= ARM_USART_EVENT_RX_OVERFLOW;
    usart->info->status.rx_overflow = 1;
  }

  if ((event != 0) && (usart->info->cb_event != NULL)) {
    usart->info->cb_event(event);
  }
}


#ifdef USART1_MODE_SYNC
USARTx_EXPORT_DRIVER(1);
#endif

#ifdef USART2_MODE_SYNC
USARTx_EXPORT_DRIVER(2);
#endif

#ifdef USART3_MODE_SYNC
USARTx_EXPORT_DRIVER(3);
#endif

#ifdef USART6_MODE_SYNC
USARTx_EXPORT_DRIVER(6);
#endif

#ifdef USART10_MODE_SYNC
USARTx_EXPORT_DRIVER(10);
#endif

#endif

/*! \endcond */
