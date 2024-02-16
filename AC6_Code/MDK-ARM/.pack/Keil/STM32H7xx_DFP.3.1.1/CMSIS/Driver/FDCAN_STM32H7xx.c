/* -----------------------------------------------------------------------------
 * Copyright (c) 2017-2023 Arm Limited (or its affiliates).
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
 * $Revision:    V1.4
 *
 * Driver:       Driver_CAN1/2
 *
 * Configured:   via CubeMX
 * Project:      CAN1/2 Driver for FDCAN 1/2 of ST STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_can CAN

# Revision History

- Version 1.4
  - Updated function CANx_GetStatus (solved potential LTO problem)
- Version 1.3
  - Updated __MEMORY_AT macro
- Version 1.2
  - Corrected CAN SRAM read/write to 4 byte accesses
- Version 1.1
  - Changed FDCAN_ECR_TREC_.. to FDCAN_ECR_REC_.. to align with updated device header
  - Corrected SetBitrate function
- Version 1.0
  - Initial release

# Capabilities

This driver supports **Master** mode.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance     | Hardware Resource
:-------------------|:-----------------:
Driver_CAN1         | FDCAN1
Driver_CAN2         | FDCAN2

# Limitations

Driver limitations:
 - TTCAN is not supported
 - Tx FIFO and Tx Buffers transmit concepts are used (Tx Queue is not used)
 - Only interrupt 0 line is used

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                         | Default value | Max Value | Description
:----------------------------------|:-------------:|:---------:|:-----------
CAN_CLOCK_TOLERANCE                | **15**        | 1024      | clock tolerance in 1/n steps
CAN_NON_ISO_EN                     | **0**         |    0      | non-ISO CAN FD mode disabled
^                                  | ^             |    1      | non-ISO CAN FD mode enabled
CAN1_RX_FIFO0_ELEM_NUM             | **8**         |   64      | number of receive FIFO0 elements
CAN1_RX_FIFO1_ELEM_NUM             | **8**         |   64      | number of receive FIFO1 elements
CAN1_TX_FIFO_ELEM_NUM              | **8**         |   32      | number of transmit FIFO elements
CAN1_RX_BUF_NUM                    | **16**        |   64      | maximum number of receive buffers
CAN1_TX_BUF_NUM                    | **16**        |   32      | maximum number of transmit buffers
CAN1_FILTER_STD_NUM                | **128**       |  128      | maximum number of standard elements
CAN1_FILTER_EXT_NUM                | **64**        |   64      | maximum number of extended elements
CAN2_RX_FIFO0_ELEM_NUM             | **8**         |   64      | number of receive FIFO0 elements
CAN2_RX_FIFO1_ELEM_NUM             | **8**         |   64      | number of receive FIFO1 elements
CAN2_TX_FIFO_ELEM_NUM              | **8**         |   32      | number of transmit FIFO elements
CAN2_RX_BUF_NUM                    | **16**        |   64      | maximum number of receive buffers
CAN2_TX_BUF_NUM                    | **16**        |   32      | maximum number of transmit buffers
CAN2_FILTER_STD_NUM                | **128**       |  128      | maximum number of standard elements
CAN2_FILTER_EXT_NUM                | **64**        |   64      | maximum number of extended elements

## STM32CubeMX

The CAN driver requires:
  - **FDCAN peripheral clock**
  - **FDCAN** peripheral Mode **Activated**
  - **FDCAN TX** and **Rx pins**

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
FDCAN1     | Activated                    | FDCAN1 Controller
PA11       | Alternate Function Push Pull | FDCAN1_RX Pin
PA12       | Alternate Function Push Pull | FDCAN1_TX Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PA11     | **FDCAN1_RX**
        PA12     | **FDCAN1_TX**
       \n

  2. Under **Categories**: **Connectivity** select **FDCAN1**:

     __Mode__:
       - Activated: **checked**

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PA11     | FDCAN1_RX     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .
            PA12     | FDCAN1_TX     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .

  3. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
            :---------------------------------|:-----------:|:-------------------:|:------------:
            FDCAN1 interrupt 0                | unchecked   | 0                   | 0
            FDCAN1 interrupt 1                | unchecked   | 0                   | 0
            FDCAN calibration unit interrupt  | unchecked   | 0                   | 0

### Clock Configuration tab

  1. Configure **To FDCAN (MHz)**: **20**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_FDCAN_Init               | FDCAN1           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "FDCAN_STM32H7xx.h"

#include <string.h>

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_fdcan.h"

#include "MX_Device.h"


// Define macros for used CAN controllers
#if      (defined(MX_FDCAN1) && (MX_FDCAN1 == 1U))
  #define CAN1_USED                     (1)
#else
  #define CAN1_USED                     (0)
#endif
#if      (defined(MX_FDCAN2) && (MX_FDCAN2 == 1U))
  #define CAN2_USED                     (1)
#else
  #define CAN2_USED                     (0)
#endif

// Determine number of instances from CubeMX defines
#if      (CAN2_USED == 1)
  #define CAN_CTRL_NUM                  (2U)
#elif    (CAN1_USED == 1)
  #define CAN_CTRL_NUM                  (1U)
#else
  #error "No FDCAN configured in STM32CubeMX!"
#endif


// Externally overridable configuration definitions

// Maximum allowed clock tolerance in 1/1024 steps
#ifndef CAN_CLOCK_TOLERANCE
#define CAN_CLOCK_TOLERANCE             (15U)   // 15/1024 approx. 1.5 %
#endif

// Use non-ISO CAN FD
#ifndef CAN_NON_ISO_EN
#define CAN_NON_ISO_EN                  (0U)    // non-ISO disabled by default
#endif


// CAN1 controller configuration ------

#ifndef CAN1_RX_FIFO0_ELEM_NUM
#define CAN1_RX_FIFO0_ELEM_NUM          (8U)
#endif
#if    (CAN1_RX_FIFO0_ELEM_NUM > 64U)
#error  Too many Receive FIFO0 Elements defined for CAN1, maximum number of Receive FIFO0 Elements is 64 !!!
#endif

#ifndef CAN1_RX_FIFO1_ELEM_NUM
#define CAN1_RX_FIFO1_ELEM_NUM          (8U)
#endif
#if    (CAN1_RX_FIFO1_ELEM_NUM > 64U)
#error  Too many Receive FIFO1 Elements defined for CAN1, maximum number of Receive FIFO1 Elements is 64 !!!
#endif

#ifndef CAN1_RX_BUF_NUM
#define CAN1_RX_BUF_NUM                 (16U)
#endif
#if    (CAN1_RX_BUF_NUM > 64U)
#error  Too many Receive Buffers defined for CAN1, maximum number of Receive Buffers is 64 !!!
#endif

#ifndef CAN1_TX_FIFO_ELEM_NUM
#define CAN1_TX_FIFO_ELEM_NUM           (8U)
#endif
#if    (CAN1_TX_FIFO_ELEM_NUM > 32U)
#error  Too many Transmit FIFO Elements defined for CAN1, maximum number of Transmit FIFO Elements is 32 !!!
#endif

#ifndef CAN1_TX_BUF_NUM
#define CAN1_TX_BUF_NUM                 (16U)
#endif
#if    (CAN1_TX_BUF_NUM > 32U)
#error  Too many Transmit Buffers defined for CAN1, maximum number of Transmit Buffers is 32 !!!
#endif

#if    ((CAN1_TX_FIFO_ELEM_NUM + CAN1_TX_BUF_NUM) > 32U)
#error  Too many Transmit Elements and Buffers defined for CAN1, maximum number of Transmit Elements and Buffers together is 32 !!!
#endif

#ifndef CAN1_FILTER_STD_NUM
#define CAN1_FILTER_STD_NUM             (128U)
#endif
#if    (CAN1_FILTER_STD_NUM > 128U)
#error  Standard Filter Elements defined for CAN1, maximum number of Standard Filter Elements is 128 !!!
#endif

#ifndef CAN1_FILTER_EXT_NUM
#define CAN1_FILTER_EXT_NUM             (64U)
#endif
#if    (CAN1_FILTER_EXT_NUM > 64U)
#error  Extended Filter Elements defined for CAN1, maximum number of Extended Filter Elements is 64 !!!
#endif

#define CAN1_RX_FIFO_EN                ((CAN1_RX_FIFO0_ELEM_NUM != 0U) || (CAN1_RX_FIFO1_ELEM_NUM != 0U))
#define CAN1_RX_BUF_EN                  (CAN1_RX_BUF_NUM        != 0U)
#define CAN1_TX_FIFO_EN                 (CAN1_TX_FIFO_ELEM_NUM  != 0U)
#define CAN1_TX_BUF_EN                  (CAN1_TX_BUF_NUM        != 0U)
#define CAN1_RX_FIFO_OBJ_NUM           ((CAN1_RX_FIFO0_ELEM_NUM != 0U) + (CAN1_RX_FIFO1_ELEM_NUM != 0U))
#define CAN1_RX_BUF_OBJ_NUM             (CAN1_RX_BUF_NUM)
#define CAN1_RX_TOT_OBJ_NUM             (CAN1_RX_FIFO_OBJ_NUM + CAN1_RX_BUF_OBJ_NUM)
#define CAN1_TX_FIFO_OBJ_NUM           ((CAN1_TX_FIFO_ELEM_NUM  != 0U))
#define CAN1_TX_BUF_OBJ_NUM             (CAN1_TX_BUF_NUM)
#define CAN1_TX_TOT_OBJ_NUM             (CAN1_TX_FIFO_OBJ_NUM + CAN1_TX_BUF_OBJ_NUM)
#define CAN1_TOT_OBJ_NUM                (CAN1_RX_TOT_OBJ_NUM  + CAN1_TX_TOT_OBJ_NUM)
#define CAN1_TOT_ELEM_NUM               (CAN1_RX_FIFO0_ELEM_NUM + CAN1_RX_FIFO1_ELEM_NUM + CAN1_RX_BUF_NUM + CAN1_TX_BUF_NUM)

#define CAN1_RAM_OFS_STD_ID             (0U)
#define CAN1_RAM_OFS_EXT_ID             (CAN1_RAM_OFS_STD_ID        +   CAN1_FILTER_STD_NUM)
#define CAN1_RAM_OFS_RX_FIFO0           (CAN1_RAM_OFS_EXT_ID        +  (CAN1_FILTER_EXT_NUM    * 2U))
#define CAN1_RAM_OFS_RX_FIFO1           (CAN1_RAM_OFS_RX_FIFO0      +  (CAN1_RX_FIFO0_ELEM_NUM * 18U))
#define CAN1_RAM_OFS_RX_BUFFER          (CAN1_RAM_OFS_RX_FIFO1      +  (CAN1_RX_FIFO1_ELEM_NUM * 18U))
#define CAN1_RAM_OFS_TX_EVENT_FIFO      (CAN1_RAM_OFS_RX_BUFFER     +  (CAN1_RX_BUF_NUM        * 18U))
#define CAN1_RAM_OFS_TX_BUFFER          (CAN1_RAM_OFS_TX_EVENT_FIFO +  (CAN1_TX_FIFO_ELEM_NUM  * 2U))
#define CAN1_TOT_RAM                    (CAN1_RAM_OFS_TX_BUFFER     + ((CAN1_TX_FIFO_ELEM_NUM  + CAN1_RX_BUF_OBJ_NUM) * 18U))


// CAN2 controller configuration ------

#ifndef CAN2_RX_FIFO0_ELEM_NUM
#define CAN2_RX_FIFO0_ELEM_NUM          (8U)
#endif
#if    (CAN2_RX_FIFO0_ELEM_NUM > 64U)
#error  Too many Receive FIFO0 Elements defined for CAN2, maximum number of Receive FIFO0 Elements is 64 !!!
#endif

#ifndef CAN2_RX_FIFO1_ELEM_NUM
#define CAN2_RX_FIFO1_ELEM_NUM          (8U)
#endif
#if    (CAN2_RX_FIFO1_ELEM_NUM > 64U)
#error  Too many Receive FIFO1 Elements defined for CAN2, maximum number of Receive FIFO1 Elements is 64 !!!
#endif

#ifndef CAN2_RX_BUF_NUM
#define CAN2_RX_BUF_NUM                 (16U)
#endif
#if    (CAN2_RX_BUF_NUM > 64U)
#error  Too many Receive Buffers defined for CAN2, maximum number of Receive Buffers is 64 !!!
#endif

#ifndef CAN2_TX_FIFO_ELEM_NUM
#define CAN2_TX_FIFO_ELEM_NUM           (8U)
#endif
#if    (CAN2_TX_FIFO_ELEM_NUM > 32U)
#error  Too many Transmit FIFO Elements defined for CAN2, maximum number of Transmit FIFO Elements is 32 !!!
#endif

#ifndef CAN2_TX_BUF_NUM
#define CAN2_TX_BUF_NUM                 (16U)
#endif
#if    (CAN2_TX_BUF_NUM > 32U)
#error  Too many Transmit Buffers defined for CAN2, maximum number of Transmit Buffers is 32 !!!
#endif

#if    ((CAN2_TX_FIFO_ELEM_NUM + CAN2_TX_BUF_NUM) > 32U)
#error  Too many Transmit Elements and Buffers defined for CAN2, maximum number of Transmit Elements and Buffers together is 32 !!!
#endif

#ifndef CAN2_FILTER_STD_NUM
#define CAN2_FILTER_STD_NUM             (128U)
#endif
#if    (CAN2_FILTER_STD_NUM > 128U)
#error  Standard Filter Elements defined for CAN2, maximum number of Standard Filter Elements is 128 !!!
#endif

#ifndef CAN2_FILTER_EXT_NUM
#define CAN2_FILTER_EXT_NUM             (64U)
#endif
#if    (CAN2_FILTER_EXT_NUM > 64U)
#error  Extended Filter Elements defined for CAN2, maximum number of Extended Filter Elements is 64 !!!
#endif

#define CAN2_RX_FIFO_EN                ((CAN2_RX_FIFO0_ELEM_NUM != 0U) || (CAN2_RX_FIFO1_ELEM_NUM != 0U))
#define CAN2_RX_BUF_EN                  (CAN2_RX_BUF_NUM        != 0U)
#define CAN2_TX_FIFO_EN                 (CAN2_TX_FIFO_ELEM_NUM  != 0U)
#define CAN2_TX_BUF_EN                  (CAN2_TX_BUF_NUM        != 0U)
#define CAN2_RX_FIFO_OBJ_NUM           ((CAN2_RX_FIFO0_ELEM_NUM != 0U) + (CAN2_RX_FIFO1_ELEM_NUM != 0U))
#define CAN2_RX_BUF_OBJ_NUM             (CAN2_RX_BUF_NUM)
#define CAN2_RX_TOT_OBJ_NUM             (CAN2_RX_FIFO_OBJ_NUM + CAN2_RX_BUF_OBJ_NUM)
#define CAN2_TX_FIFO_OBJ_NUM           ((CAN2_TX_FIFO_ELEM_NUM  != 0U))
#define CAN2_TX_BUF_OBJ_NUM             (CAN2_TX_BUF_NUM)
#define CAN2_TX_TOT_OBJ_NUM             (CAN2_TX_FIFO_OBJ_NUM + CAN2_TX_BUF_OBJ_NUM)
#define CAN2_TOT_OBJ_NUM                (CAN2_RX_TOT_OBJ_NUM  + CAN2_TX_TOT_OBJ_NUM)
#define CAN2_TOT_ELEM_NUM               (CAN2_RX_FIFO0_ELEM_NUM + CAN2_RX_FIFO1_ELEM_NUM + CAN2_RX_BUF_NUM + CAN2_TX_BUF_NUM)

#define CAN2_RAM_OFS_STD_ID             (0U)
#define CAN2_RAM_OFS_EXT_ID             (CAN2_RAM_OFS_STD_ID        +   CAN2_FILTER_STD_NUM)
#define CAN2_RAM_OFS_RX_FIFO0           (CAN2_RAM_OFS_EXT_ID        +  (CAN2_FILTER_EXT_NUM * 2U))
#define CAN2_RAM_OFS_RX_FIFO1           (CAN2_RAM_OFS_RX_FIFO0      +  (CAN2_RX_FIFO0_ELEM_NUM * 18U))
#define CAN2_RAM_OFS_RX_BUFFER          (CAN2_RAM_OFS_RX_FIFO1      +  (CAN2_RX_FIFO1_ELEM_NUM * 18U))
#define CAN2_RAM_OFS_TX_EVENT_FIFO      (CAN2_RAM_OFS_RX_BUFFER     +  (CAN2_RX_BUF_NUM * 18U))
#define CAN2_RAM_OFS_TX_BUFFER          (CAN2_RAM_OFS_TX_EVENT_FIFO +  (CAN2_TX_FIFO_ELEM_NUM * 2U))
#define CAN2_TOT_RAM                    (CAN2_RAM_OFS_TX_BUFFER     + ((CAN2_TX_FIFO_ELEM_NUM + CAN2_RX_BUF_OBJ_NUM) * 18U))

// Check Message RAM configuration
#if ((CAN1_USED == 1) && (CAN2_USED == 0) && (CAN1_TOT_RAM > 2560U))
  #error Message RAM contains only 10 kB, current configuration of CAN1 requires more memory, please reduce number of FIFO or buffer elements for Receive or Transmit!
#endif
#if ((CAN1_USED == 0) && (CAN2_USED == 1) && (CAN2_TOT_RAM > 2560U))
  #error Message RAM contains only 10 kB, current configuration of CAN2 requires more memory, please reduce number of FIFO or buffer elements for Receive or Transmit!
#endif
#if ((CAN1_USED == 1) && (CAN2_USED == 1) && ((CAN1_TOT_RAM + CAN2_TOT_RAM) > 2560U))
  #error Message RAM contains only 10 kB, current configuration of CAN1 and CAN2 requires more memory, please reduce number of FIFO or buffer elements for Receive or Transmit!
#endif

// CAN message RAM base
#define CAN_RAM_BASE                    (0x4000AC00)

// Macro for absolute  ZI-memory positioning
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


#if   (CAN1_USED == 1)
extern FDCAN_HandleTypeDef hfdcan1;
#endif
#if   (CAN2_USED == 1)
extern FDCAN_HandleTypeDef hfdcan2;
#endif


// CAN Driver ******************************************************************

#define ARM_CAN_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,4) // CAN driver version

// Driver Version
static const ARM_DRIVER_VERSION can_driver_version = { ARM_CAN_API_VERSION, ARM_CAN_DRV_VERSION };

// Driver Capabilities
static const ARM_CAN_CAPABILITIES can_driver_capabilities[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 driver capabilities
    CAN1_TOT_OBJ_NUM,           // Number of CAN Objects available
    1U,                         // Supports reentrant calls to ARM_CAN_MessageSend, ARM_CAN_MessageRead, ARM_CAN_ObjectConfigure and abort message sending used by ARM_CAN_Control.
    1U,                         // Supports CAN with flexible data-rate mode (CAN_FD)
    1U,                         // Supports restricted operation mode
    1U,                         // Supports bus monitoring mode
    1U,                         // Supports internal loopback mode
    1U                          // Supports external loopback mode
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 driver capabilities
    CAN2_TOT_OBJ_NUM,           // Number of CAN Objects available
    1U,                         // Supports reentrant calls to ARM_CAN_MessageSend, ARM_CAN_MessageRead, ARM_CAN_ObjectConfigure and abort message sending used by ARM_CAN_Control.
    1U,                         // Supports CAN with flexible data-rate mode (CAN_FD)
    1U,                         // Supports restricted operation mode
    1U,                         // Supports bus monitoring mode
    1U,                         // Supports internal loopback mode
    1U                          // Supports external loopback mode
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};

// Object Capabilities
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_rx_fifo0[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 Rx FIFO0 object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    1U,                         // Object supports range identifier filtering
    1U,                         // Object supports mask identifier filtering
    CAN1_RX_FIFO0_ELEM_NUM      // Object can buffer CAN1_RX_FIFO0_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 Rx FIFO0 object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    1U,                         // Object supports range identifier filtering
    1U,                         // Object supports mask identifier filtering
    CAN2_RX_FIFO0_ELEM_NUM      // Object can buffer CAN2_RX_FIFO0_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_rx_fifo1[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 Rx FIFO1 object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    1U,                         // Object supports range identifier filtering
    1U,                         // Object supports mask identifier filtering
    CAN1_RX_FIFO1_ELEM_NUM      // Object can buffer CAN1_RX_FIFO1_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 Rx FIFO1 object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    1U,                         // Object supports range identifier filtering
    1U,                         // Object supports mask identifier filtering
    CAN2_RX_FIFO1_ELEM_NUM      // Object can buffer CAN2_RX_FIFO1_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_rx_buffer[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 Rx Buffer object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    1U                          // Object can buffer 1 messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 Rx Buffer object capabilities
    0U,                         // Object does not support transmission
    1U,                         // Object supports reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    1U,                         // Object allows assignment of multiple filters to it
    1U,                         // Object supports exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    1U                          // Object can buffer 1 messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_tx_fifo[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 Tx FIFO object capabilities
    1U,                         // Object supports transmission
    0U,                         // Object does not support reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    0U,                         // Object does not allow assignment of multiple filters to it
    0U,                         // Object does not support exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    CAN1_TX_FIFO_ELEM_NUM       // Object can buffer CAN1_TX_FIFO_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 Tx FIFO object capabilities
    1U,                         // Object supports transmission
    0U,                         // Object does not support reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    0U,                         // Object does not allow assignment of multiple filters to it
    0U,                         // Object does not support exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    CAN2_TX_FIFO_ELEM_NUM       // Object can buffer CAN2_TX_FIFO_ELEM_NUM configured number of messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_tx_buffer[2] = {
#if (CAN1_USED == 1)
  {                             // CAN1 Tx Buffer object capabilities
    1U,                         // Object supports transmission
    0U,                         // Object does not support reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    0U,                         // Object does not allow assignment of multiple filters to it
    0U,                         // Object does not support exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    1U                          // Object can buffer 1 message
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  },
#endif
#if (CAN2_USED == 1)
  {                             // CAN2 Tx Buffer object capabilities
    1U,                         // Object supports transmission
    0U,                         // Object does not support reception
    0U,                         // Object does not support RTR reception and automatic Data transmission
    0U,                         // Object does not support RTR transmission and automatic Data reception
    0U,                         // Object does not allow assignment of multiple filters to it
    0U,                         // Object does not support exact identifier filtering
    0U,                         // Object does not support range identifier filtering
    0U,                         // Object does not support mask identifier filtering
    1U                          // Object can buffer 1 message
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#else
  { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
  , 0U
#endif
  }
#endif
};

typedef struct _CAN_DRV_CONFIG {// Compile time configuration of driver structure
  uint32_t FILTER_STD_NUM;      // Number of Standard IDs
  uint32_t FILTER_EXT_NUM;      // Number of Extended IDs
  uint32_t RX_FIFO0_ELEM_NUM;   // Number of Rx FIFO0 elements
  uint32_t RX_FIFO1_ELEM_NUM;   // Number of Rx FIFO1 elements
  uint32_t TX_FIFO_ELEM_NUM;    // Number of Tx FIFO elements
  uint32_t RX_FIFO_OBJ_NUM;     // Number of Rx FIFO objects
  uint32_t RX_FIFO_OBJ_IDX_MAX; // Maximum index of Rx FIFO object (0=no max index, 1..n=index+1)
  uint32_t RX_BUF_OBJ_NUM;      // Number of Rx buffer objects
  uint32_t RX_BUF_OBJ_IDX_MAX;  // Maximum index of Rx buffer object (0=no max index, 1..n=index+1)
  uint32_t RX_TOT_OBJ_NUM;      // Number of all Rx FIFO and buffer objects
  uint32_t TX_FIFO_OBJ_NUM;     // Number of Tx FIFO objects
  uint32_t TX_FIFO_OBJ_IDX_MAX; // Maximum index of Tx FIFO object (0=no max index, 1..n=index+1)
  uint32_t TX_BUF_OBJ_NUM;      // Number of Tx buffer objects
  uint32_t TX_BUF_OBJ_IDX_MAX;  // Maximum index of Tx buffer object (0=no max index, 1..n=index+1)
  uint32_t TX_TOT_OBJ_NUM;      // Number of all Tx FIFO and buffer objects
  uint32_t TOT_OBJ_NUM;         // Number of all Rx + Tx objects
} CAN_DRV_CONFIG_t;

typedef struct _CAN_RAM_PTR {   // RAM pointers to areas of filters and buffers structure
  uint32_t *PTR_STD_ID;         // Pointer to Standard IDs RAM
  uint32_t *PTR_EXT_ID;         // Pointer to Extended IDs RAM
  uint32_t *PTR_RX_FIFO0;       // Pointer to Rx FIFO0 RAM
  uint32_t *PTR_RX_FIFO1;       // Pointer to Rx FIFO1 RAM
  uint32_t *PTR_RX_BUFFER;      // Pointer to Rx buffers RAM
  uint32_t *PTR_TX_EVENT_FIFO;  // Pointer to Tx Event FIFO RAM
  uint32_t *PTR_TX_BUFFER;      // Pointer to Tx buffers RAM
} CAN_RAM_PTR_t;

#if (CAN1_USED == 1)
// Statically allocated RAM for filters and buffers for CAN1 controller
static uint32_t         CAN1_RAM[CAN1_TOT_RAM] __MEMORY_AT(0x4000AC00);
#endif
#if (CAN2_USED == 1)
// Statically allocated RAM for filters and buffers for CAN2 controller
static uint32_t         CAN2_RAM[CAN2_TOT_RAM] __MEMORY_AT(0x4000C000);
#endif

// Encodings of DLC and LEC to speed up their usage
static const uint8_t    CAN_DLC_ENCODE [16] =  { 0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U };
static const uint8_t    CAN_LEC_ENCODE [ 8] =  { ARM_CAN_LEC_NO_ERROR,  ARM_CAN_LEC_STUFF_ERROR, ARM_CAN_LEC_FORM_ERROR, ARM_CAN_LEC_ACK_ERROR,
                                                 ARM_CAN_LEC_BIT_ERROR, ARM_CAN_LEC_BIT_ERROR  , ARM_CAN_LEC_CRC_ERROR,  ARM_CAN_LEC_NO_ERROR };

// Compile time configuration of drivers
static const CAN_DRV_CONFIG_t CAN_DRV_CONFIG [ 2] = {
#if (CAN1_USED == 1)
                                               { CAN1_FILTER_STD_NUM,
                                                 CAN1_FILTER_EXT_NUM,
                                                 CAN1_RX_FIFO0_ELEM_NUM,
                                                 CAN1_RX_FIFO1_ELEM_NUM,
                                                 CAN1_TX_FIFO_ELEM_NUM,
                                                 CAN1_RX_FIFO_OBJ_NUM,
                                               ( CAN1_RX_FIFO_OBJ_NUM *(CAN1_RX_FIFO_OBJ_NUM != 0U)),
                                                 CAN1_RX_BUF_OBJ_NUM,
                                               ((CAN1_RX_FIFO_OBJ_NUM + CAN1_RX_BUF_OBJ_NUM)*(CAN1_RX_BUF_OBJ_NUM != 0U)),
                                                (CAN1_RX_FIFO_OBJ_NUM + CAN1_RX_BUF_OBJ_NUM),
                                                 CAN1_TX_FIFO_OBJ_NUM,
                                               ((CAN1_RX_FIFO_OBJ_NUM + CAN1_RX_BUF_OBJ_NUM + CAN1_TX_FIFO_OBJ_NUM)*(CAN1_TX_FIFO_OBJ_NUM != 0U)),
                                                 CAN1_TX_BUF_OBJ_NUM,
                                               ((CAN1_RX_FIFO_OBJ_NUM + CAN1_RX_BUF_OBJ_NUM + CAN1_TX_FIFO_OBJ_NUM + CAN1_TX_BUF_OBJ_NUM)*(CAN1_TX_BUF_OBJ_NUM != 0U)),
                                                (CAN1_TX_FIFO_OBJ_NUM + CAN1_TX_BUF_OBJ_NUM),
                                                 CAN1_TOT_OBJ_NUM },
#else
                                               { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
#endif
#if (CAN2_USED == 1)
                                               { CAN2_FILTER_STD_NUM,
                                                 CAN2_FILTER_EXT_NUM,
                                                 CAN2_RX_FIFO0_ELEM_NUM,
                                                 CAN2_RX_FIFO1_ELEM_NUM,
                                                 CAN2_TX_FIFO_ELEM_NUM,
                                                 CAN2_RX_FIFO_OBJ_NUM,
                                               ( CAN2_RX_FIFO_OBJ_NUM *(CAN2_RX_FIFO_OBJ_NUM != 0U)),
                                                 CAN2_RX_BUF_OBJ_NUM,
                                               ((CAN2_RX_FIFO_OBJ_NUM + CAN2_RX_BUF_OBJ_NUM)*(CAN2_RX_BUF_OBJ_NUM != 0U)),
                                                (CAN2_RX_FIFO_OBJ_NUM + CAN2_RX_BUF_OBJ_NUM),
                                                 CAN2_TX_FIFO_OBJ_NUM,
                                               ((CAN2_RX_FIFO_OBJ_NUM + CAN2_RX_BUF_OBJ_NUM + CAN2_TX_FIFO_OBJ_NUM)*(CAN2_TX_FIFO_OBJ_NUM != 0U)),
                                                 CAN2_TX_BUF_OBJ_NUM,
                                               ((CAN2_RX_FIFO_OBJ_NUM + CAN2_RX_BUF_OBJ_NUM + CAN2_TX_FIFO_OBJ_NUM + CAN2_TX_BUF_OBJ_NUM)*(CAN2_TX_BUF_OBJ_NUM != 0U)),
                                                (CAN2_TX_FIFO_OBJ_NUM + CAN2_TX_BUF_OBJ_NUM),
                                                 CAN2_TOT_OBJ_NUM },
#else
                                               { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
#endif
                                              };

// Compile time RAM pointers to areas of filters and buffers
static const CAN_RAM_PTR_t CAN_RAM_PTR    [ 2] = {
#if (CAN1_USED == 1)
                                               { &CAN1_RAM[CAN1_RAM_OFS_STD_ID],
                                                 &CAN1_RAM[CAN1_RAM_OFS_EXT_ID],
                                                 &CAN1_RAM[CAN1_RAM_OFS_RX_FIFO0],
                                                 &CAN1_RAM[CAN1_RAM_OFS_RX_FIFO1],
                                                 &CAN1_RAM[CAN1_RAM_OFS_RX_BUFFER],
                                                 &CAN1_RAM[CAN1_RAM_OFS_TX_EVENT_FIFO],
                                                 &CAN1_RAM[CAN1_RAM_OFS_TX_BUFFER] },
#else
                                               { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
#endif
#if (CAN2_USED == 1)
                                               { &CAN2_RAM[CAN2_RAM_OFS_STD_ID],
                                                 &CAN2_RAM[CAN2_RAM_OFS_EXT_ID],
                                                 &CAN2_RAM[CAN2_RAM_OFS_RX_FIFO0],
                                                 &CAN2_RAM[CAN2_RAM_OFS_RX_FIFO1],
                                                 &CAN2_RAM[CAN2_RAM_OFS_RX_BUFFER],
                                                 &CAN2_RAM[CAN2_RAM_OFS_TX_EVENT_FIFO],
                                                 &CAN2_RAM[CAN2_RAM_OFS_TX_BUFFER] }
#else
                                               { NULL, NULL, NULL, NULL, NULL, NULL, NULL }
#endif
                                              };

// Const variables for simplifying access to CAN instances
static FDCAN_GlobalTypeDef * const ptr_regs_CANx[2]  =  { FDCAN1, FDCAN2 };
static FDCAN_HandleTypeDef * const ptr_hndl_CANx[2]  =  {
#if (CAN1_USED == 1)
                                                         &hfdcan1,
#else
                                                          NULL,
#endif
#if (CAN2_USED == 1)
                                                         &hfdcan2
#else
                                                          NULL
#endif
                                                        };
static const IRQn_Type             FDCANx_IRQn[2][2] = {{ FDCAN1_IT0_IRQn, FDCAN1_IT1_IRQn }, { FDCAN2_IT0_IRQn, FDCAN2_IT1_IRQn }};

// Runtime variables
static uint8_t                     can_driver_initialized[CAN_CTRL_NUM] = {
#if (CAN_CTRL_NUM > 1)
                                                                            0U,
#endif
#if (CAN_CTRL_NUM > 0)
                                                                            0U
#endif
                                                                          };
static uint8_t                     can_driver_powered    [CAN_CTRL_NUM];
static uint8_t                     can_driver_fd_mode    [CAN_CTRL_NUM];
static uint8_t                     can_obj_cfg_access    [CAN_CTRL_NUM];
static uint32_t                    can_obj_rx_cfg        [CAN_CTRL_NUM][2];
static uint32_t                    can_obj_tx_cfg        [CAN_CTRL_NUM];
static uint32_t                    can_tx_obj_active     [CAN_CTRL_NUM];
static uint8_t                     can_filter_std_num    [CAN_CTRL_NUM];
static uint8_t                     can_filter_ext_num    [CAN_CTRL_NUM];
static ARM_CAN_SignalUnitEvent_t   CAN_SignalUnitEvent   [CAN_CTRL_NUM];
static ARM_CAN_SignalObjectEvent_t CAN_SignalObjectEvent [CAN_CTRL_NUM];


// CAN Driver Functions

/**
  \fn          ARM_DRIVER_VERSION CAN_GetVersion (void)
  \brief       Get driver version.
  \return      ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION CAN_GetVersion (void) { return can_driver_version; }

/**
  \fn          ARM_CAN_CAPABILITIES CAN1_GetCapabilities (void)
  \fn          ARM_CAN_CAPABILITIES CAN2_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      ARM_CAN_CAPABILITIES
*/
#if (CAN1_USED == 1)
static ARM_CAN_CAPABILITIES CAN1_GetCapabilities (void) { return can_driver_capabilities[0U]; }
#endif
#if (CAN2_USED == 1)
static ARM_CAN_CAPABILITIES CAN2_GetCapabilities (void) { return can_driver_capabilities[1U]; }
#endif
/**
  \fn          int32_t CANx_Initialize (ARM_CAN_SignalUnitEvent_t   cb_unit_event,
                                        ARM_CAN_SignalObjectEvent_t cb_object_event,
                                        uint8_t                     x)
  \brief       Initialize CAN interface and register signal (callback) functions.
  \param[in]   cb_unit_event   Pointer to ARM_CAN_SignalUnitEvent callback function
  \param[in]   cb_object_event Pointer to ARM_CAN_SignalObjectEvent callback function
  \param[in]   x               Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_Initialize (ARM_CAN_SignalUnitEvent_t   cb_unit_event,
                                ARM_CAN_SignalObjectEvent_t cb_object_event,
                                uint8_t                     x) {

  if (x >= CAN_CTRL_NUM)               { return ARM_DRIVER_ERROR; }
  if (can_driver_initialized[x] != 0U) { return ARM_DRIVER_OK;    }

  CAN_SignalUnitEvent  [x] = cb_unit_event;
  CAN_SignalObjectEvent[x] = cb_object_event;

  ptr_hndl_CANx[x]->Instance = ptr_regs_CANx[x];

  can_driver_powered[x] = 0U;

  can_driver_initialized[x] = 1U;

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_Initialize (ARM_CAN_SignalUnitEvent_t cb_unit_event, ARM_CAN_SignalObjectEvent_t cb_object_event) { return CANx_Initialize (cb_unit_event, cb_object_event, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_Initialize (ARM_CAN_SignalUnitEvent_t cb_unit_event, ARM_CAN_SignalObjectEvent_t cb_object_event) { return CANx_Initialize (cb_unit_event, cb_object_event, 1U); }
#endif

/**
  \fn          int32_t CANx_Uninitialize (uint8_t x)
  \brief       De-initialize CAN interface.
  \param[in]   x      Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_Uninitialize (uint8_t x) {

  if (x >= CAN_CTRL_NUM) { return ARM_DRIVER_ERROR; }

  can_driver_initialized[x] = 0U;

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_Uninitialize (void) { return CANx_Uninitialize (0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_Uninitialize (void) { return CANx_Uninitialize (1U); }
#endif

/**
  \fn          int32_t CANx_PowerControl (ARM_POWER_STATE state, uint8_t x)
  \brief       Control CAN interface power.
  \param[in]   state  Power state
                 - ARM_POWER_OFF :  power off: no operation possible
                 - ARM_POWER_LOW :  low power mode: retain state, detect and signal wake-up events
                 - ARM_POWER_FULL : power on: full operation at maximum performance
  \param[in]   x      Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_PowerControl (ARM_POWER_STATE state, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;

  if (x >= CAN_CTRL_NUM) { return ARM_DRIVER_ERROR; }

  ptr_CAN = ptr_regs_CANx[x];

  switch (state) {
    case ARM_POWER_OFF:
      can_driver_powered[x] = 0U;
      ptr_CAN->IE           = 0U;
      ptr_CAN->IR           = 0xFFFFFFFFU;
      ptr_CAN->ILE          = 0U;
      NVIC_DisableIRQ     (FDCANx_IRQn[x][0]);
      NVIC_ClearPendingIRQ(FDCANx_IRQn[x][0]);
      NVIC_DisableIRQ     (FDCANx_IRQn[x][1]);
      NVIC_ClearPendingIRQ(FDCANx_IRQn[x][1]);
      if (ptr_hndl_CANx[x]->Instance != NULL) {
        HAL_FDCAN_MspDeInit(ptr_hndl_CANx[x]);
      }
      break;

    case ARM_POWER_FULL:
      if (can_driver_initialized[x] == 0U) { return ARM_DRIVER_ERROR; }
      if (can_driver_powered[x]     != 0U) { return ARM_DRIVER_OK;    }

      if (ptr_hndl_CANx[x]->Instance != NULL) {
        HAL_FDCAN_MspInit(ptr_hndl_CANx[x]);
      }

      // Clear runtime variables
      can_driver_fd_mode[x] = 0U;
      can_obj_cfg_access[x] = 0U;
      can_obj_rx_cfg[x][0]  = 0U;
      can_obj_rx_cfg[x][1]  = 0U;
      can_obj_tx_cfg[x]     = 0U;
      can_tx_obj_active[x]  = 0U;
      can_filter_std_num[x] = 0U;
      can_filter_ext_num[x] = 0U;

      // Set peripheral to initialization and configuration change enable to set RAM pointers
      ptr_CAN->CCCR = FDCAN_CCCR_INIT; while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;

      switch (x) {
#if (CAN1_USED == 1)
        case 0:
          memset((void *)CAN1_RAM, 0U, sizeof(CAN1_RAM));
          break;
#endif
#if (CAN2_USED == 1)
        case 1:
          memset((void *)CAN2_RAM, 0U, sizeof(CAN2_RAM));
          break;
#endif
        default:
          return ARM_DRIVER_ERROR;
      }

      // Configure memory areas for filters and Rx/Tx buffers, FIFOs for Rx and Tx are not used
      // Rx Buffers are configured for 64 byte data field to accommodate maximum CAN FD message
      // Tx Buffers are configured for 64 byte data field to accommodate maximum CAN FD message
      ptr_CAN->SIDFC = (FDCAN_SIDFC_LSS   &  (CAN_DRV_CONFIG[x].FILTER_STD_NUM    << FDCAN_SIDFC_LSS_Pos) ) |
                       (FDCAN_SIDFC_FLSSA & ((uint32_t)(CAN_RAM_PTR[x].PTR_STD_ID)        - CAN_RAM_BASE) ) ;
      ptr_CAN->XIDFC = (FDCAN_XIDFC_LSE   &  (CAN_DRV_CONFIG[x].FILTER_EXT_NUM    << FDCAN_XIDFC_LSE_Pos) ) |
                       (FDCAN_XIDFC_FLESA & ((uint32_t)(CAN_RAM_PTR[x].PTR_EXT_ID)        - CAN_RAM_BASE) ) ;
      ptr_CAN->RXF0C = (FDCAN_RXF0C_F0S   &  (CAN_DRV_CONFIG[x].RX_FIFO0_ELEM_NUM << FDCAN_RXF0C_F0S_Pos) ) |
                       (FDCAN_RXF0C_F0SA  & ((uint32_t)(CAN_RAM_PTR[x].PTR_RX_FIFO0)      - CAN_RAM_BASE) ) ;
      ptr_CAN->RXF1C = (FDCAN_RXF1C_F1S   &  (CAN_DRV_CONFIG[x].RX_FIFO1_ELEM_NUM << FDCAN_RXF1C_F1S_Pos) ) |
                       (0xFFFCU           & ((uint32_t)(CAN_RAM_PTR[x].PTR_RX_FIFO1)      - CAN_RAM_BASE) ) ;
      ptr_CAN->RXBC  = (0xFFFCU           & ((uint32_t)(CAN_RAM_PTR[x].PTR_RX_BUFFER)     - CAN_RAM_BASE) ) ;
      ptr_CAN->RXESC = (FDCAN_RXESC_F0DS  &  (7U                                  << FDCAN_RXESC_F0DS_Pos)) |
                       (FDCAN_RXESC_F1DS  &  (7U                                  << FDCAN_RXESC_F1DS_Pos)) |
                       (FDCAN_RXESC_RBDS  &  (7U                                  << FDCAN_RXESC_RBDS_Pos)) ;
      ptr_CAN->TXEFC = (FDCAN_TXEFC_EFS   &  (CAN_DRV_CONFIG[x].TX_FIFO_ELEM_NUM  << FDCAN_TXEFC_EFS_Pos )) |
                       (0xFFFCU           & ((uint32_t)(CAN_RAM_PTR[x].PTR_TX_EVENT_FIFO) - CAN_RAM_BASE) ) ;
      ptr_CAN->TXBC  = (FDCAN_TXBC_NDTB   &  (CAN_DRV_CONFIG[x].TX_BUF_OBJ_NUM    << FDCAN_TXBC_NDTB_Pos) ) |
                       (FDCAN_TXBC_TFQS   &  (CAN_DRV_CONFIG[x].TX_FIFO_ELEM_NUM  << FDCAN_TXBC_TFQS_Pos) ) |
                       (0xFFFCU           & ((uint32_t)(CAN_RAM_PTR[x].PTR_TX_BUFFER)     - CAN_RAM_BASE) ) ;
      ptr_CAN->TXESC = (FDCAN_TXESC_TBDS  &  (7U                                  << FDCAN_TXESC_TBDS_Pos)) ;

      // Reject non-matching frames
      ptr_CAN->GFC   = (FDCAN_GFC_ANFE & (3U << FDCAN_GFC_ANFE_Pos)) | (FDCAN_GFC_ANFS & (3U << FDCAN_GFC_ANFS_Pos));

      // Route all interrupt sources to m_can_int0 and enable it
      ptr_CAN->ILS = 0U;
      ptr_CAN->ILE = FDCAN_ILE_EINT0;

      // Enable relevant interrupts
      ptr_CAN->IR  = 0xFFFFFFFFU;
      ptr_CAN->IE  = FDCAN_IE_TCE | FDCAN_IE_DRXE | FDCAN_IE_EPE | FDCAN_IE_EWE | FDCAN_IE_BOE;

      can_driver_powered[x] = 1U;

      NVIC_ClearPendingIRQ(FDCANx_IRQn[x][0]);
      if ((CAN_SignalUnitEvent[x] != NULL) || (CAN_SignalObjectEvent[x] != NULL)) {
        NVIC_EnableIRQ    (FDCANx_IRQn[x][0]);
      }
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_PowerControl (ARM_POWER_STATE state) { return CANx_PowerControl (state, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_PowerControl (ARM_POWER_STATE state) { return CANx_PowerControl (state, 1U); }
#endif

/**
  \fn          uint32_t CAN_GetClock (void)
  \brief       Retrieve CAN base clock frequency.
  \return      base clock frequency
*/
static uint32_t CAN_GetClock (void) {
  PLL1_ClocksTypeDef pll1_clocks;
  PLL2_ClocksTypeDef pll2_clocks;
  uint32_t           src, clk;

  src = __HAL_RCC_GET_FDCAN_SOURCE();
  switch (src) {
    case RCC_FDCANCLKSOURCE_HSE:
      clk = HSE_VALUE;
      break;
    case RCC_FDCANCLKSOURCE_PLL:
      HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
      clk = pll1_clocks.PLL1_Q_Frequency;
      break;
    case RCC_FDCANCLKSOURCE_PLL2:
      HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
      clk = pll2_clocks.PLL2_Q_Frequency;
      break;
    default:
      clk = 0U;
      break;
  }

  return clk;
}


