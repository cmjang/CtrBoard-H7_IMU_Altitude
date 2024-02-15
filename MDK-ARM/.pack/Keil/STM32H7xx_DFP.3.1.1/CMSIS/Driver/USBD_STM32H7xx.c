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
 * Driver:       Driver_USBD0/1
 *
 * Configured:   via CubeMX
 * Project:      USB High/Full/Low-Speed Device Driver for ST STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_usbd USB Device

# Revision History

- Version 1.6
  - Made variable state volatile (solved potential LTO problems)
- Version 1.5
  - Added support for VBUS sensing
- Version 1.4
  - Updated documentation
  - Corrected USBD_GetFrameNumber function
- Version 1.3
  - Updated to be compliant with HAL drivers version 1.5.0
- Version 1.2
  - Updated USBD_EndpointConfigure function to check that maximum packet
  - size requested fits into configured FIFO (compile time configured)
- Version 1.1
  - Corrected transmitted count for non-control IN endpoints
- Version 1.0
  - Initial release

# Capabilities

This driver supports USB **High**, **Full** and **Low** speeds.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_USBD0    | USB_OTG_FS
Driver_USBD1    | USB_OTG_HS

# Configuration

## STM32CubeMX

The USB Device driver requires:
  - **USB peripheral clock** at **48 MHz**
  - **USB_OTG_FS** or **USB_OTG_HS** peripheral configured as **Device_Only**
  - **USB DP**, **DM** and **VBUS pins** or **USB HS ULPI pins**

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral  | Mode                         | Description
:----------:|:----------------------------:|:-----------
USB_OTG_FS  | Device_Only                  | USB Controller
PA9         | Input mode                   | USB VBUS Pin
PA11        | Alternate Function Push Pull | USB DM Pin
PA12        | Alternate Function Push Pull | USB DP Pin
USB_OTG_HS  | Device_Only                  | USB Controller
PA3         | Alternate Function Push Pull | ULPI D0 Pin
PA5         | Alternate Function Push Pull | ULPI CLK Pin
PB0         | Alternate Function Push Pull | ULPI D1 Pin
PB1         | Alternate Function Push Pull | ULPI D2 Pin
PB5         | Alternate Function Push Pull | ULPI D7 Pin
PB10        | Alternate Function Push Pull | ULPI D3 Pin
PB11        | Alternate Function Push Pull | ULPI D4 Pin
PB12        | Alternate Function Push Pull | ULPI D5 Pin
PB13        | Alternate Function Push Pull | ULPI D6 Pin
PC0         | Alternate Function Push Pull | ULPI STP Pin
PH4         | Alternate Function Push Pull | ULPI NXT Pin
PI11        | Alternate Function Push Pull | ULPI DIR Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PA9      | **USB_OTG_FS_VBUS** (required for VBUS sensing)
        PA11     | **USB_OTG_FS_DM**
        PA12     | **USB_OTG_FS_DP**
        PA3      | **USB_OTG_HS_ULPI_D0**
        PA5      | **USB_OTG_HS_ULPI_CLK**
        PB0      | **USB_OTG_HS_ULPI_D1**
        PB1      | **USB_OTG_HS_ULPI_D2**
        PB5      | **USB_OTG_HS_ULPI_D7**
        PB10     | **USB_OTG_HS_ULPI_D3**
        PB11     | **USB_OTG_HS_ULPI_D4**
        PB12     | **USB_OTG_HS_ULPI_D5**
        PB13     | **USB_OTG_HS_ULPI_D6**
        PC0      | **USB_OTG_HS_ULPI_STP**
        PH4      | **USB_OTG_HS_ULPI_NXT**
        PI11     | **USB_OTG_HS_ULPI_DIR**
       \n

  2. Under **Categories**: **Connectivity** select **USB_OTG_FS**:

     __Mode__:
       - Mode: **Device_Only**
       - Activate_VBUS: **VBUS sensing**
       - Activate_SOF: unchecked

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - GPIO Settings:
            Pin Name | Signal on Pin       | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PA9      | USB_OTG_FS_VBUS     | n/a          | n/a          | Input mode                    | No pull-up and no..| n/a          | n/a       | .
            PA11     | USB_OTG_FS_DM       | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PA12     | USB_OTG_FS_DP       | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
       \n

  3. Under **Categories**: **Connectivity** select **USB_OTG_HS**:

     __Mode__:
       - External Phy: **Device_Only**
       - Internal FS Phy: Disable
       - Activate_SOF: unchecked
       - Activate_VBUS: Disable

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - GPIO Settings:
            Pin Name | Signal on Pin       | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PA3      | USB_OTG_HS_ULPI_D0  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PA5      | USB_OTG_HS_ULPI_CLK | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB0      | USB_OTG_HS_ULPI_D1  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB1      | USB_OTG_HS_ULPI_D2  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB5      | USB_OTG_HS_ULPI_D7  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB10     | USB_OTG_HS_ULPI_D3  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB11     | USB_OTG_HS_ULPI_D4  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB12     | USB_OTG_HS_ULPI_D5  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PB13     | USB_OTG_HS_ULPI_D6  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PC0      | USB_OTG_HS_ULPI_STP | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PH4      | USB_OTG_HS_ULPI_NXT | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
            PI11     | USB_OTG_HS_ULPI_DIR | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **High**     | n/a       | .
       \n

  4. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled       | Preemption Priority | Sub Priority
            :---------------------------------|:-------------:|:-------------------:|:------------:
            USB On The Go HS global interrupt | **checked**   | 0                   | 0
            USB On The Go FS global interrupt | **checked**   | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..  | Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-------------:|:-------------------:|:----------------:|:----------------:
            USB On The Go HS global interrupt | unchecked     | checked             | **unchecked**    | n/a
            USB On The Go FS global interrupt | unchecked     | checked             | **unchecked**    | n/a

### Clock Configuration tab

  1. Configure **To USB (MHz)**: **48**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_USB_OTG_FS_PCD_Init      | USB_OTG_FS       | **checked**       | checked
        checked       | MX_USB_OTG_HS_PCD_Init      | USB_OTG_HS       | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "OTG_STM32H7xx.h"
#include "USBD_STM32H7xx.h"

#include "stm32h7xx.h"

#include "string.h"

#if (OTG_FS_DEVICE_USED == 1)
#ifndef USBD_FS_MAX_ENDPOINT_NUM
#define USBD_FS_MAX_ENDPOINT_NUM   (8U)
#endif
#if    (USBD_FS_MAX_ENDPOINT_NUM > 8U)
#error  Too many Endpoints, maximum IN/OUT Endpoint pairs that this driver supports is 8 !!!
#endif

// FIFO sizes in bytes (total available memory for FIFOs is 4 kB)
#ifndef OTG_FS_RX_FIFO_SIZE
#define OTG_FS_RX_FIFO_SIZE        (1024U)
#endif
#ifndef OTG_FS_TX0_FIFO_SIZE
#define OTG_FS_TX0_FIFO_SIZE       (64U)
#endif
#ifndef OTG_FS_TX1_FIFO_SIZE
#define OTG_FS_TX1_FIFO_SIZE       (1024U)
#endif
#ifndef OTG_FS_TX2_FIFO_SIZE
#define OTG_FS_TX2_FIFO_SIZE       (512U)
#endif
#ifndef OTG_FS_TX3_FIFO_SIZE
#define OTG_FS_TX3_FIFO_SIZE       (256U)
#endif
#ifndef OTG_FS_TX4_FIFO_SIZE
#define OTG_FS_TX4_FIFO_SIZE       (256U)
#endif
#ifndef OTG_FS_TX5_FIFO_SIZE
#define OTG_FS_TX5_FIFO_SIZE       (256U)
#endif
#ifndef OTG_FS_TX6_FIFO_SIZE
#define OTG_FS_TX6_FIFO_SIZE       (256U)
#endif
#ifndef OTG_FS_TX7_FIFO_SIZE
#define OTG_FS_TX7_FIFO_SIZE       (256U)
#endif
#ifndef OTG_FS_TX8_FIFO_SIZE
#define OTG_FS_TX8_FIFO_SIZE       (192U)
#endif
#endif

#if (OTG_HS_DEVICE_USED == 1)
#ifndef OTG_HS_IN_FS
#define OTG_HS_IN_FS               (0U)
#endif

#ifndef USBD_HS_MAX_ENDPOINT_NUM
#define USBD_HS_MAX_ENDPOINT_NUM   (8U)
#endif
#if    (USBD_HS_MAX_ENDPOINT_NUM > 8U)
#error  Too many Endpoints, maximum IN/OUT Endpoint pairs that this driver supports is 8 !!!
#endif