/**
  \fn          int32_t CANx_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments, uint8_t x)
  \brief       Set bitrate for CAN interface.
  \param[in]   select       Bitrate selection
                 - ARM_CAN_BITRATE_NOMINAL : nominal (flexible data-rate arbitration) bitrate
                 - ARM_CAN_BITRATE_FD_DATA : flexible data-rate data bitrate
  \param[in]   bitrate      Bitrate
  \param[in]   bit_segments Bit segments settings
  \param[in]   x            Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t             cccr, sjw, prop_seg, phase_seg1, phase_seg2, pclk, brp, brp_max, tq_num;

  if (x >= CAN_CTRL_NUM)                                                            { return ARM_DRIVER_ERROR;               }
  if ((select != ARM_CAN_BITRATE_NOMINAL) && (select != ARM_CAN_BITRATE_FD_DATA)  ) { return ARM_CAN_INVALID_BITRATE_SELECT; }
  if (can_driver_powered[x] == 0U)                                                  { return ARM_DRIVER_ERROR;               }

  prop_seg   = (bit_segments & ARM_CAN_BIT_PROP_SEG_Msk  ) >> ARM_CAN_BIT_PROP_SEG_Pos;
  phase_seg1 = (bit_segments & ARM_CAN_BIT_PHASE_SEG1_Msk) >> ARM_CAN_BIT_PHASE_SEG1_Pos;
  phase_seg2 = (bit_segments & ARM_CAN_BIT_PHASE_SEG2_Msk) >> ARM_CAN_BIT_PHASE_SEG2_Pos;
  sjw        = (bit_segments & ARM_CAN_BIT_SJW_Msk       ) >> ARM_CAN_BIT_SJW_Pos;

  if (select == ARM_CAN_BITRATE_NOMINAL) {
    if (((prop_seg + phase_seg1) < 2U) || ((prop_seg + phase_seg1) > 256U)) { return ARM_CAN_INVALID_BIT_PROP_SEG;   }
    if (( phase_seg2             < 1U) || ( phase_seg2             > 128U)) { return ARM_CAN_INVALID_BIT_PHASE_SEG2; }
    if (( sjw                    < 1U) || ( sjw                    > 128U)) { return ARM_CAN_INVALID_BIT_SJW;        }
    brp_max = 512U;
  } else {
    if (((prop_seg + phase_seg1) < 2U) || ((prop_seg + phase_seg1) >  32U)) { return ARM_CAN_INVALID_BIT_PROP_SEG;   }
    if (( phase_seg2             < 1U) || ( phase_seg2             >  16U)) { return ARM_CAN_INVALID_BIT_PHASE_SEG2; }
    if (( sjw                    < 1U) || ( sjw                    >   8U)) { return ARM_CAN_INVALID_BIT_SJW;        }
    brp_max = 32U;
  }

  tq_num = 1U + prop_seg + phase_seg1 + phase_seg2;
  pclk   = CAN_GetClock ();           if (pclk == 0U)    { return ARM_DRIVER_ERROR;        }
  brp    = pclk / (tq_num * bitrate); if (brp > brp_max) { return ARM_CAN_INVALID_BITRATE; }
  if (pclk > (brp * tq_num * bitrate)) {
    if ((((pclk - (brp * tq_num * bitrate)) * 1024U) / pclk) > CAN_CLOCK_TOLERANCE) { return ARM_CAN_INVALID_BITRATE; }
  } else if (pclk < (brp * tq_num * bitrate)) {
    if (((((brp * tq_num * bitrate) - pclk) * 1024U) / pclk) > CAN_CLOCK_TOLERANCE) { return ARM_CAN_INVALID_BITRATE; }
  }

  ptr_CAN = ptr_regs_CANx[x];

  cccr = ptr_CAN->CCCR;
  if ((cccr & (FDCAN_CCCR_CCE | FDCAN_CCCR_INIT)) != (FDCAN_CCCR_CCE | FDCAN_CCCR_INIT)) {
    ptr_CAN->CCCR = FDCAN_CCCR_CCE  |   // Configuration change enable
                    FDCAN_CCCR_INIT ;   // Initialization
    while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
  }

  if (select == ARM_CAN_BITRATE_NOMINAL) {
    ptr_CAN->NBTP  = ((brp - 1U) << 16) | ((prop_seg + phase_seg1 - 1U) << 8) |  (phase_seg2 - 1U)       | ((sjw - 1U) << 25);
  } else {
    ptr_CAN->DBTP = ((brp - 1U) << 16) | ((prop_seg + phase_seg1 - 1U) << 8) | ((phase_seg2 - 1U) << 4) |  (sjw - 1U);
  }
  ptr_CAN->CCCR = cccr;

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments) { return CANx_SetBitrate (select, bitrate, bit_segments, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments) { return CANx_SetBitrate (select, bitrate, bit_segments, 1U); }
#endif

/**
  \fn          int32_t CANx_SetMode (ARM_CAN_MODE mode, uint8_t x)
  \brief       Set operating mode for CAN interface.
  \param[in]   mode   Operating mode
                 - ARM_CAN_MODE_INITIALIZATION :    initialization mode
                 - ARM_CAN_MODE_NORMAL :            normal operation mode
                 - ARM_CAN_MODE_RESTRICTED :        restricted operation mode
                 - ARM_CAN_MODE_MONITOR :           bus monitoring mode
                 - ARM_CAN_MODE_LOOPBACK_INTERNAL : loopback internal mode
                 - ARM_CAN_MODE_LOOPBACK_EXTERNAL : loopback external mode
  \param[in]   x      Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_SetMode (ARM_CAN_MODE mode, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t             event;

  if (x >= CAN_CTRL_NUM)           { return ARM_DRIVER_ERROR; }
  if (can_driver_powered[x] == 0U) { return ARM_DRIVER_ERROR; }

  ptr_CAN = ptr_regs_CANx[x];

  event = 0U;
  switch (mode) {
    case ARM_CAN_MODE_INITIALIZATION:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST;
      ptr_CAN->TEST = 0U;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      event         = ARM_CAN_EVENT_UNIT_BUS_OFF;
      break;
    case ARM_CAN_MODE_NORMAL:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST;
      ptr_CAN->TEST = 0U;
      ptr_CAN->CCCR = 0U;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      event         = ARM_CAN_EVENT_UNIT_ACTIVE;
      break;
    case ARM_CAN_MODE_RESTRICTED:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST | FDCAN_CCCR_ASM;
      ptr_CAN->TEST = 0U;
      ptr_CAN->CCCR =                                                      FDCAN_CCCR_ASM;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      event         = ARM_CAN_EVENT_UNIT_ACTIVE;
      break;
    case ARM_CAN_MODE_MONITOR:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST | FDCAN_CCCR_MON;
      ptr_CAN->TEST = 0U;
      ptr_CAN->CCCR =                                                      FDCAN_CCCR_MON;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      event         = ARM_CAN_EVENT_UNIT_PASSIVE;
      break;
    case ARM_CAN_MODE_LOOPBACK_INTERNAL:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST | FDCAN_CCCR_MON;
      ptr_CAN->TEST = FDCAN_TEST_LBCK;
      ptr_CAN->CCCR =                                    FDCAN_CCCR_TEST | FDCAN_CCCR_MON;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      event         = ARM_CAN_EVENT_UNIT_PASSIVE;
      break;
    case ARM_CAN_MODE_LOOPBACK_EXTERNAL:
      ptr_CAN->CCCR = FDCAN_CCCR_INIT;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST;
      ptr_CAN->TEST = FDCAN_TEST_LBCK;
      ptr_CAN->CCCR =                                    FDCAN_CCCR_TEST;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      event         = ARM_CAN_EVENT_UNIT_ACTIVE;
      break;
    default:
      return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((CAN_SignalUnitEvent[x] != NULL) && (event != 0U)) { CAN_SignalUnitEvent[x](event); }

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_SetMode (ARM_CAN_MODE mode) { return CANx_SetMode (mode, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_SetMode (ARM_CAN_MODE mode) { return CANx_SetMode (mode, 1U); }
#endif

/**
  \fn          ARM_CAN_OBJ_CAPABILITIES CANx_ObjectGetCapabilities (uint32_t obj_idx, uint8_t x)
  \brief       Retrieve capabilities of an object.
  \param[in]   obj_idx  Object index
  \param[in]   x        Controller number (0..1)
  \return      ARM_CAN_OBJ_CAPABILITIES
*/
static ARM_CAN_OBJ_CAPABILITIES CANx_ObjectGetCapabilities (uint32_t obj_idx, uint8_t x) {
  ARM_CAN_OBJ_CAPABILITIES obj_cap_null;

  if ((x >= CAN_CTRL_NUM) || (obj_idx >= CAN_DRV_CONFIG[x].TOT_OBJ_NUM)) {
    memset (&obj_cap_null, 0U, sizeof(ARM_CAN_OBJ_CAPABILITIES));
    return obj_cap_null;
  }

  if         (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
    if (obj_idx == 0U) {
      return can_object_capabilities_rx_fifo0[x];
    } else {
      return can_object_capabilities_rx_fifo1[x];
    }
  } else if  (obj_idx < CAN_DRV_CONFIG[x].RX_BUF_OBJ_IDX_MAX) {
    return can_object_capabilities_rx_buffer[x];
  } else if  (obj_idx < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX) {
    return can_object_capabilities_tx_fifo[x];
  } else {
    return can_object_capabilities_tx_buffer[x];
  }
}
#if (CAN1_USED == 1)
static ARM_CAN_OBJ_CAPABILITIES CAN1_ObjectGetCapabilities (uint32_t obj_idx) { return CANx_ObjectGetCapabilities (obj_idx, 0U); }
#endif
#if (CAN2_USED == 1)
static ARM_CAN_OBJ_CAPABILITIES CAN2_ObjectGetCapabilities (uint32_t obj_idx) { return CANx_ObjectGetCapabilities (obj_idx, 1U); }
#endif