// FIFO sizes in bytes (total available memory for FIFOs is 4 kB)
#ifndef OTG_HS_RX_FIFO_SIZE
#define OTG_HS_RX_FIFO_SIZE        (1024U)
#endif
#ifndef OTG_HS_TX0_FIFO_SIZE
#define OTG_HS_TX0_FIFO_SIZE       (64U)
#endif
#ifndef OTG_HS_TX1_FIFO_SIZE
#define OTG_HS_TX1_FIFO_SIZE       (1024U)
#endif
#ifndef OTG_HS_TX2_FIFO_SIZE
#define OTG_HS_TX2_FIFO_SIZE       (512U)
#endif
#ifndef OTG_HS_TX3_FIFO_SIZE
#define OTG_HS_TX3_FIFO_SIZE       (256U)
#endif
#ifndef OTG_HS_TX4_FIFO_SIZE
#define OTG_HS_TX4_FIFO_SIZE       (256U)
#endif
#ifndef OTG_HS_TX5_FIFO_SIZE
#define OTG_HS_TX5_FIFO_SIZE       (256U)
#endif
#ifndef OTG_HS_TX6_FIFO_SIZE
#define OTG_HS_TX6_FIFO_SIZE       (256U)
#endif
#ifndef OTG_HS_TX7_FIFO_SIZE
#define OTG_HS_TX7_FIFO_SIZE       (256U)
#endif
#ifndef OTG_HS_TX8_FIFO_SIZE
#define OTG_HS_TX8_FIFO_SIZE       (192U)
#endif
#endif

// USBD Driver *****************************************************************

#define ARM_USBD_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,6)

// Driver Version
static const ARM_DRIVER_VERSION usbd_driver_version = { ARM_USBD_API_VERSION, ARM_USBD_DRV_VERSION };

// Driver Capabilities
static const ARM_USBD_CAPABILITIES usbd_fs_driver_capabilities = {
#ifdef MX_USB_OTG_FS_VBUS_Pin
  1U,   // VBUS Detection
  1U,   // Event VBUS On
  1U    // Event VBUS Off
#else
  0U,   // VBUS Detection
  0U,   // Event VBUS On
  0U    // Event VBUS Off
#endif
#if (defined(ARM_USBD_API_VERSION) && (ARM_USBD_API_VERSION >= 0x202U))
, 0U    // Reserved
#endif
};

// Driver Capabilities
static const ARM_USBD_CAPABILITIES usbd_hs_driver_capabilities = {
  1U,   // VBUS Detection
  1U,   // Event VBUS On
  1U    // Event VBUS Off
#if (defined(ARM_USBD_API_VERSION) && (ARM_USBD_API_VERSION >= 0x202U))
, 0U    // Reserved
#endif
};

#define EP_NUM(ep_addr)         ((ep_addr) & ARM_USB_ENDPOINT_NUMBER_MASK)
#define EP_ID(ep_addr)          ((EP_NUM(ep_addr) * 2U) + (((ep_addr) >> 7) & 1U))

#if (OTG_FS_DEVICE_USED == 1)
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

// Endpoints runtime information
static  ENDPOINT_t usbd_fs_ep[(USBD_FS_MAX_ENDPOINT_NUM + 1U) * 2U];
static             USBD_INFO_t  usbd_fs_info;
static             uint32_t     usbd_fs_setup_packet[2];

static const USBD_RESOURCES_t USBD_FS_Resources = {
  &hpcd_USB_OTG_FS,
  &usbd_fs_info,
   usbd_fs_ep,
   usbd_fs_setup_packet,
  &otg_fs_role,
   OTG_FS_IRQn,
   USBD_FS_MAX_ENDPOINT_NUM,
   0,
   OTG_FS_RX_FIFO_SIZE,
  {OTG_FS_TX0_FIFO_SIZE,
   OTG_FS_TX1_FIFO_SIZE,
   OTG_FS_TX2_FIFO_SIZE,
   OTG_FS_TX3_FIFO_SIZE,
   OTG_FS_TX4_FIFO_SIZE,
   OTG_FS_TX5_FIFO_SIZE,
   OTG_FS_TX6_FIFO_SIZE,
   OTG_FS_TX7_FIFO_SIZE,
   OTG_FS_TX8_FIFO_SIZE}
};
#endif

#if (OTG_HS_DEVICE_USED == 1)
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

// Endpoints runtime information
static  ENDPOINT_t usbd_hs_ep[(USBD_HS_MAX_ENDPOINT_NUM + 1U) * 2U];
static             USBD_INFO_t  usbd_hs_info;
static             uint32_t     usbd_hs_setup_packet[2];

static const USBD_RESOURCES_t USBD_HS_Resources = {
  &hpcd_USB_OTG_HS,
  &usbd_hs_info,
   usbd_hs_ep,
   usbd_hs_setup_packet,
  &otg_hs_role,
   OTG_HS_IRQn,
   USBD_HS_MAX_ENDPOINT_NUM,
   0,
   OTG_HS_RX_FIFO_SIZE,
  {OTG_HS_TX0_FIFO_SIZE,
   OTG_HS_TX1_FIFO_SIZE,
   OTG_HS_TX2_FIFO_SIZE,
   OTG_HS_TX3_FIFO_SIZE,
   OTG_HS_TX4_FIFO_SIZE,
   OTG_HS_TX5_FIFO_SIZE,
   OTG_HS_TX6_FIFO_SIZE,
   OTG_HS_TX7_FIFO_SIZE,
   OTG_HS_TX8_FIFO_SIZE}
};
#endif

/**
  \fn          const USBD_RESOURCES_t * USBD_Resources (PCD_HandleTypeDef *hpcd)
  \brief       Get USBD_RESOURCES_t strusture from PCD_HandleTypeDef
*/
static const USBD_RESOURCES_t * USBD_Resources (PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t *usbd = NULL;

#if (OTG_FS_DEVICE_USED == 1)
  if (hpcd->Instance == USB_OTG_FS) { usbd = &USBD_FS_Resources; }
#endif
#if (OTG_HS_DEVICE_USED == 1)
  if (hpcd->Instance == USB_OTG_HS) { usbd = &USBD_HS_Resources; }
#endif

  return usbd;
}


// USBD Driver functions

/**
  \fn          ARM_DRIVER_VERSION USBD_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USBD_GetVersion (void) { return usbd_driver_version; }

/**
  \fn          ARM_USBD_CAPABILITIES USBD_GetCapabilities (const USBD_RESOURCES_t *usbd)
  \brief       Get driver capabilities.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref ARM_USBD_CAPABILITIES
*/
static ARM_USBD_CAPABILITIES USBD_GetCapabilities (const USBD_RESOURCES_t *usbd) {
  ARM_USBD_CAPABILITIES cap ={0U};
#if (OTG_FS_DEVICE_USED == 1)
  if (usbd->p_hpcd->Instance == USB_OTG_FS) { return usbd_fs_driver_capabilities; }
#endif
#if (OTG_HS_DEVICE_USED == 1)
  if (usbd->p_hpcd->Instance == USB_OTG_HS) { return usbd_hs_driver_capabilities; }
#endif
return cap;
}