/**
  \fn          int32_t CANx_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg, uint8_t x)
  \brief       Add or remove filter for message reception.
  \param[in]   obj_idx      Object index of object that filter should be or is assigned to
  \param[in]   operation    Operation on filter
                 - ARM_CAN_FILTER_ID_EXACT_ADD :       add    exact id filter
                 - ARM_CAN_FILTER_ID_EXACT_REMOVE :    remove exact id filter
                 - ARM_CAN_FILTER_ID_RANGE_ADD :       add    range id filter
                 - ARM_CAN_FILTER_ID_RANGE_REMOVE :    remove range id filter
                 - ARM_CAN_FILTER_ID_MASKABLE_ADD :    add    maskable id filter
                 - ARM_CAN_FILTER_ID_MASKABLE_REMOVE : remove maskable id filter
  \param[in]   id           ID or start of ID range (depending on filter type)
  \param[in]   arg          Mask or end of ID range (depending on filter type)
  \param[in]   x            Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg, uint8_t x) {
  uint32_t *ptr_filter_RAM;
  uint32_t  i, val0, val1;

  if (x >= CAN_CTRL_NUM)                           { return ARM_DRIVER_ERROR; }
  if (obj_idx >= CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) { return ARM_DRIVER_ERROR; }
  if (can_driver_powered[x] == 0U)                 { return ARM_DRIVER_ERROR; }

  switch (operation) {
    case ARM_CAN_FILTER_ID_MASKABLE_ADD:                // Add
    case ARM_CAN_FILTER_ID_RANGE_ADD:
      // Maskable and range filters are only supported on Rx FIFO
      if (obj_idx >= CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) { return ARM_DRIVER_ERROR; }
    case ARM_CAN_FILTER_ID_EXACT_ADD:
      if ((id & ARM_CAN_ID_IDE_Msk) != 0U) {            // Extended Identifier
        if (can_filter_ext_num[x] >= CAN_DRV_CONFIG[x].FILTER_EXT_NUM) { return ARM_DRIVER_ERROR; }
        switch (operation) {
          case ARM_CAN_FILTER_ID_EXACT_ADD:
            if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
              // If Rx FIFO is used for reception on this object
              ptr_filter_RAM = CAN_RAM_PTR[x].PTR_EXT_ID;
              for (i = 0U; i < can_filter_ext_num[x]; i++) {
                if ((((ptr_filter_RAM[i*2U+1U] >> 30) & 3U) == 1U)             &&       // If Dual ID entry
                    (((ptr_filter_RAM[i*2U]    >> 29) & 7U) == (obj_idx + 1U))) {       // If same FIFO is assigned
                  if        ((ptr_filter_RAM[i*2U]    & 0x1FFFFFFFU) == 0U) {
                    // First entry in Dual ID entry is unused
                    ptr_filter_RAM[i*2U]    |= (id  & 0x1FFFFFFFU);
                    break;
                  } else if ((ptr_filter_RAM[i*2U+1U] & 0x1FFFFFFFU) == 0U) {
                    // Second entry in Dual ID entry is unused
                    ptr_filter_RAM[i*2U+1U] |= (id  & 0x1FFFFFFFU);
                    break;
                  }
                }
              }
              if (i == can_filter_ext_num[x]) {
                // No free entry in Dual ID entries, add new Dual ID entry
                ptr_filter_RAM    = (uint32_t *)(CAN_RAM_PTR[x].PTR_EXT_ID + can_filter_ext_num[x] * 2U);
                ptr_filter_RAM[0] = ((obj_idx + 1U) << 29) | (id  & 0x1FFFFFFFU);
                ptr_filter_RAM[1] = (1U             << 30);
                can_filter_ext_num[x]++;
              }
            } else {
              // If Rx Buffer is used for reception on this object
              ptr_filter_RAM    = (uint32_t *)(CAN_RAM_PTR[x].PTR_EXT_ID + can_filter_ext_num[x] * 2U);
              ptr_filter_RAM[0] = (7U             << 29) | (id & 0x1FFFFFFFU);
              ptr_filter_RAM[1] =  obj_idx - CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM;
              can_filter_ext_num[x]++;
            }
            break;
          case ARM_CAN_FILTER_ID_MASKABLE_ADD:
            ptr_filter_RAM    =   (uint32_t *)(CAN_RAM_PTR[x].PTR_EXT_ID + can_filter_ext_num[x] * 2U);
            ptr_filter_RAM[0] =   ((obj_idx + 1U) << 29) | (id  & 0x1FFFFFFFU);
            ptr_filter_RAM[1] =   (2U             << 30) | (arg & 0x1FFFFFFFU);
            can_filter_ext_num[x]++;
            break;
          case ARM_CAN_FILTER_ID_RANGE_ADD:
            ptr_filter_RAM    =   (uint32_t *)(CAN_RAM_PTR[x].PTR_EXT_ID + can_filter_ext_num[x] * 2U);
            ptr_filter_RAM[0] =   ((obj_idx + 1U) << 29) | (id  & 0x1FFFFFFFU);
            ptr_filter_RAM[1] =                            (arg & 0x1FFFFFFFU);
            can_filter_ext_num[x]++;
            break;
          case ARM_CAN_FILTER_ID_EXACT_REMOVE:
          case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:
          case ARM_CAN_FILTER_ID_RANGE_REMOVE:
          default:
            return ARM_DRIVER_ERROR;
        }
      } else {                                          // Standard Identifier
        if (can_filter_std_num[x] >= CAN_DRV_CONFIG[x].FILTER_STD_NUM) { return ARM_DRIVER_ERROR; }
        switch (operation) {
          case ARM_CAN_FILTER_ID_EXACT_ADD:
            if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
              // If Rx FIFO is used for reception on this object
              ptr_filter_RAM = CAN_RAM_PTR[x].PTR_STD_ID;
              for (i = 0U; i < can_filter_std_num[x]; i++) {
                if ((((ptr_filter_RAM[i] >> 30) & 3U) == 1U)             &&     // If Dual ID entry
                    (((ptr_filter_RAM[i] >> 27) & 7U) == (obj_idx + 1U))) {     // If same FIFO is assigned
                  if        ((ptr_filter_RAM[i] & (0x7FFU << 16)) == 0U) {
                    // First entry in Dual ID entry is unused
                    ptr_filter_RAM[i] |= ((id & 0x7FFU) << 16);
                    break;
                  } else if ((ptr_filter_RAM[i] & (0x7FFU)      ) == 0U) {
                    // Second entry in Dual ID entry is unused
                    ptr_filter_RAM[i] |=  (id & 0x7FFU);
                    break;
                  }
                }
              }
              if (i == can_filter_std_num[x]) {
                // No free entry in Dual ID entries, add new Dual ID entry
                ptr_filter_RAM    = (uint32_t *)(CAN_RAM_PTR[x].PTR_STD_ID + can_filter_std_num[x]);
                ptr_filter_RAM[0] = (1U << 30) | ((obj_idx + 1U) << 27) | ((id & 0x7FFU) << 16);
                can_filter_std_num[x]++;
              }
            } else {
              // If Rx Buffer is used for reception on this object
              ptr_filter_RAM    =   (uint32_t *)(CAN_RAM_PTR[x].PTR_STD_ID + can_filter_std_num[x]);
              ptr_filter_RAM[0] =               (7U            << 27) | ((id & 0x7FFU) << 16) | (obj_idx - CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM);
              can_filter_std_num[x]++;
            }
            break;
          case ARM_CAN_FILTER_ID_MASKABLE_ADD:
            ptr_filter_RAM    =     (uint32_t *)(CAN_RAM_PTR[x].PTR_STD_ID + can_filter_std_num[x]);
            ptr_filter_RAM[0] =     (2U << 30) | ((obj_idx + 1U) << 27) | ((id & 0x7FFU) << 16) | (arg & 0x7FFU);
            can_filter_std_num[x]++;
            break;
          case ARM_CAN_FILTER_ID_RANGE_ADD:
            ptr_filter_RAM    =     (uint32_t *)(CAN_RAM_PTR[x].PTR_STD_ID + can_filter_std_num[x]);
            ptr_filter_RAM[0] =                  ((obj_idx + 1U) << 27) | ((id & 0x7FFU) << 16) | (arg & 0x7FFU);
            can_filter_std_num[x]++;
            break;
          case ARM_CAN_FILTER_ID_EXACT_REMOVE:
          case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:
          case ARM_CAN_FILTER_ID_RANGE_REMOVE:
          default:
            return ARM_DRIVER_ERROR;
        }
      }
      break;
    case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:             // Remove
    case ARM_CAN_FILTER_ID_RANGE_REMOVE:
      // Maskable and range filters are only supported on Rx FIFO
      if (obj_idx >= CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) { return ARM_DRIVER_ERROR; }
    case ARM_CAN_FILTER_ID_EXACT_REMOVE:
      if ((id & ARM_CAN_ID_IDE_Msk) != 0U) {            // Extended Identifier
        if (can_filter_ext_num[x] == 0U) { return ARM_DRIVER_ERROR; }
        switch (operation) {
          case ARM_CAN_FILTER_ID_EXACT_REMOVE:
            if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
              // If Rx FIFO is used for reception on this object
              ptr_filter_RAM = CAN_RAM_PTR[x].PTR_EXT_ID;
              for (i = 0U; i < can_filter_ext_num[x]; i++) {
                if ((((ptr_filter_RAM[i*2U+1U] >> 30) & 3U) == 1U)             &&       // If Dual ID entry
                    (((ptr_filter_RAM[i*2U]    >> 29) & 7U) == (obj_idx + 1U))) {       // If same FIFO is assigned
                  if        ((ptr_filter_RAM[i*2U]    & 0x1FFFFFFFU) == (id & 0x1FFFFFFFU)) {
                    // First entry in Dual ID entry is the one to be removed
                    ptr_filter_RAM[i*2U] &= ~0x1FFFFFFFU;
                    if ((ptr_filter_RAM[i*2U+1U] & 0x1FFFFFFFU) == 0U) {
                      // Dual ID entry is free it can be removed
                      // Shift all remaining Extended ID filter entries 1 entry (64 bits) towards start of table
                      for (; i < can_filter_ext_num[x]; i++) {
                        ptr_filter_RAM[i*2U   ] = ptr_filter_RAM[i*2U+2U];
                        ptr_filter_RAM[i*2U+1U] = ptr_filter_RAM[i*2U+3U];
                      }
                      can_filter_ext_num[x]--;
                    }
                    return ARM_DRIVER_OK;
                  } else if ((ptr_filter_RAM[i*2U+1U] & 0x1FFFFFFFU) == (id & 0x1FFFFFFFU)) {
                    // Second entry in Dual ID entry is the one to be removed
                    ptr_filter_RAM[i*2U+1U] &= ~0x1FFFFFFFU;
                    if ((ptr_filter_RAM[i*2U]    & 0x1FFFFFFFU) == 0U) {
                      // Dual ID entry is free it can be removed
                      // Shift all remaining Extended ID filter entries 1 entry (64 bits) towards start of table
                      for (; i < can_filter_ext_num[x]; i++) {
                        ptr_filter_RAM[i*2U   ] = ptr_filter_RAM[i*2U+2U];
                        ptr_filter_RAM[i*2U+1U] = ptr_filter_RAM[i*2U+3U];
                      }
                      can_filter_ext_num[x]--;
                    }
                    return ARM_DRIVER_OK;
                  }
                }
              }
              return ARM_DRIVER_ERROR;
            } else {
              // If Rx Buffer is used for reception on this object
              val0 = (7U             << 29) | (id & 0x1FFFFFFFU);
              val1 =  obj_idx - CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM;
            }
            break;
          case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:
            val0 =   ((obj_idx + 1U) << 29) | (id  & 0x1FFFFFFFU);
            val1 =   (2U             << 30) | (arg & 0x1FFFFFFFU);
            break;
          case ARM_CAN_FILTER_ID_RANGE_REMOVE:
            val0 =   ((obj_idx + 1U) << 29) | (id  & 0x1FFFFFFFU);
            val1 =                            (arg & 0x1FFFFFFFU);
            break;
          case ARM_CAN_FILTER_ID_EXACT_ADD:
          case ARM_CAN_FILTER_ID_MASKABLE_ADD:
          case ARM_CAN_FILTER_ID_RANGE_ADD:
          default:
            return ARM_DRIVER_ERROR;
        }
        ptr_filter_RAM = CAN_RAM_PTR[x].PTR_EXT_ID;
        for (i = 0U; i < can_filter_ext_num[x]; i++) {
          if ((ptr_filter_RAM[i*2U]    == val0) &&
              (ptr_filter_RAM[i*2U+1U] == val1)) {
            // Entry to be removed was found
            // Shift all remaining Extended ID filter entries 1 entry (64 bits) towards start of table
            for (; i < can_filter_ext_num[x]; i++) {
              ptr_filter_RAM[i*2U   ] = ptr_filter_RAM[i*2U+2U];
              ptr_filter_RAM[i*2U+1U] = ptr_filter_RAM[i*2U+3U];
            }
            can_filter_ext_num[x]--;
            break;
          }
        }
      } else {                                          // Standard Identifier
        if (can_filter_std_num[x] == 0U) { return ARM_DRIVER_ERROR; }
        switch (operation) {
          case ARM_CAN_FILTER_ID_EXACT_REMOVE:
            if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
              // If Rx FIFO is used for reception on this object
              ptr_filter_RAM = CAN_RAM_PTR[x].PTR_STD_ID;
              for (i = 0U; i < can_filter_std_num[x]; i++) {
                if ((((ptr_filter_RAM[i] >> 30) & 3U) == 1U)             &&     // If Dual ID entry
                    (((ptr_filter_RAM[i] >> 27) & 7U) == (obj_idx + 1U))) {     // If same FIFO is assigned
                  if        (((ptr_filter_RAM[i] >> 16) & 0x7FFU) == (id & 0x7FFU)) {
                    // First entry in Dual ID entry is the one to be removed
                    ptr_filter_RAM[i] &= ~(0x7FFU << 16);
                    if ((ptr_filter_RAM[i] & 0x7FFU) == 0U) {
                      // Dual ID entry is free it can be removed
                      // Shift all remaining Standard ID filter entries 1 entry (32 bits) towards start of table
                      for (; i < can_filter_std_num[x]; i++) {
                        ptr_filter_RAM[i] = ptr_filter_RAM[i+1U];
                      }
                      can_filter_std_num[x]--;
                    }
                    return ARM_DRIVER_OK;
                  } else if ( (ptr_filter_RAM[i]        & 0x7FFU) == (id & 0x7FFU)) {
                    // Second entry in Dual ID entry is the one to be removed
                    ptr_filter_RAM[i] &= ~0x7FFU;
                    if ((ptr_filter_RAM[i] & (0x7FFU << 16)) == 0U) {
                      // Dual ID entry is free it can be removed
                      // Shift all remaining Standard ID filter entries 1 entry (32 bits) towards start of table
                      for (; i < can_filter_std_num[x]; i++) {
                        ptr_filter_RAM[i] = ptr_filter_RAM[i+1U];
                      }
                      can_filter_std_num[x]--;
                    }
                    return ARM_DRIVER_OK;
                  }
                }
              }
              return ARM_DRIVER_ERROR;
            } else {
              // If Rx Buffer is used for reception on this object
              val0 =               (7U            << 27) | ((id & 0x7FFU) << 16) | (obj_idx - CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM);
            }
            break;
          case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:
            val0 =   (2U << 30) | ((obj_idx + 1U) << 27) | ((id & 0x7FFU) << 16) | (arg & 0x7FFU);
            break;
          case ARM_CAN_FILTER_ID_RANGE_REMOVE:
            val0 =                ((obj_idx + 1U) << 27) | ((id & 0x7FFU) << 16) | (arg & 0x7FFU);
            break;
          case ARM_CAN_FILTER_ID_EXACT_ADD:
          case ARM_CAN_FILTER_ID_MASKABLE_ADD:
          case ARM_CAN_FILTER_ID_RANGE_ADD:
          default:
            return ARM_DRIVER_ERROR;
        }
        ptr_filter_RAM = CAN_RAM_PTR[x].PTR_STD_ID;
        for (i = 0U; i < can_filter_std_num[x]; i++) {
          if (ptr_filter_RAM[i] == val0) {
            // Entry to be removed was found
            // Shift all remaining Standard ID filter entries 1 entry (32 bits) towards start of table
            for (; i < can_filter_std_num[x]; i++) {
              ptr_filter_RAM[i] = ptr_filter_RAM[i+1U];
            }
            can_filter_std_num[x]--;
            break;
          }
        }
      }
      break;
    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg) { return CANx_ObjectSetFilter (obj_idx, operation, id, arg, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg) { return CANx_ObjectSetFilter (obj_idx, operation, id, arg, 1U); }
#endif

/**
  \fn          int32_t CANx_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg, uint8_t x)
  \brief       Configure object.
  \param[in]   obj_idx  Object index
  \param[in]   obj_cfg  Object configuration state
                 - ARM_CAN_OBJ_INACTIVE :       deactivate object
                 - ARM_CAN_OBJ_RX :             configure object for reception
                 - ARM_CAN_OBJ_TX :             configure object for transmission
                 - ARM_CAN_OBJ_RX_RTR_TX_DATA : configure object that on RTR reception automatically transmits Data Frame
                 - ARM_CAN_OBJ_TX_RTR_RX_DATA : configure object that transmits RTR and automatically receives Data Frame
  \param[in]   x        Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t             msk;

  if (x >= CAN_CTRL_NUM)                        { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_DRV_CONFIG[x].TOT_OBJ_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U)              { return ARM_DRIVER_ERROR;           }
  if (can_obj_cfg_access[x] != 0U)              { return ARM_DRIVER_ERROR_BUSY;      }
  can_obj_cfg_access[x] = 1U;

  ptr_CAN = ptr_regs_CANx[x];

  switch (obj_cfg) {
    case ARM_CAN_OBJ_INACTIVE:
      if (obj_idx >= CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) {        // If this is Tx object
        msk = 1U << ((obj_idx - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) & 0x1FU);
        can_obj_tx_cfg[x]    &= ~msk;
        if        (obj_idx < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX) {
          // If disabling Tx FIFO
          ptr_CAN->IE &= ~FDCAN_IE_TEFNE;
        } else if (obj_idx < CAN_DRV_CONFIG[x].TX_BUF_OBJ_IDX_MAX) {
          // If disabling Tx Buffer
          ptr_CAN->TXBTIE &= ~msk;
        }
      } else {                                          // If this is Rx object
        can_obj_rx_cfg[x][obj_idx >> 5] &= ~(1U << (obj_idx & 0x1FU));
        if        (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
          // If disabling Rx FIFO
          if (obj_idx == 0U) {
            ptr_CAN->IE &= ~FDCAN_IE_RF0NE;
          } else {
            ptr_CAN->IE &= ~FDCAN_IE_RF1NE;
          }
        } else if (obj_idx < CAN_DRV_CONFIG[x].RX_BUF_OBJ_IDX_MAX) {
          // If disabling Rx Buffer
        }
      }
      break;
    case ARM_CAN_OBJ_TX:
      if (obj_idx < CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) { can_obj_cfg_access[x] = 0U; return ARM_DRIVER_ERROR_PARAMETER; }
      msk = 1U << ((obj_idx - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) & 0x1FU);
      can_obj_tx_cfg[x]    |= msk;
      if        (obj_idx < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX) {
        // If enabling Tx FIFO
        ptr_CAN->IE |=  FDCAN_IE_TEFNE;
      } else if (obj_idx < CAN_DRV_CONFIG[x].TX_BUF_OBJ_IDX_MAX) {
        // If enabling Tx Buffer
        ptr_CAN->TXBTIE   |=  msk;
      }
      break;
    case ARM_CAN_OBJ_RX:
      if (obj_idx >= CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) { can_obj_cfg_access[x] = 0U; return ARM_DRIVER_ERROR_PARAMETER; }
      if        (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
        // If enabling Rx FIFO
        if (obj_idx == 0U) {
          ptr_CAN->IE |=  FDCAN_IE_RF0NE;
        } else {
          ptr_CAN->IE |=  FDCAN_IE_RF1NE;
        }
      }
      can_obj_rx_cfg[x][obj_idx >> 5] |= 1U << (obj_idx & 0x1FU);
      break;
    case ARM_CAN_OBJ_RX_RTR_TX_DATA:
    case ARM_CAN_OBJ_TX_RTR_RX_DATA:
    default:
      return ARM_DRIVER_ERROR;
  }
  can_obj_cfg_access[x] = 0U;

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg) { return CANx_ObjectConfigure (obj_idx, obj_cfg, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg) { return CANx_ObjectConfigure (obj_idx, obj_cfg, 1U); }
#endif

/**
  \fn          int32_t CANx_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size, uint8_t x)
  \brief       Send message on CAN bus.
  \param[in]   obj_idx  Object index
  \param[in]   msg_info Pointer to CAN message information
  \param[in]   data     Pointer to data buffer
  \param[in]   size     Number of data bytes to send
  \param[in]   x        Controller number (0..1)
  \return      value >= 0  number of data bytes accepted to send
  \return      value < 0   execution status
*/
static int32_t CANx_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t            *ptr_buffer_RAM;
  uint32_t            *dest32;
  const uint32_t      *src32;
  const uint8_t       *src8;
  uint32_t             t0, i, tx_obj_idx, buf_ofs_idx, val;

  if (x >= CAN_CTRL_NUM)                                                                          { return ARM_DRIVER_ERROR;           }
  if ((obj_idx < CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) || (obj_idx >= CAN_DRV_CONFIG[x].TOT_OBJ_NUM)) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U)                                                                { return ARM_DRIVER_ERROR;           }
  tx_obj_idx = (obj_idx - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) & 0x1FU;
  if ((can_obj_tx_cfg[x] & (1U << tx_obj_idx)) == 0U)                                             { return ARM_DRIVER_ERROR;           }

  ptr_CAN = ptr_regs_CANx[x];

  if (obj_idx < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX) {
    // Send using Tx FIFO
    buf_ofs_idx = ((ptr_CAN->TXFQS & FDCAN_TXFQS_TFQPI_Msk) >> FDCAN_TXFQS_TFQPI_Pos);
  } else {
    // Send using Tx Buffer
    buf_ofs_idx = tx_obj_idx;
  }
  ptr_buffer_RAM = CAN_RAM_PTR[x].PTR_TX_BUFFER + (buf_ofs_idx * 18U);

  if ((msg_info->id & ARM_CAN_ID_IDE_Msk) == 0U) {      // Standard ID
    t0 = (msg_info->id & 0x7FFU) << 18;
  } else {                                              // Extended ID
    t0 = (msg_info->id & 0x1FFFFFFFU) | (1U << 30);
  }

  if (msg_info->rtr != 0U) {                            // RTR message
    t0 |= 1U << 29;
    ptr_buffer_RAM[0] = t0;
  } else {                                              // Data message
    ptr_buffer_RAM[0] = t0;
    if (can_driver_fd_mode[x] != 0U) {                  // If FD mode
      ptr_buffer_RAM[1] = (1U << 21) | (1U << 20);      // Set FDF (FD Format) and BRS (Bit Rate Switching)
      if (size > 64U) { size = 64U; }
      for (i = 0U; i < 16U; i++) {
        if (CAN_DLC_ENCODE[i] >= size) {
          ptr_buffer_RAM[1] |= i << 16;
          break;
        }
      }
    } else {                                            // If not FD mode
      if (size > 8U) { size = 8U; }
      ptr_buffer_RAM[1] = ((uint32_t)size) << 16;
    }
    if (obj_idx < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX) {
      // Send using Tx FIFO
      ptr_buffer_RAM[1] |= (1U << 23);
    }

    dest32 = ptr_buffer_RAM + 2U;
    if (((uint32_t)data & 3U) == 0U) {
      // If source is 4-byte aligned copy integer number of words with
      // aligned read from source
      src32 = (const uint32_t *)data;
      for (i = 0U; i < size / 4U; i++) {
        dest32[i] = src32[i];
      }
    } else {
      // If source is not 4-byte aligned copy integer number of words with
      // unaligned read from source
      src8 = data;
      for (i = 0U; i < size / 4U; i++) {
        dest32[i] = __UNALIGNED_UINT32_READ(src8);
        src8 += 4U;
      }
    }
    if ((size & 3U) != 0U) {
      // Copy remaining up to 3 bytes (read byte by byte and write as single word)
      dest32 = ptr_buffer_RAM + 2U + ((size & 0xFFFFFFFCU) / 4U);
      src8   = data +                 (size & 0xFFFFFFFCU);
      val    = 0U;
      if ((size & 3U) > 2U) {
        val |= (uint32_t)src8[2] << 16;
      }
      if ((size & 3U) > 1U) {
        val |= (uint32_t)src8[1] << 8;
      }
      if ((size & 3U) > 0U) {
        val |= (uint32_t)src8[0];
      }
      dest32[0] = val;
    }
  }

  can_tx_obj_active[x] |= 1U << tx_obj_idx;

  ptr_CAN->TXBAR = 1U << buf_ofs_idx;

  return ((int32_t)size);
}
#if (CAN1_USED == 1)
static int32_t CAN1_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size) { return CANx_MessageSend (obj_idx, msg_info, data, size, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size) { return CANx_MessageSend (obj_idx, msg_info, data, size, 1U); }
#endif

/**
  \fn          int32_t CANx_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size, uint8_t x)
  \brief       Read message received on CAN bus.
  \param[in]   obj_idx  Object index
  \param[out]  msg_info Pointer to read CAN message information
  \param[out]  data     Pointer to data buffer for read data
  \param[in]   size     Maximum number of data bytes to read
  \param[in]   x        Controller number (0..1)
  \return      value >= 0  number of data bytes read
  \return      value < 0   execution status
*/
static int32_t CANx_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t            *ptr_buffer_RAM;
  uint32_t            *src32;
  uint32_t            *dest32;
  uint8_t             *dest8;
  uint32_t             r, i, val;

  if (x >= CAN_CTRL_NUM)                                                   { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM)                         { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U)                                         { return ARM_DRIVER_ERROR;           }
  if ((can_obj_rx_cfg[x][obj_idx >> 5] & (1U << (obj_idx & 0x1FU))) == 0U) { return ARM_DRIVER_ERROR;           }

  ptr_CAN = ptr_regs_CANx[x];

  if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
    // Read message received with Rx FIFO
    if (obj_idx == 0U) {
      ptr_buffer_RAM = CAN_RAM_PTR[x].PTR_RX_FIFO0 + (((ptr_CAN->RXF0S & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos) * 18U);
    } else {
      ptr_buffer_RAM = CAN_RAM_PTR[x].PTR_RX_FIFO1 + (((ptr_CAN->RXF1S & FDCAN_RXF1S_F1GI_Msk) >> FDCAN_RXF1S_F1GI_Pos) * 18U);
    }
  } else {
    // Read message received with Rx Buffer
    ptr_buffer_RAM = CAN_RAM_PTR[x].PTR_RX_BUFFER + ((obj_idx - CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM) * 18U);
  }

  r = ptr_buffer_RAM[0];                // Read R0
  msg_info->esi = (r >> 31);            // Get Error Status Indicator flag
  if ((r & (1U << 30)) != 0U) {         // If Extended ID
    msg_info->id = ((r      ) & 0x1FFFFFFFU) | ARM_CAN_ID_IDE_Msk;
  } else {                              // If Standard ID
    msg_info->id = ((r >> 18) & 0x7FFU);
  }
  if ((r & (1U << 29)) != 0U) {         // If RTR message
    msg_info->rtr = 1U;
  } else {                              // If Data message
    msg_info->rtr = 0U;
  }
  r = ptr_buffer_RAM[1];                // Read R1
  msg_info->edl = (r >> 21) &  1U;      // Get Extended Data Length flag
  msg_info->brs = (r >> 20) &  1U;      // Get Baud Rate Switching flag
  msg_info->dlc = (r >> 16) & 15U;      // Get Data Length Code
  if (msg_info->rtr == 0U) {            // If Data message read data bytes
    if ((msg_info->edl == 1U) && (can_driver_fd_mode[x] != 0U)) {       // If FD mode and Extended Data Length bit active
      if (size > CAN_DLC_ENCODE[msg_info->dlc]) {
        size = CAN_DLC_ENCODE[msg_info->dlc];
      }
    } else {
      if (size > msg_info->dlc) {       // If not FD mode or not Extended Data Length bit active
        size = msg_info->dlc;
      }
      if (size > 8U) {
        size = 8U;
      }
    }
  } else {
    size = 0U;
  }

  if (size > 0U) {
    src32 = ptr_buffer_RAM + 2U;
    if (((uint32_t)data & 3U) == 0U) {
      // If destination is 4-byte aligned copy integer number of words with
      // aligned read from source
      dest32 = (uint32_t *)data;
      for (i = 0U; i < size / 4U; i++) {
        dest32[i] = src32[i];
      }
    } else {
      // If destination is not 4-byte aligned copy integer number of words with
      // unaligned write to destination
      dest8 = data;
      for (i = 0U; i < size / 4U; i++) {
        __UNALIGNED_UINT32_WRITE(dest8, src32[i]);
        dest8 += 4U;
      }
    }
    if ((size & 3U) != 0U) {
      // Copy remaining up to 3 bytes (read word and write byte by byte)
      src32 = ptr_buffer_RAM + 2U + ((size & 0xFFFFFFFCU) / 4U);
      dest8 = data +                 (size & 0xFFFFFFFCU);
      val   = src32[0];
      if ((size & 3U) > 0U) {
        dest8[0] = (uint8_t)(val);
      }
      if ((size & 3U) > 1U) {
        dest8[1] = (uint8_t)(val >> 8);
      }
      if ((size & 3U) > 2U) {
        dest8[2] = (uint8_t)(val >> 16);
      }
    }
  }

  if (obj_idx < CAN_DRV_CONFIG[x].RX_FIFO_OBJ_IDX_MAX) {
    // Acknowledge message received with Rx FIFO
    if (obj_idx == 0U) {
      ptr_CAN->RXF0A = (ptr_CAN->RXF0S & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos;
    } else {
      ptr_CAN->RXF1A = (ptr_CAN->RXF1S & FDCAN_RXF1S_F1GI_Msk) >> FDCAN_RXF1S_F1GI_Pos;
    }
  } else {
    // Clear new data flag and release Rx buffer
    obj_idx -= CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM;
    if (obj_idx < 32U) {
      ptr_CAN->NDAT1 = 1 <<  obj_idx;
    } else {
      ptr_CAN->NDAT2 = 1 << (obj_idx - 32U);
    }
  }

  return ((int32_t)size);
}
#if (CAN1_USED == 1)
static int32_t CAN1_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size) { return CANx_MessageRead (obj_idx, msg_info, data, size, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size) { return CANx_MessageRead (obj_idx, msg_info, data, size, 1U); }
#endif

/**
  \fn          int32_t CANx_Control (uint32_t control, uint32_t arg, uint8_t x)
  \brief       Control CAN interface.
  \param[in]   control  Operation
                 - ARM_CAN_SET_FD_MODE :            set FD operation mode
                 - ARM_CAN_ABORT_MESSAGE_SEND :     abort sending of CAN message
                 - ARM_CAN_CONTROL_RETRANSMISSION : enable/disable automatic retransmission
                 - ARM_CAN_SET_TRANSCEIVER_DELAY :  set transceiver delay
  \param[in]   arg      Argument of operation
  \param[in]   x        Controller number (0..1)
  \return      execution status
*/
static int32_t CANx_Control (uint32_t control, uint32_t arg, uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t             cccr;

  if (x >= CAN_CTRL_NUM)           { return ARM_DRIVER_ERROR; }
  if (can_driver_powered[x] == 0U) { return ARM_DRIVER_ERROR; }

  ptr_CAN = ptr_regs_CANx[x];

  switch (control & ARM_CAN_CONTROL_Msk) {
    case ARM_CAN_ABORT_MESSAGE_SEND:
      if ((arg < CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) || (arg >= CAN_DRV_CONFIG[x].TOT_OBJ_NUM))     { return ARM_DRIVER_ERROR_PARAMETER; }
      if ( arg < CAN_DRV_CONFIG[x].TX_FIFO_OBJ_IDX_MAX)                                           { return ARM_DRIVER_ERROR;           }
      if ((can_obj_tx_cfg[x] & (1U << ((arg - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM) & 0x1FU))) == 0U) { return ARM_DRIVER_OK;              }
      if (can_driver_powered[x] == 0U)                                                            { return ARM_DRIVER_ERROR;           }
      ptr_CAN->TXBCR        =  (1U << (arg - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM - CAN_DRV_CONFIG[x].TX_FIFO_OBJ_NUM));
      can_tx_obj_active[x] &= ~(1U << (arg - CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM));
      break;
    case ARM_CAN_CONTROL_RETRANSMISSION:
      switch (arg) {
        case 0:                                 // Disable automatic retransmission
          ptr_CAN->CCCR |=  FDCAN_CCCR_DAR;
          break;
        case 1:                                 // Enable automatic retransmission
          ptr_CAN->CCCR &= ~FDCAN_CCCR_DAR;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
    case ARM_CAN_SET_FD_MODE:
      cccr = ptr_CAN->CCCR;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) != 0U);
      cccr |= FDCAN_CCCR_INIT;
      ptr_CAN->CCCR = cccr;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 0U);
      cccr |= FDCAN_CCCR_CCE;
      ptr_CAN->CCCR = cccr;
      switch (arg) {
        case 0:                         // Disable FD mode
          cccr &= ~(FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE | FDCAN_CCCR_NISO);
          break;
        case 1:                         // Enable FD mode with bit rate switching
          cccr |=   FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE;
#if (CAN_NON_ISO_EN == 1U)
          cccr |=   FDCAN_CCCR_NISO;    // Enable non-ISO mode
#endif
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      ptr_CAN->CCCR = cccr;
      cccr &= ~(FDCAN_CCCR_INIT | FDCAN_CCCR_CCE);
      ptr_CAN->CCCR = cccr;
      while ((ptr_CAN->CCCR & FDCAN_CCCR_INIT) == 1U);
      if (arg == 1U) {
        can_driver_fd_mode[x] = 1U;
      } else {
        can_driver_fd_mode[x] = 0U;
      }
      break;
    case ARM_CAN_SET_TRANSCEIVER_DELAY:
      switch (arg) {
        case 0:                                 // Disable transceiver delay
          ptr_CAN->DBTP |=  FDCAN_DBTP_TDC;
          break;
        case 1:                                 // Enable transceiver delay
          ptr_CAN->DBTP &= ~FDCAN_DBTP_TDC;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}
#if (CAN1_USED == 1)
static int32_t CAN1_Control (uint32_t control, uint32_t arg) { return CANx_Control (control, arg, 0U); }
#endif
#if (CAN2_USED == 1)
static int32_t CAN2_Control (uint32_t control, uint32_t arg) { return CANx_Control (control, arg, 1U); }
#endif

/**
  \fn          ARM_CAN_STATUS CANx_GetStatus (uint8_t x)
  \brief       Get CAN status.
  \param[in]   x      Controller number (0..1)
  \return      CAN status ARM_CAN_STATUS
*/
static ARM_CAN_STATUS CANx_GetStatus (uint8_t x) {
           FDCAN_GlobalTypeDef *ptr_CAN;
  volatile ARM_CAN_STATUS       can_status;
           uint32_t             psr, ecr;

  if ((x >= CAN_CTRL_NUM) || (can_driver_powered[x] == 0U)) {
    *((volatile uint32_t *)&can_status) = 0U;
    return can_status;
  }

  ptr_CAN = ptr_regs_CANx[x];
  psr     = ptr_CAN->PSR;
  ecr     = ptr_CAN->ECR;

  if       ((psr & FDCAN_PSR_BO) != 0U) { can_status.unit_state = ARM_CAN_UNIT_STATE_INACTIVE; }
  else if  ((psr & FDCAN_PSR_EP) != 0U) { can_status.unit_state = ARM_CAN_UNIT_STATE_PASSIVE;  }
  else                                  { can_status.unit_state = ARM_CAN_UNIT_STATE_ACTIVE;   }

  can_status.last_error_code = CAN_LEC_ENCODE[psr & FDCAN_PSR_LEC_Msk];
  can_status.tx_error_count  = (uint8_t)(ecr & FDCAN_ECR_TEC_Msk);
  if ((ecr & FDCAN_ECR_RP) != 0U) {
    can_status.rx_error_count = 128U;
  } else {
    can_status.rx_error_count = (uint8_t)((ecr & FDCAN_ECR_REC_Msk) >> FDCAN_ECR_REC_Pos);
  }

  return can_status;
}
#if (CAN1_USED == 1)
static ARM_CAN_STATUS CAN1_GetStatus (void) { return CANx_GetStatus (0U); }
#endif
#if (CAN2_USED == 1)
static ARM_CAN_STATUS CAN2_GetStatus (void) { return CANx_GetStatus (1U); }
#endif


/**
  \fn          void CANx_Handler (void)
  \brief       CANx Interrupt Routine (IRQ).
*/
static void CANx_Handler (uint8_t x) {
  FDCAN_GlobalTypeDef *ptr_CAN;
  uint32_t             ir, psr, ndat1, ndat2, txbto, msk, obj_idx;
  volatile uint32_t    get_idx, put_idx;

  ptr_CAN = ptr_regs_CANx[x];

  ir      = ptr_CAN->IR & ptr_CAN->IE;
  ndat1   = ptr_CAN->NDAT1;
  ndat2   = ptr_CAN->NDAT2;
  txbto   = ptr_CAN->TXBTO;
  ptr_CAN->IR = ir;

  if (CAN_SignalUnitEvent[x] != NULL) {
    psr = ptr_CAN->PSR;
    if (ir & (FDCAN_IR_BO | FDCAN_IR_EP)) {             // Handle error interrupt
      if      (((ir & FDCAN_IR_BO) != 0U) && ((psr & FDCAN_PSR_BO) != 0U)) { CAN_SignalUnitEvent[x](ARM_CAN_EVENT_UNIT_BUS_OFF); }
      else if (((ir & FDCAN_IR_EP) != 0U) && ((psr & FDCAN_PSR_EP) != 0U)) { CAN_SignalUnitEvent[x](ARM_CAN_EVENT_UNIT_PASSIVE); }
      else                                                                 { CAN_SignalUnitEvent[x](ARM_CAN_EVENT_UNIT_ACTIVE);  }
    }
    if (((ir & FDCAN_IR_EW) != 0U) && ((psr & FDCAN_PSR_EW) != 0U))        { CAN_SignalUnitEvent[x](ARM_CAN_EVENT_UNIT_WARNING); }
  }

  if (CAN_SignalObjectEvent[x] != NULL) {
                                                        // Handle transmit events
    if (ir & FDCAN_IR_TEFN) {                           // If message was transmitted using Tx FIFO
      get_idx = (ptr_CAN->TXEFS & FDCAN_TXEFS_EFGI_Msk) >> FDCAN_TXEFS_EFGI_Pos;
      put_idx = (ptr_CAN->TXEFS & FDCAN_TXEFS_EFPI_Msk) >> FDCAN_TXEFS_EFPI_Pos;
      do {
        ptr_CAN->TXEFA = get_idx;
        CAN_SignalObjectEvent[x](CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM, ARM_CAN_EVENT_SEND_COMPLETE);
        get_idx = (ptr_CAN->TXEFS & FDCAN_TXEFS_EFGI_Msk) >> FDCAN_TXEFS_EFGI_Pos;
        put_idx = (ptr_CAN->TXEFS & FDCAN_TXEFS_EFPI_Msk) >> FDCAN_TXEFS_EFPI_Pos;
      } while (get_idx != put_idx);
    }
    if (ir & FDCAN_IR_TC) {                             // If message was transmitted using dedicated Tx buffer
      for (obj_idx = 0U, msk = 1U; (txbto != 0U) || (msk != 0U); obj_idx++, msk <<= 1U) {
        if ((txbto & msk) != 0U) {
          txbto &= ~msk;
          CAN_SignalObjectEvent[x](obj_idx + CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM, ARM_CAN_EVENT_SEND_COMPLETE);
        }
      }
    }

                                                        // Handle receive events
    if (ir & FDCAN_IR_RF0N) {                           // If message was received to Rx FIFO0
      CAN_SignalObjectEvent[x](0U, ARM_CAN_EVENT_RECEIVE);
    }
    if (ir & FDCAN_IR_RF1N) {                           // If message was received to Rx FIFO1
      CAN_SignalObjectEvent[x](1U, ARM_CAN_EVENT_RECEIVE);
    }
    if (ir & FDCAN_IR_DRX) {                            // If message was received to dedicated Rx buffer
      if ((ndat1 | ndat2) != 0U) {
        for (obj_idx = 0U, msk = 1U; (ndat1 != 0U) && (msk != 0U); obj_idx++, msk <<= 1U) {
          if ((ndat1 & msk) != 0U) {
            ndat1 &= ~msk;
            CAN_SignalObjectEvent[x](obj_idx + CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM, ARM_CAN_EVENT_RECEIVE);
          }
        }
        if (CAN_DRV_CONFIG[x].RX_TOT_OBJ_NUM > 32U) {
          for (obj_idx = 32U, msk = 1U; (ndat2 != 0U) && (msk != 0U); obj_idx++, msk <<= 1U) {
            if ((ndat2 & msk) != 0U) {
              ndat2 &= ~msk;
              CAN_SignalObjectEvent[x](obj_idx + CAN_DRV_CONFIG[x].RX_FIFO_OBJ_NUM, ARM_CAN_EVENT_RECEIVE);
            }
          }
        }
      }
    }
  }
}

/**
  \fn          void MCAN1_Handler (void)
  \brief       CAN1 Interrupt Routine (IRQ).
*/
#if (CAN1_USED == 1)
void FDCAN1_IT0_IRQHandler (void) { CANx_Handler (0); }
#endif

/**
  \fn          void MCAN2_Handler (void)
  \brief       CAN2 Interrupt Routine (IRQ).
*/
#if (CAN2_USED == 1)
void FDCAN2_IT0_IRQHandler (void) { CANx_Handler (1); }
#endif


#if (CAN1_USED == 1)
ARM_DRIVER_CAN Driver_CAN1 = {
  CAN_GetVersion,
  CAN1_GetCapabilities,
  CAN1_Initialize,
  CAN1_Uninitialize,
  CAN1_PowerControl,
  CAN_GetClock,
  CAN1_SetBitrate,
  CAN1_SetMode,
  CAN1_ObjectGetCapabilities,
  CAN1_ObjectSetFilter,
  CAN1_ObjectConfigure,
  CAN1_MessageSend,
  CAN1_MessageRead,
  CAN1_Control,
  CAN1_GetStatus
};
#endif

#if (CAN2_USED == 1)
ARM_DRIVER_CAN Driver_CAN2 = {
  CAN_GetVersion,
  CAN2_GetCapabilities,
  CAN2_Initialize,
  CAN2_Uninitialize,
  CAN2_PowerControl,
  CAN_GetClock,
  CAN2_SetBitrate,
  CAN2_SetMode,
  CAN2_ObjectGetCapabilities,
  CAN2_ObjectSetFilter,
  CAN2_ObjectConfigure,
  CAN2_MessageSend,
  CAN2_MessageRead,
  CAN2_Control,
  CAN2_GetStatus
};
#endif

/*! \endcond */