/**
  \fn          int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t    cb_device_event,
                                        ARM_USBD_SignalEndpointEvent_t  cb_endpoint_event
                                        const USBD_RESOURCES_t         *usbd)
  \brief       Initialize USB Device Interface.
  \param[in]   cb_device_event    Pointer to \ref ARM_USBD_SignalDeviceEvent
  \param[in]   cb_endpoint_event  Pointer to \ref ARM_USBD_SignalEndpointEvent
  \param[in]   usbd                Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t    cb_device_event,
                                ARM_USBD_SignalEndpointEvent_t  cb_endpoint_event,
                                const USBD_RESOURCES_t         *usbd) {

  if (usbd->info->hw_initialized == true) {
    return ARM_DRIVER_OK;
  }

  usbd->info->cb_device_event   = cb_device_event;
  usbd->info->cb_endpoint_event = cb_endpoint_event;

  *usbd->p_otg_role          = ARM_USB_ROLE_DEVICE;
  usbd->info->hw_initialized = true;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_Uninitialize (const USBD_RESOURCES_t *usbd)
  \brief       De-initialize USB Device Interface.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_Uninitialize (const USBD_RESOURCES_t *usbd) {

  usbd->p_hpcd->Instance     = NULL;
  *usbd->p_otg_role          = ARM_USB_ROLE_NONE;
  usbd->info->hw_initialized = false;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_PowerControl (ARM_POWER_STATE         state,
                                          const USBD_RESOURCES_t *usbd)
  \brief       Control USB Device Interface Power.
  \param[in]   state  Power state
  \param[in]   usbd   Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_PowerControl (ARM_POWER_STATE         state,
                                  const USBD_RESOURCES_t *usbd) {

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:
      NVIC_DisableIRQ ((IRQn_Type)usbd->irqn);

      // Deinitialize
      HAL_PCD_DeInit(usbd->p_hpcd);

      // Clear powered flag
      usbd->info->hw_powered = false;
      break;

    case ARM_POWER_FULL:
      if (usbd->info->hw_initialized == false) {
        return ARM_DRIVER_ERROR;
      }
      if (usbd->info->hw_powered == true) {
        return ARM_DRIVER_OK;
      }

      // Set powered flag
      usbd->info->hw_powered = true;

      // Initialize
#if (OTG_FS_DEVICE_USED == 1)
      if (usbd == &USBD_FS_Resources) {
        usbd->p_hpcd->Instance = USB_OTG_FS;
        usbd->p_hpcd->Init.dev_endpoints = USBD_FS_MAX_ENDPOINT_NUM;
        usbd->p_hpcd->Init.use_dedicated_ep1 = 0;
        usbd->p_hpcd->Init.ep0_mps = 0x40;
        usbd->p_hpcd->Init.low_power_enable = 0;
        usbd->p_hpcd->Init.phy_itface = PCD_PHY_EMBEDDED;
        usbd->p_hpcd->Init.Sof_enable = 0;
        usbd->p_hpcd->Init.speed = PCD_SPEED_FULL;
#ifdef MX_USB_OTG_FS_VBUS_Pin
        usbd->p_hpcd->Init.vbus_sensing_enable = 1;
#else
        usbd->p_hpcd->Init.vbus_sensing_enable = 0;
#endif
        usbd->p_hpcd->Init.lpm_enable = 0;
        usbd->p_hpcd->pData = NULL;
      }
#endif
#if (OTG_HS_DEVICE_USED == 1)
      if (usbd == &USBD_HS_Resources) {
        usbd->p_hpcd->Instance = USB_OTG_HS;
        usbd->p_hpcd->Init.dev_endpoints = USBD_HS_MAX_ENDPOINT_NUM;
        usbd->p_hpcd->Init.use_dedicated_ep1 = 0;
        usbd->p_hpcd->Init.ep0_mps = 0x40;
        usbd->p_hpcd->Init.low_power_enable = 0;
#if (OTG_HS_IN_FS == 1)
        usbd->p_hpcd->Init.phy_itface = PCD_PHY_EMBEDDED;
#else
        usbd->p_hpcd->Init.phy_itface = PCD_PHY_ULPI;
#endif
        usbd->p_hpcd->Init.Sof_enable = 0;
        usbd->p_hpcd->Init.speed = PCD_SPEED_HIGH;
        usbd->p_hpcd->Init.dma_enable = DISABLE;
        usbd->p_hpcd->Init.vbus_sensing_enable = 1;
        usbd->p_hpcd->Init.lpm_enable = 0;
        usbd->p_hpcd->pData = NULL;
      }
#endif
      HAL_PCD_Init (usbd->p_hpcd);

#if (OTG_FS_DEVICE_USED == 1)
    if (usbd == &USBD_FS_Resources) {
      HAL_PWREx_EnableUSBVoltageDetector();

      HAL_PCDEx_SetRxFiFo(usbd->p_hpcd,    OTG_FS_RX_FIFO_SIZE  / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 0, OTG_FS_TX0_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 1, OTG_FS_TX1_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 2, OTG_FS_TX2_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 3, OTG_FS_TX3_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 4, OTG_FS_TX4_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 5, OTG_FS_TX5_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 6, OTG_FS_TX6_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 7, OTG_FS_TX7_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 8, OTG_FS_TX8_FIFO_SIZE / 4);
    }
#endif

#if (OTG_HS_DEVICE_USED == 1)
    if (usbd == &USBD_HS_Resources) {
#if (OTG_HS_IN_FS == 1)
      HAL_PWREx_EnableUSBVoltageDetector();
#endif
      HAL_PCDEx_SetRxFiFo(usbd->p_hpcd,    OTG_HS_RX_FIFO_SIZE  / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 0, OTG_HS_TX0_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 1, OTG_HS_TX1_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 2, OTG_HS_TX2_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 3, OTG_HS_TX3_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 4, OTG_HS_TX4_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 5, OTG_HS_TX5_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 6, OTG_HS_TX6_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 7, OTG_HS_TX7_FIFO_SIZE / 4);
      HAL_PCDEx_SetTxFiFo(usbd->p_hpcd, 8, OTG_HS_TX8_FIFO_SIZE / 4);
    }
#endif

      NVIC_EnableIRQ ((IRQn_Type)usbd->irqn);
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceConnect (const USBD_RESOURCES_t *usbd)
  \brief       Connect USB Device.
  \param[in]   usbd Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_DeviceConnect (const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  HAL_PCD_Start(usbd->p_hpcd);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceDisconnect (const USBD_RESOURCES_t *usbd)
  \brief       Disconnect USB Device.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_DeviceDisconnect (const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  HAL_PCD_DevDisconnect(usbd->p_hpcd);

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USBD_STATE USBD_DeviceGetState (const USBD_RESOURCES_t *usbd)
  \brief       Get current USB Device State.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      Device State \ref ARM_USBD_STATE
*/
static ARM_USBD_STATE USBD_DeviceGetState (const USBD_RESOURCES_t *usbd) {
  return usbd->info->state;
}

/**
  \fn          int32_t USBD_DeviceRemoteWakeup (const USBD_RESOURCES_t *usbd)
  \brief       Trigger USB Remote Wakeup.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_DeviceRemoteWakeup (const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  HAL_PCD_ActivateRemoteWakeup(usbd->p_hpcd);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceSetAddress (uint8_t                 dev_addr,
                                              const USBD_RESOURCES_t *usbd)
  \brief       Set USB Device Address.
  \param[in]   dev_addr  Device Address
  \param[in]   usbd      Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_DeviceSetAddress (uint8_t                 dev_addr,
                                      const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  HAL_PCD_SetAddress(usbd->p_hpcd, dev_addr);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_ReadSetupPacket (uint8_t                *setup,
                                             const USBD_RESOURCES_t *usbd)
  \brief       Read setup packet received over Control Endpoint.
  \param[out]  setup  Pointer to buffer for setup packet
  \param[in]   usbd   Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_ReadSetupPacket (uint8_t                *setup,
                                     const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false)  { return ARM_DRIVER_ERROR; }
  if (usbd->info->setup_received == 0U) { return ARM_DRIVER_ERROR; }

  usbd->info->setup_received = 0U;
  memcpy(setup, usbd->p_setup_packet, 8);

  // If new setup packet was received while this was being read
  if (usbd->info->setup_received != 0U) {
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointConfigure (uint8_t                ep_addr,
                                               uint8_t                ep_type,
                                               uint16_t               ep_max_packet_size,
                                               const USBD_RESOURCES_t *usbd)
  \brief       Configure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   ep_type  Endpoint Type (ARM_USB_ENDPOINT_xxx)
  \param[in]   ep_max_packet_size Endpoint Maximum Packet Size
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_EndpointConfigure (uint8_t                ep_addr,
                                       uint8_t                ep_type,
                                       uint16_t               ep_max_packet_size,
                                       const USBD_RESOURCES_t *usbd) {
  uint8_t              ep_num;
  uint16_t             ep_mps;
  volatile ENDPOINT_t *ptr_ep;

  ep_num = EP_NUM(ep_addr);
  ep_mps = ep_max_packet_size & ARM_USB_ENDPOINT_MAX_PACKET_SIZE_MASK;

  if (ep_num > usbd->max_ep_num)       { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK) { // IN endpoint
    if (ep_mps > usbd->tx_fifo_size[ep_num])                          { return ARM_DRIVER_ERROR_PARAMETER; }
  } else {                                                                              // OUT endpoint
    if ((ep_mps + 60U + (8 * usbd->max_ep_num)) > usbd->rx_fifo_size) { return ARM_DRIVER_ERROR_PARAMETER; }
  }
  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  ptr_ep = &usbd->ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  // Clear Endpoint transfer and configuration information
  memset((void *)((uint32_t)ptr_ep), 0, sizeof (ENDPOINT_t));

  // Set maximum packet size to requested
  ptr_ep->max_packet_size = ep_mps;

  HAL_PCD_EP_Open(usbd->p_hpcd, ep_addr, ep_mps, ep_type);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointUnconfigure (uint8_t                 ep_addr,
                                                 const USBD_RESOURCES_t *usbd)
  \brief       Unconfigure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction

  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_EndpointUnconfigure (uint8_t                 ep_addr,
                                         const USBD_RESOURCES_t *usbd) {
  uint8_t              ep_num;
  volatile ENDPOINT_t *ptr_ep;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > usbd->max_ep_num)       { return ARM_DRIVER_ERROR_PARAMETER; }
  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  ptr_ep = &usbd->ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  // Clear Endpoint transfer and configuration information
  memset((void *)((uint32_t)ptr_ep), 0, sizeof (ENDPOINT_t));

  HAL_PCD_EP_Close(usbd->p_hpcd, ep_addr);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointStall (uint8_t                 ep_addr,
                                           bool                    stall,
                                           const USBD_RESOURCES_t *usbd)
  \brief       Set/Clear Stall for USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   stall  Operation
                - \b false Clear
                - \b true Set
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_EndpointStall (uint8_t                 ep_addr,
                                   bool                    stall,
                                   const USBD_RESOURCES_t *usbd) {
  uint8_t ep_num;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > usbd->max_ep_num)       { return ARM_DRIVER_ERROR_PARAMETER; }
  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  if (stall != 0U) {
    // Set STALL
    HAL_PCD_EP_SetStall(usbd->p_hpcd, ep_addr);
  } else {
    // Clear STALL
    HAL_PCD_EP_ClrStall(usbd->p_hpcd, ep_addr);
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointTransfer (uint8_t                 ep_addr,
                                              uint8_t                *data,
                                              uint32_t                num,
                                              const USBD_RESOURCES_t *usbd)
  \brief       Read data from or Write data to USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[out]  data Pointer to buffer for data to read or with data to write
  \param[in]   num  Number of data bytes to transfer
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransfer (uint8_t                 ep_addr,
                                      uint8_t                *data,
                                      uint32_t                num,
                                      const USBD_RESOURCES_t *usbd) {
  uint8_t              ep_num;
  bool                 ep_dir;
  volatile ENDPOINT_t *ptr_ep;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > usbd->max_ep_num)       { return ARM_DRIVER_ERROR_PARAMETER; }
  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }


  ptr_ep = &usbd->ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  ep_dir = (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK;

  ptr_ep->active = 1U;

  ptr_ep->data                  = data;
  ptr_ep->num                   = num;
  ptr_ep->num_transferred_total = 0U;
  ptr_ep->num_transferring      = num;

  if ((ep_addr & 0x7F) == 0) {
    // Only for EP0
    if (ptr_ep->max_packet_size < num) {
      ptr_ep->num_transferring = ptr_ep->max_packet_size;
    }
  }

  if (ep_dir != 0U) {
    // IN Endpoint
    HAL_PCD_EP_Transmit(usbd->p_hpcd, ep_addr, (uint8_t *)data, ptr_ep->num_transferring);
  } else {
    // OUT Endpoint
    HAL_PCD_EP_Receive(usbd->p_hpcd, ep_addr, (uint8_t *)data, ptr_ep->num_transferring);
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USBD_EndpointTransferGetResult (uint8_t                 ep_addr,
                                                        const USBD_RESOURCES_t *usbd)
  \brief       Get result of USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      number of successfully transferred data bytes
*/
static uint32_t USBD_EndpointTransferGetResult (uint8_t                 ep_addr,
                                                const USBD_RESOURCES_t *usbd) {
  volatile ENDPOINT_t *ptr_ep;

  ptr_ep = &usbd->ep[EP_ID(ep_addr)];
  return (ptr_ep->num_transferred_total);
}

/**
  \fn          int32_t USBD_EndpointTransferAbort (uint8_t                 ep_addr,
                                                   const USBD_RESOURCES_t *usbd)
  \brief       Abort current USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransferAbort (uint8_t                 ep_addr,
                                           const USBD_RESOURCES_t *usbd) {
           uint8_t       ep_num;
  volatile ENDPOINT_t   *ptr_ep;
  volatile uint32_t      tout;
           uint32_t      USBx_BASE;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > usbd->max_ep_num)       { return ARM_DRIVER_ERROR_PARAMETER; }
  if (usbd->info->hw_powered == false) { return ARM_DRIVER_ERROR; }

  USBx_BASE = (uint32_t)usbd->p_hpcd->Instance;
  if (ep_addr &  0x80) {
    USBx_INEP(ep_num)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;

    // Wait until core starts transmitting NAK handshakes
    tout = 1000;
    while (((USBx_INEP(ep_num)->DIEPCTL & USB_OTG_DIEPCTL_NAKSTS) == 0) && (tout-- != 0)) {
      __NOP();
    }
    HAL_PCD_EP_Flush(usbd->p_hpcd, ep_addr);
  } else {
    USBx_OUTEP(ep_num)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
  }

  ptr_ep = &usbd->ep[EP_ID(ep_addr)];
  ptr_ep->active = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          uint16_t USBD_GetFrameNumber (const USBD_RESOURCES_t *usbd)
  \brief       Get current USB Frame Number.
  \param[in]   usbd  Pointer to USB_OTG resources
  \return      Frame Number
*/
static uint16_t USBD_GetFrameNumber (const USBD_RESOURCES_t *usbd) {

  if (usbd->info->hw_powered == false) { return 0U; }
  return ((uint16_t)USB_GetCurrentFrame(usbd->p_hpcd->Instance));
}

// Callbacks from HAL
/**
  * @brief  Data OUT stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  uint32_t             cnt;
  volatile ENDPOINT_t *ptr_ep;
  const    USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);
  ptr_ep = &usbd->ep[EP_ID(epnum)];

  if (epnum != 0) {
    if (usbd->info->cb_endpoint_event != NULL) {
      ptr_ep->active = 0U;
      ptr_ep->num_transferred_total = HAL_PCD_EP_GetRxCount(hpcd, epnum);
      usbd->info->cb_endpoint_event(epnum, ARM_USBD_EVENT_OUT);
    }
  } else {
    cnt = HAL_PCD_EP_GetRxCount(hpcd, epnum);
    ptr_ep->num_transferred_total += cnt;
    if ((cnt < ptr_ep->max_packet_size) || (ptr_ep->num_transferred_total == ptr_ep->num)) {
      if (usbd->info->cb_endpoint_event != NULL) {
        ptr_ep->active = 0U;
         usbd->info->cb_endpoint_event(epnum, ARM_USBD_EVENT_OUT);
      }
    } else {
      ptr_ep->num_transferring = ptr_ep->num - ptr_ep->num_transferred_total;
      if (ptr_ep->num_transferring > ptr_ep->max_packet_size) {
        ptr_ep->num_transferring = ptr_ep->max_packet_size;
      }
      HAL_PCD_EP_Receive(usbd->p_hpcd, epnum, (uint8_t *)(ptr_ep->data + ptr_ep->num_transferred_total), ptr_ep->num_transferring);
    }
  }
}

/**
  * @brief  Data IN stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  volatile ENDPOINT_t *ptr_ep;
  const    USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);
  ptr_ep = &usbd->ep[EP_ID(epnum | 0x80)];

  ptr_ep->num_transferred_total += ptr_ep->num_transferring;
  if (epnum != 0) {
    if ( usbd->info->cb_endpoint_event != NULL) {
      ptr_ep->active = 0U;
       usbd->info->cb_endpoint_event(epnum | ARM_USB_ENDPOINT_DIRECTION_MASK, ARM_USBD_EVENT_IN);
    }
  } else {
    // EP0
    if(ptr_ep->num_transferred_total == ptr_ep->num) {
      if (usbd->info->cb_endpoint_event != NULL) {
        ptr_ep->active = 0U;
        usbd->info->cb_endpoint_event(epnum | ARM_USB_ENDPOINT_DIRECTION_MASK, ARM_USBD_EVENT_IN);
      }
    } else {
      ptr_ep->num_transferring = ptr_ep->num - ptr_ep->num_transferred_total;
      if (ptr_ep->num_transferring > ptr_ep->max_packet_size) {
        ptr_ep->num_transferring = ptr_ep->max_packet_size;
      }
        HAL_PCD_EP_Transmit(usbd->p_hpcd, epnum | 0x80, (uint8_t *)(ptr_ep->data + ptr_ep->num_transferred_total), ptr_ep->num_transferring);
    }
  }
}
/**
  * @brief  Setup stage callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);

  memcpy(usbd->p_setup_packet, hpcd->Setup, 8);
  usbd->info->setup_received = 1;

  // Analyze Setup packet for SetAddress
  if ((*usbd->p_setup_packet & 0xFFFFU) == 0x0500U) {
    USBD_DeviceSetAddress((*usbd->p_setup_packet >> 16) & 0xFFU, usbd);
  }

  if (usbd->info->cb_endpoint_event != NULL) {
    usbd->info->cb_endpoint_event(0, ARM_USBD_EVENT_SETUP);
  }
}

/**
  * @brief  USB Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;
  usbd = USBD_Resources(hpcd);

  memset((void *)((uint32_t)usbd->ep), 0U, sizeof(ENDPOINT_t) * usbd->max_ep_num * 2);
  memset((void *)((uint32_t)&usbd->info->state), 0, sizeof(ARM_USBD_STATE));

  if (usbd->info->cb_device_event != NULL) {
    usbd->info->cb_device_event(ARM_USBD_EVENT_RESET);
  }

  // USB Speed
  switch (hpcd->Init.speed)
  {
  case PCD_SPEED_HIGH:
    usbd->info->state.speed = ARM_USB_SPEED_HIGH;
    if (usbd->info->cb_device_event != NULL) {
      usbd->info->cb_device_event(ARM_USBD_EVENT_HIGH_SPEED );
    }
    break;

  case PCD_SPEED_FULL:
  default:
    usbd->info->state.speed = ARM_USB_SPEED_FULL;
    break;
  }

  HAL_PCD_EP_Open(usbd->p_hpcd, 0x80, 64, 0);
  HAL_PCD_EP_Open(usbd->p_hpcd, 0x00, 64, 0);
}

/**
  * @brief  Suspend event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);

  if (usbd->info->cb_device_event != NULL) {
    usbd->info->cb_device_event(ARM_USBD_EVENT_SUSPEND);
  }
}

/**
  * @brief  Resume event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);

  if (usbd->info->cb_device_event != NULL) {
    usbd->info->cb_device_event(ARM_USBD_EVENT_RESUME);
  }
}

/**
  * @brief  Incomplete ISO OUT callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOOUTIncompleteCallback could be implemented in the user file
   */
}

/**
  * @brief  Incomplete ISO IN callback.
  * @param  hpcd: PCD handle
  * @param  epnum: endpoint number
  * @retval None
  */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOINIncompleteCallback could be implemented in the user file
   */
}

/**
  * @brief  Connection event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);

  if (usbd->info->cb_device_event != NULL) {
    usbd->info->cb_device_event(ARM_USBD_EVENT_VBUS_ON);
  }
}

/**
  * @brief  Disconnection event callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {
  const USBD_RESOURCES_t * usbd;

  usbd = USBD_Resources(hpcd);

  if (usbd->info->cb_device_event != NULL) {
    usbd->info->cb_device_event(ARM_USBD_EVENT_VBUS_OFF);
  }
}

#if (OTG_FS_DEVICE_USED == 1)
USBDx_EXPORT_DRIVER(0, FS);
#endif

#if (OTG_HS_DEVICE_USED == 1)
USBDx_EXPORT_DRIVER(1, HS);
#endif

/*! \endcond */
