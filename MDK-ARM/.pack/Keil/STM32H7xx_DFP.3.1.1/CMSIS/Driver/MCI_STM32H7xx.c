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
 * $Revision:    V1.8
 *
 * Driver:       Driver_MCI0/1
 *
 * Configured:   via CubeMX
 * Project:      MCI Driver for ST STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_mci MCI

# Revision History

- Version 1.8
  - Improved PowerControl and Control functions
  - Made variable status volatile (solved potential LTO problems)
- Version 1.7
  - Updated retrieval of SDMMC kernel clock frequency in PowerControl function
- Version 1.6
  - Corrected Data cache handling
- Version 1.5
  - Replaced empty delay loops with _NOP()
- Version 1.4
  - Added handling for separate SD and MMC HAL layers
- Version 1.3
  - Added busy signal handling after CMDREND interrupt
- Version 1.2
  - Added data cache enable bit check
- Version 1.1
  - Updated to be compliant with HAL drivers version 1.5.0
- Version 1.0
  - Initial release

# Capabilities

This driver supports **SD** and **MMC** devices.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance     | Hardware Resource
:-------------------|:-----------------:
Driver_MCI0         | SDMMC1
Driver_MCI1         | SDMMC2

# Limitations

DMA usage limitations:
 - The size of DCache line on Cortex M7 is 32 Bytes. To safely perform
   DCache maintenance operations, MCI DMA transfers data must be aligned
   to a 32 Byte boundary and data size must be n*32 Bytes.

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                         | Default value | Value         | Description
:----------------------------------|:-------------:|:--------------:|:-----------
MCI_DCACHE_MAINTENANCE             |     **1**     |   1            | MCI DCache maintenance operations: **enabled**
^                                  |       ^       |   0            | MCI DCache maintenance operations: **disabled**
MemoryCard_CD0_Pin_Active          | GPIO_PIN_RESET| GPIO_PIN_RESET | Card Detect pin active state: **low**
^                                  | ^             | GPIO_PIN_SET   | Card Detect pin active state: **high**
MemoryCard_WP0_Pin_Active          | GPIO_PIN_SET  | GPIO_PIN_RESET | Write Protect pin active state: **low**
^                                  | ^             | GPIO_PIN_SET   | Write Protect pin active state: **high**
MemoryCard_CD1_Pin_Active          | GPIO_PIN_RESET| GPIO_PIN_RESET | Card Detect pin active state: **low**
^                                  | ^             | GPIO_PIN_SET   | Card Detect pin active state: **high**
MemoryCard_WP1_Pin_Active          | GPIO_PIN_SET  | GPIO_PIN_RESET | Write Protect pin active state: **low**
^                                  | ^             | GPIO_PIN_SET   | Write Protect pin active state: **high**
MemoryCard_CD0_Pin_Extern          | not defined   | not defined    | Card Detect pin handling: **internal** MCI_ReadCD() function
^                                  | ^             | defined        | Card Detect pin handling: **external** MCI_ReadCD() function
MemoryCard_WP0_Pin_Extern          | not defined   | not defined    | Write Protect pin handling: **internal** MCI_ReadCD() function
^                                  | ^             | defined        | Write Protect pin handling: **external** MCI_ReadCD() function
MemoryCard_CD1_Pin_Extern          | not defined   | not defined    | Card Detect pin handling: **internal** MCI_ReadCD() function
^                                  | ^             | defined        | Card Detect pin handling: **external** MCI_ReadCD() function
MemoryCard_WP1_Pin_Extern          | not defined   | not defined    | Write Protect pin handling: **internal** MCI_ReadCD() function
^                                  | ^             | defined        | Write Protect pin handling: **external** MCI_ReadCD() function
MemoryCard_MMC0                    | not defined   | not defined    | SDMMC1 is configured for SD device
^                                  | ^             | defined        | SDMMC1 is configured for MMC device
MemoryCard_MMC1                    | not defined   | not defined    | SDMMC2 is configured for SD device
^                                  | ^             | defined        | SDMMC2 is configured for MMC device

## STM32CubeMX

The MCI driver requires:
  - **SDMMC1,2 peripheral clock**
  - **SDMMC** peripheral **mode** configured
  - **SDMMC_CK, SDMMC_CMD** and **SDMMC_Dx** pins
  - **SDMMC1 global interrupt** enabled in NVIC
  - When using MMC devices:
    - Define **MemoryCard_MMC0** if SDMMC1 uses MMC device (otherwise define is not required)
    - Define **MemoryCard_MMC1** if SDMMC2 uses MMC device (otherwise define is not required)

Optional configuration:
  - Card Detect Pin:
    - Configure arbitrary pin in **GPIO_Input** mode and add User Label: **MemoryCard_CD0**
    - Alternatively, define **MemoryCard_CDx_Pin_Extern** and **implement function MCI_ReadCD** in application
  - Write Protect Pin:
    - Configure arbitrary pin in **GPIO_Input** mode and add User Label: **MemoryCard_WP0**
    - Alternatively, **define MemoryCard_WPx_Pin_Extern** and **implement function MCI_ReadWP** in application
  - Configuring data buffers to reside in:
    - AXI SRAM when using SDMMC1 (Driver_MCI0)
    - AHB SRAM when using SDMMC2 (Driver_MCI1)

\note The User Label name is used to connect the CMSIS-Driver to the GPIO pin.

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
SDMMC1     | SD 4 bits Wide bus with dir..| SDMMC1 controller
PB8        | Alternate Function Push Pull | SDMMC1_CKIN Pin
PB9        | Alternate Function Push Pull | SDMMC1_CDIR Pin
PC6        | Alternate Function Push Pull | SDMMC1_D0DIR Pin
PC7        | Alternate Function Push Pull | SDMMC1_D123DIR Pin
PC8        | Alternate Function Push Pull | SDMMC1_D0 Pin
PC9        | Alternate Function Push Pull | SDMMC1_D1 Pin
PC10       | Alternate Function Push Pull | SDMMC1_D2 Pin
PC11       | Alternate Function Push Pull | SDMMC1_D3 Pin
PC12       | Alternate Function Push Pull | SDMMC1_CK Pin
PD2        | Alternate Function Push Pull | SDMMC1_CMD Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PB8      | **SDMMC1_CKIN**
        PB9      | **SDMMC1_CDIR**
        PC6      | **SDMMC1_D0DIR**
        PC7      | **SDMMC1_D123DIR**
        PC8      | **SDMMC1_D0**
        PC9      | **SDMMC1_D1**
        PC10     | **SDMMC1_D2**
        PC11     | **SDMMC1_D3**
        PC12     | **SDMMC1_CK**
        PD2      | **SDMMC1_CMD**
       \n

  2. Under **Categories**: **Connectivity** select **SDMMC1**:

     __Mode__:
       - Mode: **SD 4 bits Wide bus with dir voltage converter**

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PB8      | SDMMC1_CKIN   | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PB9      | SDMMC1_CDIR   | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC6      | SDMMC1_D0DIR  | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC7      | SDMMC1_D123DIR| n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC8      | SDMMC1_D0     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC9      | SDMMC1_D1     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC10     | SDMMC1_D2     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC11     | SDMMC1_D3     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PC12     | SDMMC1_CK     | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
            PD2      | SDMMC1_CMD    | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Very High**| n/a       | .
       \n

  3. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table    | Enabled     | Preemption Priority | Sub Priority
            :-----------------------|:-----------:|:-------------------:|:------------:
            SDMMC1 global interrupt | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :-----------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            SDMMC1 global interrupt | unchecked   | checked             | **unchecked**    | n/a

### Clock Configuration tab

  1. Configure **To SDMMC1,2 (MHz)**: **75**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_SDMMC1_SD_Init           | SDMMC1           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "MCI_STM32H7xx.h"

#define ARM_MCI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,8)  /* driver version */

/* MCI0: Define Card Detect pin active state */
#if !defined(MemoryCard_CD0_Pin_Active)
  #define MemoryCard_CD0_Pin_Active GPIO_PIN_RESET
#endif

/* MCI0: Define Write Protect pin active state */
#if !defined(MemoryCard_WP0_Pin_Active)
  #define MemoryCard_WP0_Pin_Active GPIO_PIN_SET
#endif

/* MCI1: Define Card Detect pin active state */
#if !defined(MemoryCard_CD1_Pin_Active)
  #define MemoryCard_CD1_Pin_Active GPIO_PIN_RESET
#endif

/* MCI1: Define Write Protect pin active state */
#if !defined(MemoryCard_WP1_Pin_Active)
  #define MemoryCard_WP1_Pin_Active GPIO_PIN_SET
#endif

/* MCI0: Define Card Detect pin handling with external MCI_ReadCD() function */
#if defined(MemoryCard_CD0_Pin_Extern)
  #define MCI0_CD_PIN_EXT     1U
#else
  #define MCI0_CD_PIN_EXT     0U
#endif

/* MCI0: Define Write Protect pin handling with external MCI_ReadWP() function */
#if defined(MemoryCard_WP0_Pin_Extern)
  #define MCI0_WP_PIN_EXT     1U
#else
  #define MCI0_WP_PIN_EXT     0U
#endif

/* MCI1: Define Card Detect pin handling with external MCI_ReadCD() function */
#if defined(MemoryCard_CD1_Pin_Extern)
  #define MCI1_CD_PIN_EXT     1U
#else
  #define MCI1_CD_PIN_EXT     0U
#endif

/* MCI1: Define Write Protect pin handling with external MCI_ReadWP() function */
#if defined(MemoryCard_WP1_Pin_Extern)
  #define MCI1_WP_PIN_EXT     1U
#else
  #define MCI1_WP_PIN_EXT     0U
#endif

/* MCI0: Define MemoryCard_MMC0 if SDMMC1 is configured for MMC device */
#if !defined(MemoryCard_MMC0)
  #define MCI0_HANDLE_TYPE    0U
#else
  #define MCI0_HANDLE_TYPE    1U
#endif

/* MCI0: Define MemoryCard_MMC1 if SDMMC2 is configured for MMC device */
#if !defined(MemoryCard_MMC1)
  #define MCI1_HANDLE_TYPE    0U
#else
  #define MCI1_HANDLE_TYPE    1U
#endif

#if defined(MX_SDMMC1)
#if (MCI0_HANDLE_TYPE == 0)
#define MCI0_HANDLE      hsd1
extern SD_HandleTypeDef  hsd1;
#else
#define MCI0_HANDLE      hmmc1
extern MMC_HandleTypeDef hmmc1;
#endif

#ifndef MCI_DCACHE_MAINTENANCE
#define MCI_DCACHE_MAINTENANCE    (1U)
#endif

/* IRQ handler prototype */
void SDMMC1_IRQHandler (void);

#if (MCI0_CD_PIN != 0U)
static MCI_IO MCI0_IO_CD = { MX_MemoryCard_CD0_GPIOx,
                             MX_MemoryCard_CD0_GPIO_Pin, 0U,
                             MX_MemoryCard_CD0_GPIO_PuPd,
                             MemoryCard_CD0_Pin_Active };
#endif
#if (MCI0_WP_PIN != 0U)
static MCI_IO MCI0_IO_WP = { MX_MemoryCard_WP0_GPIOx,
                             MX_MemoryCard_WP0_GPIO_Pin, 0U,
                             MX_MemoryCard_WP0_GPIO_PuPd,
                             MemoryCard_WP0_Pin_Active };
#endif

/* MCI0 Information (Run-Time) */
static MCI_INFO MCI0_Info = {
  NULL,
  {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
  NULL,
  0U,
  0U,
  0U,
  0U
};

/* MCI0 Resources */
static MCI_RESOURCES MCI0_Resources = {
  &MCI0_HANDLE,
  SDMMC1,
  #if (MCI0_CD_PIN != 0U)
  &MCI0_IO_CD,
  #else
  NULL,
  #endif
  #if (MCI0_WP_PIN != 0U)
  &MCI0_IO_WP,
  #else
  NULL,
  #endif
  &MCI0_Info
};

/* MCI0 Driver Capabilities */
static const ARM_MCI_CAPABILITIES MCI0_DriverCapabilities = {
  MCI0_CD_PIN | MCI0_CD_PIN_EXT,                  /* cd_state          */
  0U,                                             /* cd_event          */
  MCI0_WP_PIN | MCI0_WP_PIN_EXT,                  /* wp_state          */
  0U,                                             /* vdd               */
  0U,                                             /* vdd_1v8           */
  0U,                                             /* vccq              */
  0U,                                             /* vccq_1v8          */
  0U,                                             /* vccq_1v2          */
  MCI0_BUS_WIDTH_4,                               /* data_width_4      */
  MCI0_BUS_WIDTH_8,                               /* data_width_8      */
  0U,                                             /* data_width_4_ddr  */
  0U,                                             /* data_width_8_ddr  */
  1U,                                             /* high_speed        */
  0U,                                             /* uhs_signaling     */
  0U,                                             /* uhs_tuning        */
  0U,                                             /* uhs_sdr50         */
  0U,                                             /* uhs_sdr104        */
  0U,                                             /* uhs_ddr50         */
  0U,                                             /* uhs_driver_type_a */
  0U,                                             /* uhs_driver_type_c */
  0U,                                             /* uhs_driver_type_d */
  1U,                                             /* sdio_interrupt    */
  1U,                                             /* read_wait         */
  0U,                                             /* suspend_resume    */
  0U,                                             /* mmc_interrupt     */
  0U,                                             /* mmc_boot          */
  0U,                                             /* rst_n             */
  0U,                                             /* ccs               */
  0U                                              /* ccs_timeout       */
#if (defined(ARM_MCI_API_VERSION) && (ARM_MCI_API_VERSION >= 0x203U))
, 0U                                              /* reserved bits     */
#endif
};

#endif /* MX_SDMMC1 */

#if defined(MX_SDMMC2)
#if (MCI1_HANDLE_TYPE == 0)
#define MCI1_HANDLE      hsd2
extern SD_HandleTypeDef  hsd2;
#else
#define MCI1_HANDLE      hmmc2
extern MMC_HandleTypeDef hmmc2;
#endif

/* IRQ handler prototype */
void SDMMC2_IRQHandler (void);

#if (MCI1_CD_PIN != 0U)
static MCI_IO MCI1_IO_CD = { MX_MemoryCard_CD1_GPIOx,
                             MX_MemoryCard_CD1_GPIO_Pin, 0U,
                             MX_MemoryCard_CD1_GPIO_PuPd,
                             MemoryCard_CD1_Pin_Active };
#endif
#if (MCI1_WP_PIN != 0U)
static MCI_IO MCI1_IO_WP = { MX_MemoryCard_WP1_GPIOx,
                             MX_MemoryCard_WP1_GPIO_Pin, 0U,
                             MX_MemoryCard_WP1_GPIO_PuPd,
                             MemoryCard_WP1_Pin_Active };
#endif

/* MCI1 Information (Run-Time) */
static MCI_INFO MCI1_Info = {
  NULL,
  {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
  NULL,
  0U,
  0U,
  0U,
  0U
};

/* MCI1 Resources */
static MCI_RESOURCES MCI1_Resources = {
  &MCI1_HANDLE,
  SDMMC2,
  #if (MCI1_CD_PIN != 0U)
  &MCI1_IO_CD,
  #else
  NULL,
  #endif
  #if (MCI1_WP_PIN != 0U)
  &MCI1_IO_WP,
  #else
  NULL,
  #endif
  &MCI1_Info
};

/* MCI1 Driver Capabilities */
static const ARM_MCI_CAPABILITIES MCI1_DriverCapabilities = {
  MCI1_CD_PIN | MCI1_CD_PIN_EXT,                  /* cd_state          */
  0U,                                             /* cd_event          */
  MCI1_WP_PIN | MCI1_WP_PIN_EXT,                  /* wp_state          */
  0U,                                             /* vdd               */
  0U,                                             /* vdd_1v8           */
  0U,                                             /* vccq              */
  0U,                                             /* vccq_1v8          */
  0U,                                             /* vccq_1v2          */
  MCI1_BUS_WIDTH_4,                               /* data_width_4      */
  MCI1_BUS_WIDTH_8,                               /* data_width_8      */
  0U,                                             /* data_width_4_ddr  */
  0U,                                             /* data_width_8_ddr  */
  1U,                                             /* high_speed        */
  0U,                                             /* uhs_signaling     */
  0U,                                             /* uhs_tuning        */
  0U,                                             /* uhs_sdr50         */
  0U,                                             /* uhs_sdr104        */
  0U,                                             /* uhs_ddr50         */
  0U,                                             /* uhs_driver_type_a */
  0U,                                             /* uhs_driver_type_c */
  0U,                                             /* uhs_driver_type_d */
  1U,                                             /* sdio_interrupt    */
  1U,                                             /* read_wait         */
  0U,                                             /* suspend_resume    */
  0U,                                             /* mmc_interrupt     */
  0U,                                             /* mmc_boot          */
  0U,                                             /* rst_n             */
  0U,                                             /* ccs               */
  0U                                              /* ccs_timeout       */
#if (defined(ARM_MCI_API_VERSION) && (ARM_MCI_API_VERSION >= 0x203U))
, 0U                                              /* reserved bits     */
#endif
};

#endif /* MX_SDMMC2 */


/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_MCI_API_VERSION,
  ARM_MCI_DRV_VERSION
};


/**
  \fn          void Control_SDMMC_Reset (SDMMC_TypeDef *instance)
  \brief       Reset SDMMC peripheral
*/
static void Control_SDMMC_Reset (SDMMC_TypeDef *instance) {

  if (instance == SDMMC1) {
    __HAL_RCC_SDMMC1_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDMMC1_RELEASE_RESET();
  }
#if defined(SDMMC2)
  else {
    __HAL_RCC_SDMMC2_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDMMC2_RELEASE_RESET();
  }
#endif
}

/**
  \fn          void Assign_SDMMC_Instance (uint32_t set, MCI_RESOURCES *mci)
  \brief       Set/Reset instance variable inside HAL handle structure
*/
static void Assign_SDMMC_Instance (uint32_t set, MCI_RESOURCES *mci) {
#if (defined(MX_SDMMC1) && (MCI0_HANDLE_TYPE == 0)) || \
    (defined(MX_SDMMC2) && (MCI1_HANDLE_TYPE == 0))
  SD_HandleTypeDef *h_sd;
#endif
#if (defined(MX_SDMMC1) && (MCI0_HANDLE_TYPE != 0)) || \
    (defined(MX_SDMMC2) && (MCI1_HANDLE_TYPE != 0))
  MMC_HandleTypeDef *h_mmc;
#endif

#if defined(MX_SDMMC1)
  if (mci == &MCI0_Resources) {
    #if (MCI0_HANDLE_TYPE == 0)
      h_sd = (SD_HandleTypeDef *)mci->h;
      /* Instance is the coresponding peripheral register inteface */
      if (set == 0) {
        h_sd->Instance = NULL;
      } else {
        h_sd->Instance = mci->reg;
      }
    #else
      h_mmc = (MMC_HandleTypeDef *)mci->h;
      /* Instance is the coresponding peripheral register inteface */
      if (set == 0) {
        h_mmc->Instance = NULL;
      } else {
        h_mmc->Instance = mci->reg;
      }
    #endif
  }
#endif

#if defined(MX_SDMMC2)
  if (mci == &MCI1_Resources) {
    #if (MCI1_HANDLE_TYPE == 0)
      h_sd = (SD_HandleTypeDef *)mci->h;
      /* Instance is the coresponding peripheral register inteface */
      if (set == 0) {
        h_sd->Instance = NULL;
      } else {
        h_sd->Instance = mci->reg;
      }
    #else
      h_mmc = (MMC_HandleTypeDef *)mci->h;
      /* Instance is the coresponding peripheral register inteface */
      if (set == 0) {
        h_mmc->Instance = NULL;
      } else {
        h_mmc->Instance = mci->reg;
      }
    #endif
  }
#endif
}

/**
  \fn          void Config_SDMMC_Msp (uint32_t init, MCI_RESOURCES *mci)
  \brief       Init/DeInit MSP layer
*/
static void Config_SDMMC_Msp (uint32_t init, MCI_RESOURCES *mci) {
#if defined(MX_SDMMC1)
  if (mci == &MCI0_Resources) {
    #if (MCI0_HANDLE_TYPE == 0)
      if (mci->h != NULL) {
        if (init == 0) {
          HAL_SD_MspDeInit (mci->h);
        } else {
          HAL_SD_MspInit (mci->h);
        }
      }
    #else
      if (mci->h != NULL) {
        if (init == 0) {
          HAL_MMC_MspDeInit (mci->h);
        } else {
          HAL_MMC_MspInit (mci->h);
        }
      }
    #endif
  }
#endif

#if defined(MX_SDMMC2)
  if (mci == &MCI1_Resources) {
    #if (MCI1_HANDLE_TYPE == 0)
      if (mci->h != NULL) {
        if (init == 0) {
          HAL_SD_MspDeInit (mci->h);
        } else {
          HAL_SD_MspInit (mci->h);
        }
      }
    #else
      if (mci->h != NULL) {
        if (init == 0) {
          HAL_MMC_MspDeInit (mci->h);
        } else {
          HAL_MMC_MspInit (mci->h);
        }
      }
    #endif
  }
#endif
}


/**
  \fn            int32_t MCI_ReadCD (uint32_t instance)
  \brief         Read Card Detect (CD) state.
  \param[in]     instance  MCI driver instance number (0:SDMMC1, 1:SDMMC2)
  \return        1:card detected, 0:card not detected, or error
  \note This function should be reimplemented in user application
*/
__WEAK int32_t MCI_ReadCD (uint32_t instance) {
  (void)instance;
  /* Default implementation */
  return (0);
}

/**
  \fn            int32_t MCI_ReadWP (uint32_t instance)
  \brief         Read Write Protect (WP) state.
  \param[in]     instance  MCI driver instance number (0:SDMMC1, 1:SDMMC2)
  \return        1:write protected, 0:not write protected, or error
  \note This function should be reimplemented in user application
*/
__WEAK int32_t MCI_ReadWP (uint32_t instance) {
  (void)instance;
  /* Default implementation */
  return (0);
}


/**
  \fn          ARM_DRV_VERSION GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_MCI_CAPABILITIES MCI_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_MCI_CAPABILITIES
*/
static ARM_MCI_CAPABILITIES GetCapabilities (MCI_RESOURCES *mci) {
  ARM_MCI_CAPABILITIES cap;

  memset((void *)&cap, 0U, sizeof(ARM_MCI_CAPABILITIES));
#if defined(MX_SDMMC1)
  if (mci->reg == SDMMC1) { cap = MCI0_DriverCapabilities; }
#endif
#if defined(MX_SDMMC2)
  if (mci->reg == SDMMC2) { cap = MCI1_DriverCapabilities; }
#endif

  return (cap);
}


/**
  \fn            int32_t Initialize (ARM_MCI_SignalEvent_t cb_event)
  \brief         Initialize the Memory Card Interface
  \param[in]     cb_event  Pointer to \ref ARM_MCI_SignalEvent
  \return        \ref execution_status
*/
static int32_t Initialize (ARM_MCI_SignalEvent_t cb_event, MCI_RESOURCES *mci) {

  if (mci->info->flags & MCI_INIT) { return ARM_DRIVER_OK; }

  /* Set SDMMC instance pointer */
  Assign_SDMMC_Instance (1U, mci);

  /* Clear control structure */
  memset ((void *)mci->info, 0, sizeof (MCI_INFO));

  mci->info->cb_event = cb_event;
  mci->info->flags    = MCI_INIT;

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t Uninitialize (void)
  \brief         De-initialize Memory Card Interface.
  \return        \ref execution_status
*/
static int32_t Uninitialize (MCI_RESOURCES *mci) {

  mci->info->flags = 0U;

  /* Reset SDMMC instance pointer */
  Assign_SDMMC_Instance (0U, mci);

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t PowerControl (ARM_POWER_STATE state)
  \brief         Control Memory Card Interface Power.
  \param[in]     state   Power state \ref ARM_POWER_STATE
  \return        \ref execution_status
*/
static int32_t PowerControl (ARM_POWER_STATE state, MCI_RESOURCES *mci) {
  int32_t status;

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  status = ARM_DRIVER_OK;

  switch (state) {
    case ARM_POWER_OFF:
      /* Reset/Dereset SDMMC peripheral */
      Control_SDMMC_Reset (mci->reg);

      Config_SDMMC_Msp (0U, mci);

      /* Clear status */
      mci->info->status.command_active   = 0U;
      mci->info->status.command_timeout  = 0U;
      mci->info->status.command_error    = 0U;
      mci->info->status.transfer_active  = 0U;
      mci->info->status.transfer_timeout = 0U;
      mci->info->status.transfer_error   = 0U;
      mci->info->status.sdio_interrupt   = 0U;
      mci->info->status.ccs              = 0U;

      mci->info->flags &= ~MCI_POWER;
      break;

    case ARM_POWER_FULL:
      if ((mci->info->flags & MCI_INIT)  == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((mci->info->flags & MCI_POWER) != 0U) {
        return ARM_DRIVER_OK;
      }

      Config_SDMMC_Msp (1U, mci);

      /* Get SDMMC kernel clock frequency */
      mci->info->ker_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SDMMC);

      /* Clear response variable */
      mci->info->response = NULL;

      /* Enable SDMMC peripheral interrupts */
      mci->reg->MASK = SDMMC_MASK_DATAENDIE  |
                       SDMMC_MASK_CMDSENTIE  |
                       SDMMC_MASK_CMDRENDIE  |
                       SDMMC_MASK_DTIMEOUTIE |
                       SDMMC_MASK_CTIMEOUTIE |
                       SDMMC_MASK_DCRCFAILIE |
                       SDMMC_MASK_CCRCFAILIE |
                       SDMMC_MASK_BUSYD0ENDIE;

      /* Set max data timeout */
      mci->reg->DTIMER = 0xFFFFFFFF;

      /* Set transceiver polarity */
      mci->reg->POWER = SDMMC_POWER_DIRPOL;

      /* Set maximum clock divider */
      mci->reg->CLKCR = 0x3FF;

      /* Enable clock to the card (SDIO_CK) */
      mci->reg->POWER |= SDMMC_POWER_PWRCTRL_1 | SDMMC_POWER_PWRCTRL_0;

      mci->info->flags |= MCI_POWER;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return status;
}


/**
  \fn            int32_t CardPower (uint32_t voltage)
  \brief         Set Memory Card supply voltage.
  \param[in]     voltage  Memory Card supply voltage
  \return        \ref execution_status
*/
static int32_t CardPower (uint32_t voltage, MCI_RESOURCES *mci) {
  (void)voltage;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }
  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t ReadCD (void)
  \brief         Read Card Detect (CD) state.
  \return        1:card detected, 0:card not detected, or error
*/
static int32_t ReadCD (MCI_RESOURCES *mci) {
  int32_t val;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

  val = 0;

  /* Read CD (Card Detect) Pin */
  #if (MCI0_CD_PIN != 0U) || (MCI1_CD_PIN != 0U) || (MCI0_CD_PIN_EXT != 0U) || (MCI1_CD_PIN_EXT != 0U)
    if (mci->io_cd != NULL) {
      /* Note: io->af holds MemoryCard_CD_Pin_Active definition */
      if (HAL_GPIO_ReadPin (mci->io_cd->port, mci->io_cd->pin) == mci->io_cd->af) {
        /* Card Detect switch is active */
        val = 1;
      }
    }
    else {
      /* Card detect pin not specified, call application */
      if (mci->reg == SDMMC1) {
        #if (MCI0_CD_PIN_EXT != 0U)
        val = MCI_ReadCD (0U);
        #endif
      }
      else {
        #if (MCI1_CD_PIN_EXT != 0U)
        val = MCI_ReadCD (1U);
        #endif
      }
    }
  #endif

  return (val);
}


/**
  \fn            int32_t ReadWP (void)
  \brief         Read Write Protect (WP) state.
  \return        1:write protected, 0:not write protected, or error
*/
static int32_t ReadWP (MCI_RESOURCES *mci) {
  int32_t val;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

  val = 0;

  /* Read WP (Write Protect) Pin */
  #if (MCI0_WP_PIN != 0U) || (MCI1_WP_PIN != 0U) || (MCI0_WP_PIN_EXT != 0U) || (MCI1_WP_PIN_EXT != 0U)
    if (mci->io_wp != NULL) {
      /* Note: io->af holds MemoryCard_WP_Pin_Active definition */
      if (HAL_GPIO_ReadPin (mci->io_wp->port, mci->io_wp->pin) == mci->io_wp->af) {
        /* Write protect switch is active */
        val = 1;
      }
    }
    else {
      /* Write protect pin not specified, call application */
      if (mci->reg == SDMMC1) {
        #if (MCI0_WPPIN_EXT != 0U)
        val = MCI_ReadWP (0U);
        #endif
      }
      else {
        #if (MCI1_WP_PIN_EXT != 0U)
        val = MCI_ReadWP (1U);
        #endif
      }
    }
  #endif

  return (val);
}


/**
  \fn            int32_t SendCommand (uint32_t  cmd,
                                      uint32_t  arg,
                                      uint32_t  flags,
                                      uint32_t *response)
  \brief         Send Command to card and get the response.
  \param[in]     cmd       Memory Card command
  \param[in]     arg       Command argument
  \param[in]     flags     Command flags
  \param[out]    response  Pointer to buffer for response
  \return        \ref execution_status
*/
static int32_t SendCommand (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response, MCI_RESOURCES *mci) {
  uint32_t i, clkcr;

  if (((flags & MCI_RESPONSE_EXPECTED_Msk) != 0U) && (response == NULL)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((mci->info->flags & MCI_SETUP) == 0U) {
    return ARM_DRIVER_ERROR;
  }
  if (mci->info->status.command_active) {
    return ARM_DRIVER_ERROR_BUSY;
  }
  mci->info->status.command_active   = 1U;
  mci->info->status.command_timeout  = 0U;
  mci->info->status.command_error    = 0U;
  mci->info->status.transfer_timeout = 0U;
  mci->info->status.transfer_error   = 0U;
  mci->info->status.ccs              = 0U;

  if (flags & ARM_MCI_CARD_INITIALIZE) {
    clkcr = mci->reg->CLKCR;

    if ((clkcr & SDMMC_CLKCR_PWRSAV) != 0) {
      mci->reg->CLKCR = (clkcr & ~SDMMC_CLKCR_PWRSAV);

      /* Determine SDMMC_CK frequency */
      i = mci->info->ker_clk / ((clkcr & 0x3FF) << 1U);

      /* Determine ratio between CPU and SDMMC_CK frequency */
      i = SystemCoreClock / i;

      for (i *= 74; i; i--) {
        __NOP(); /* Wait for at least 74 cycles */
      }
      mci->reg->CLKCR = clkcr;
    }
  }

  if (cmd == 11U) {
    /* Voltage switch procedure enable */
    mci->reg->POWER |= SDMMC_POWER_VSWITCHEN;
  }

  /* Set command register value */
  cmd = SDMMC_CMD_CPSMEN | (cmd & 0xFFU);

  mci->info->response = response;
  mci->info->flags   &= ~(MCI_RESP_CRC | MCI_RESP_LONG);

  switch (flags & ARM_MCI_RESPONSE_Msk) {
    case ARM_MCI_RESPONSE_NONE:
      /* No response expected (wait CMDSENT) */
      break;

    case ARM_MCI_RESPONSE_SHORT:
    case ARM_MCI_RESPONSE_SHORT_BUSY:
      /* Short response expected (wait CMDREND or CCRCFAIL) */
      cmd |= SDMMC_CMD_WAITRESP_0;
      break;

    case ARM_MCI_RESPONSE_LONG:
      mci->info->flags |= MCI_RESP_LONG;
      /* Long response expected (wait CMDREND or CCRCFAIL) */
      cmd |= SDMMC_CMD_WAITRESP_1 | SDMMC_CMD_WAITRESP_0;
      break;

    default:
      return ARM_DRIVER_ERROR;
  }
  if (flags & ARM_MCI_RESPONSE_CRC) {
    mci->info->flags |= MCI_RESP_CRC;
  }
  if (flags & ARM_MCI_TRANSFER_DATA) {
    mci->info->flags |= MCI_DATA_XFER;
  }

  /* Clear all interrupt flags */
  mci->reg->ICR = SDMMC_ICR_BIT_Msk;

  /* Send the command */
  mci->reg->ARG = arg;
  mci->reg->CMD = cmd;

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t SetupTransfer (uint8_t *data,
                                        uint32_t block_count,
                                        uint32_t block_size,
                                        uint32_t mode)
  \brief         Setup read or write transfer operation.
  \param[in,out] data         Pointer to data block(s) to be written or read
  \param[in]     block_count  Number of blocks
  \param[in]     block_size   Size of a block in bytes
  \param[in]     mode         Transfer mode
  \return        \ref execution_status
*/
static int32_t SetupTransfer (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode, MCI_RESOURCES *mci) {
  uint32_t sz, dctrl, daddr;

  if ((data == NULL) || (block_count == 0U) || (block_size == 0U)) { return ARM_DRIVER_ERROR_PARAMETER; }

  if ((mci->info->flags & MCI_SETUP) == 0U) {
    return ARM_DRIVER_ERROR;
  }
  if (mci->info->status.transfer_active) {
    return ARM_DRIVER_ERROR_BUSY;
  }

#if (MCI_DCACHE_MAINTENANCE == 1U)
    /* Is DCache enabled */
    if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
      /* Data buffer must be 32 Byte aligned and size is n *32 */
      if (((((uint32_t)data)& 0x1FU) != 0) || (((block_count * block_size) & 0x1FU) != 0)) {
        return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
    }
#endif

  daddr = (uint32_t)data;

  mci->reg->IDMABASE0 = daddr;
  mci->reg->IDMACTRL  =  SDMMC_IDMA_IDMAEN;

  dctrl = 0U;

  if ((mode & ARM_MCI_TRANSFER_WRITE) == 0) {
    /* Direction: From card to controller */
    mci->info->flags |= MCI_DATA_READ;
    dctrl |= SDMMC_DCTRL_DTDIR;
  }
  else {
    /* Direction: From controller to card */
    mci->info->flags &= ~MCI_DATA_READ;

#if (MCI_DCACHE_MAINTENANCE == 1U)
    /* Is DCache enabled */
    if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
      /* Clean data cache:ensure data coherency between DMA and CPU */
      SCB_CleanDCache_by_Addr ((uint32_t *)daddr, (int32_t)(block_count * block_size));
    }
#endif
  }

  if (mode & ARM_MCI_TRANSFER_STREAM) {
    /* Stream or SDIO multibyte data transfer enable */
    dctrl |= SDMMC_DCTRL_DTMODE;
  }

  /* Set data block size */
  if (block_size == 512U) {
    sz = 9U;
  }
  else {
    if (block_size > 16384U) {
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
    for (sz = 0U; sz < 14U; sz++) {
      if (block_size & (1UL << sz)) {
        break;
      }
    }
  }

  mci->info->dlen   = block_count * block_size;
  mci->info->dctrl  = dctrl | (sz << 4);

  return (ARM_DRIVER_OK);
}


/**
  \fn            int32_t AbortTransfer (void)
  \brief         Abort current read/write data transfer.
  \return        \ref execution_status
*/
static int32_t AbortTransfer (MCI_RESOURCES *mci) {
  int32_t  status;
  uint32_t mask;

  if ((mci->info->flags & MCI_SETUP) == 0U) { return ARM_DRIVER_ERROR; }

  status = ARM_DRIVER_OK;

  /* Disable interrupts */
  mask = mci->reg->MASK;
  mci->reg->MASK = 0U;

  /* Disable IDMA */
  mci->reg->IDMACTRL &= ~(SDMMC_IDMA_IDMAEN);

  /* Flush FIFO */
  mci->reg->DCTRL |= SDMMC_DCTRL_FIFORST;

  /* Clear data transfer bit */
  mci->reg->DCTRL &= ~(SDMMC_DCTRL_DTEN);

  mci->info->status.command_active  = 0U;
  mci->info->status.transfer_active = 0U;
  mci->info->status.sdio_interrupt  = 0U;
  mci->info->status.ccs             = 0U;

  /* Clear pending interrupts */
  mci->reg->ICR = SDMMC_ICR_BIT_Msk;

  /* Enable interrupts */
  mci->reg->MASK = mask;

  return status;
}


/**
  \fn            int32_t Control (uint32_t control, uint32_t arg)
  \brief         Control MCI Interface.
  \param[in]     control  Operation
  \param[in]     arg      Argument of operation (optional)
  \return        \ref execution_status
*/
static int32_t Control (uint32_t control, uint32_t arg, MCI_RESOURCES *mci) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_TypeDef *port;
  uint32_t val, clkdiv, bps;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

  switch (control) {
    case ARM_MCI_BUS_SPEED:
      /* Determine clock divider and set bus speed */
      clkdiv = ((mci->info->ker_clk - 1U) / arg) + 1U;
      clkdiv = clkdiv & 0x3FFU;

      val  = mci->reg->CLKCR & ~SDMMC_CLKCR_CLKDIV;
      val |= clkdiv >> 1U;

      /* Set new clock divider */
      mci->reg->CLKCR = val;

      bps = mci->info->ker_clk / clkdiv;

      /* Bus speed configured */
      mci->info->flags |= MCI_SETUP;
      return ((int32_t)bps);

    case ARM_MCI_BUS_SPEED_MODE:
      /* Read clock control register */
      val = mci->reg->CLKCR;
      /* Clear DDR bit */
      val &= ~SDMMC_CLKCR_DDR;

      switch (arg) {
        case ARM_MCI_BUS_DEFAULT_SPEED:
          /* Speed mode up to 25MHz */
        case ARM_MCI_BUS_HIGH_SPEED:
          /* Speed mode up to 50MHz */
        case ARM_MCI_BUS_UHS_SDR12:
          /* SDR12:  up to  25MHz,  12.5MB/s: UHS-I 1.8V signaling */
        case ARM_MCI_BUS_UHS_SDR25:
          /* SDR25:  up to  50MHz,  25  MB/s: UHS-I 1.8V signaling */
          val &= ~SDMMC_CLKCR_BUSSPEED;
          break;

        case ARM_MCI_BUS_UHS_DDR50:
          /* DDR50:  up to  50MHz,  50  MB/s: UHS-I 1.8V signaling */
          val |= SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED;
          break;
        case ARM_MCI_BUS_UHS_SDR50:
          /* SDR50:  up to 100MHz,  50  MB/s: UHS-I 1.8V signaling */
        case ARM_MCI_BUS_UHS_SDR104:
          /* SDR104: up to 208MHz, 104  MB/s: UHS-I 1.8V signaling */
          val |= SDMMC_CLKCR_BUSSPEED;
          break;

        default: return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
      /* Set new register value */
      mci->reg->CLKCR = val;
      break;

    case ARM_MCI_BUS_CMD_MODE:
      switch (arg) {
        case ARM_MCI_BUS_CMD_OPEN_DRAIN:
          /* Configure command line in open-drain mode */
          val = GPIO_MODE_AF_OD;
          break;
        case ARM_MCI_BUS_CMD_PUSH_PULL:
          /* Configure command line in push-pull mode */
          val = GPIO_MODE_AF_PP;
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      GPIO_InitStruct.Mode  = val;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Pull  = GPIO_PULLUP;

      #if defined(MX_SDMMC1_CMD_Pin)
      if (mci->reg == SDMMC1) {
        port                      = MX_SDMMC1_CMD_GPIOx;
        GPIO_InitStruct.Pin       = MX_SDMMC1_CMD_GPIO_Pin;
        GPIO_InitStruct.Alternate = MX_SDMMC1_CMD_GPIO_AF;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
      }
      #endif
      #if defined(MX_SDMMC2_CMD_Pin)
      if (mci->reg == SDMMC2) {
        port                      = MX_SDMMC2_CMD_GPIOx;
        GPIO_InitStruct.Pin       = MX_SDMMC2_CMD_GPIO_Pin;
        GPIO_InitStruct.Alternate = MX_SDMMC2_CMD_GPIO_AF;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
      }
      #endif
      break;

    case ARM_MCI_BUS_DATA_WIDTH:
      val = mci->reg->CLKCR & ~SDMMC_CLKCR_WIDBUS;

      switch (arg) {
        case ARM_MCI_BUS_DATA_WIDTH_1:
          break;
        case ARM_MCI_BUS_DATA_WIDTH_4:
          val |= SDMMC_CLKCR_WIDBUS_0;
          break;
        case ARM_MCI_BUS_DATA_WIDTH_8:
          val |= SDMMC_CLKCR_WIDBUS_1;
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      mci->reg->CLKCR = val;
      break;

    case ARM_MCI_CONTROL_CLOCK_IDLE:
      val = mci->reg->CLKCR;
      if (arg) {
        /* Clock generation enabled when idle */
        val &= ~SDMMC_CLKCR_PWRSAV;
      }
      else {
        /* Clock generation disabled when idle */
        val |= SDMMC_CLKCR_PWRSAV;
      }
      mci->reg->CLKCR = val;
      break;

    case ARM_MCI_DATA_TIMEOUT:
      mci->reg->DTIMER = arg;
      break;

    case ARM_MCI_MONITOR_SDIO_INTERRUPT:
      mci->info->status.sdio_interrupt = 0U;
      mci->reg->MASK |= SDMMC_MASK_SDIOITIE;
      break;

    case ARM_MCI_CONTROL_READ_WAIT:
      if (arg) {
        /* Assert read wait */
        mci->info->flags |= MCI_READ_WAIT;
      }
      else {
        /* Clear read wait */
        mci->info->flags &= ~MCI_READ_WAIT;
        mci->reg->DCTRL &= ~SDMMC_DCTRL_RWSTOP;
      }
      break;

    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn            ARM_MCI_STATUS GetStatus (void)
  \brief         Get MCI status.
  \return        MCI status \ref ARM_MCI_STATUS
*/
static ARM_MCI_STATUS GetStatus (MCI_RESOURCES *mci) {
  return mci->info->status;
}


/* SDMMC interrupt handler */
static void SDMMC_IRQHandler (MCI_RESOURCES *mci) {
  uint32_t sta, icr, event, mask;

  event = 0U;
  icr   = 0U;

  /* Read SDMMC interrupt status */
  sta = mci->reg->STA;

  if (sta & SDMMC_STA_ERR_BIT_Msk) {
    /* Check error interrupts */
    if (sta & SDMMC_STA_CCRCFAIL) {
      icr |= SDMMC_ICR_CCRCFAILC;
      /* Command response CRC check failed */
      if (mci->info->flags & MCI_RESP_CRC) {
        mci->info->status.command_error = 1U;

        event |= ARM_MCI_EVENT_COMMAND_ERROR;
      }
      else {
        /* Ignore CRC error and read the response */
        sta |= SDMMC_STA_CMDREND;
      }
    }
    if (sta & SDMMC_STA_DCRCFAIL) {
      icr |= SDMMC_ICR_DCRCFAILC;
      /* Data block CRC check failed */
      mci->info->status.transfer_error = 1U;

      event |= ARM_MCI_EVENT_TRANSFER_ERROR;
    }
    if (sta & SDMMC_STA_CTIMEOUT) {
      icr |= SDMMC_ICR_CTIMEOUTC;
      /* Command response timeout */
      mci->info->status.command_timeout = 1U;

      event |= ARM_MCI_EVENT_COMMAND_TIMEOUT;
    }
    if (sta & SDMMC_STA_DTIMEOUT) {
      icr |= SDMMC_ICR_DTIMEOUTC;
      /* Data timeout */
      mci->info->status.transfer_timeout = 1U;

      event |= ARM_MCI_EVENT_TRANSFER_TIMEOUT;
    }
  }

  if (sta & SDMMC_STA_CMDREND) {
    /* Command response received */
    icr |= SDMMC_ICR_CMDRENDC;

    if (!(sta & SDMMC_STA_BUSYD0)) {
      /* BusyD0 signal not active */
      event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
    }
  }
  if (sta & SDMMC_STA_BUSYD0END) {
    icr |= SDMMC_ICR_BUSYD0ENDC;
    /* BusyD0 signal deactivated */
    event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
  }

  if (event & ARM_MCI_EVENT_COMMAND_COMPLETE) {
    if (mci->info->response) {
      /* Read response registers */
      if (mci->info->flags & MCI_RESP_LONG) {
        mci->info->response[0] = mci->reg->RESP4;
        mci->info->response[1] = mci->reg->RESP3;
        mci->info->response[2] = mci->reg->RESP2;
        mci->info->response[3] = mci->reg->RESP1;
      }
      else {
        mci->info->response[0] = mci->reg->RESP1;
      }
    }
    if (mci->info->flags & MCI_DATA_XFER) {
      mci->info->flags &= ~MCI_DATA_XFER;

      if (mci->info->flags & MCI_READ_WAIT) {
        mci->info->dctrl |= SDMMC_DCTRL_RWSTART;
      }

      /* Start data transfer */
      mci->reg->DLEN  = mci->info->dlen;
      mci->reg->DCTRL = mci->info->dctrl | SDMMC_DCTRL_DTEN;

      mci->info->status.transfer_active = 1U;
    }
  }
  if (sta & SDMMC_STA_CMDSENT) {
    icr |= SDMMC_ICR_CMDSENTC;
    /* Command sent (no response required) */
    event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
  }
  if (sta & SDMMC_STA_DATAEND) {
    icr |= SDMMC_ICR_DATAENDC;
    /* Data end (DCOUNT is zero) */
    event |= ARM_MCI_EVENT_TRANSFER_COMPLETE;

#if (MCI_DCACHE_MAINTENANCE == 1U)
    /* Is DCache enabled */
    if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
      /* Invalidate data cache: ensure data coherency between DMA and CPU */
      SCB_InvalidateDCache_by_Addr ((uint32_t *)mci->reg->IDMABASE0, (int32_t)mci->info->dlen);
    }
#endif
  }
  if (sta & SDMMC_STA_DBCKEND) {
    icr |= SDMMC_ICR_DBCKENDC;
    /* Data block sent/received (CRC check passed) */
  }
  if (sta & SDMMC_STA_SDIOIT) {
    icr |= SDMMC_ICR_SDIOITC;
    /* Disable interrupt (must be re-enabled using Control) */
    mci->reg->MASK &= SDMMC_MASK_SDIOITIE;

    event |= ARM_MCI_EVENT_SDIO_INTERRUPT;
  }

  /* Clear processed interrupts */
  mci->reg->ICR = icr;

  if (event) {
    /* Check for transfer events */
    mask = ARM_MCI_EVENT_TRANSFER_ERROR   |
           ARM_MCI_EVENT_TRANSFER_TIMEOUT |
           ARM_MCI_EVENT_TRANSFER_COMPLETE;
    if (event & mask) {
      mci->info->status.transfer_active = 0U;

      if (mci->info->cb_event) {
        if (event & ARM_MCI_EVENT_TRANSFER_ERROR) {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_ERROR);
        }
        else if (event & ARM_MCI_EVENT_TRANSFER_TIMEOUT) {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_TIMEOUT);
        }
        else {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_COMPLETE);
        }
      }
    }
    /* Check for command events */
    mask = ARM_MCI_EVENT_COMMAND_ERROR   |
           ARM_MCI_EVENT_COMMAND_TIMEOUT |
           ARM_MCI_EVENT_COMMAND_COMPLETE;
    if (event & mask) {
      mci->info->status.command_active = 0U;

      if (mci->info->cb_event) {
        if (event & ARM_MCI_EVENT_COMMAND_ERROR) {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_ERROR);
        }
        else if (event & ARM_MCI_EVENT_COMMAND_TIMEOUT) {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_TIMEOUT);
        }
        else {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_COMPLETE);
        }
      }
    }
    /* Check for SDIO INT event */
    if (event & ARM_MCI_EVENT_SDIO_INTERRUPT) {
      mci->info->status.sdio_interrupt = 1U;

      if (mci->info->cb_event) {
        (mci->info->cb_event)(ARM_MCI_EVENT_SDIO_INTERRUPT);
      }
    }
  }
}


#if defined (MX_SDMMC1)
/* MCI0 Driver wrapper functions */
static ARM_MCI_CAPABILITIES MCI0_GetCapabilities (void) {
  return GetCapabilities (&MCI0_Resources);
}
static int32_t MCI0_Initialize (ARM_MCI_SignalEvent_t cb_event) {
  return Initialize (cb_event, &MCI0_Resources);
}
static int32_t MCI0_Uninitialize (void) {
  return Uninitialize (&MCI0_Resources);
}
static int32_t MCI0_PowerControl (ARM_POWER_STATE state) {
  return PowerControl (state, &MCI0_Resources);
}
static int32_t MCI0_CardPower (uint32_t voltage) {
  return CardPower (voltage, &MCI0_Resources);
}
static int32_t MCI0_ReadCD (void) {
  return ReadCD (&MCI0_Resources);
}
static int32_t MCI0_ReadWP (void) {
  return ReadWP (&MCI0_Resources);
}
static int32_t MCI0_SendCommand (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response) {
  return SendCommand (cmd, arg, flags, response, &MCI0_Resources);
}
static int32_t MCI0_SetupTransfer (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode) {
  return SetupTransfer (data, block_count, block_size, mode, &MCI0_Resources);
}
static int32_t MCI0_AbortTransfer (void) {
  return AbortTransfer (&MCI0_Resources);
}
static int32_t MCI0_Control (uint32_t control, uint32_t arg) {
  return Control (control, arg, &MCI0_Resources);
}
static ARM_MCI_STATUS MCI0_GetStatus (void) {
  return GetStatus (&MCI0_Resources);
}
void SDMMC1_IRQHandler (void) {
  SDMMC_IRQHandler (&MCI0_Resources);
}

/* MCI0 Driver Control Block */
ARM_DRIVER_MCI Driver_MCI0 = {
  GetVersion,
  MCI0_GetCapabilities,
  MCI0_Initialize,
  MCI0_Uninitialize,
  MCI0_PowerControl,
  MCI0_CardPower,
  MCI0_ReadCD,
  MCI0_ReadWP,
  MCI0_SendCommand,
  MCI0_SetupTransfer,
  MCI0_AbortTransfer,
  MCI0_Control,
  MCI0_GetStatus
};
#endif /* MX_MCI0 */


#if defined (MX_SDMMC2)
/* MCI1 Driver wrapper functions */
static ARM_MCI_CAPABILITIES MCI1_GetCapabilities (void) {
  return GetCapabilities (&MCI1_Resources);
}
static int32_t MCI1_Initialize (ARM_MCI_SignalEvent_t cb_event) {
  return Initialize (cb_event, &MCI1_Resources);
}
static int32_t MCI1_Uninitialize (void) {
  return Uninitialize (&MCI1_Resources);
}
static int32_t MCI1_PowerControl (ARM_POWER_STATE state) {
  return PowerControl (state, &MCI1_Resources);
}
static int32_t MCI1_CardPower (uint32_t voltage) {
  return CardPower (voltage, &MCI1_Resources);
}
static int32_t MCI1_ReadCD (void) {
  return ReadCD (&MCI1_Resources);
}
static int32_t MCI1_ReadWP (void) {
  return ReadWP (&MCI1_Resources);
}
static int32_t MCI1_SendCommand (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response) {
  return SendCommand (cmd, arg, flags, response, &MCI1_Resources);
}
static int32_t MCI1_SetupTransfer (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode) {
  return SetupTransfer (data, block_count, block_size, mode, &MCI1_Resources);
}
static int32_t MCI1_AbortTransfer (void) {
  return AbortTransfer (&MCI1_Resources);
}
static int32_t MCI1_Control (uint32_t control, uint32_t arg) {
  return Control (control, arg, &MCI1_Resources);
}
static ARM_MCI_STATUS MCI1_GetStatus (void) {
  return GetStatus (&MCI1_Resources);
}
void SDMMC2_IRQHandler (void) {
  SDMMC_IRQHandler (&MCI1_Resources);
}

/* MCI1 Driver Control Block */
ARM_DRIVER_MCI Driver_MCI1 = {
  GetVersion,
  MCI1_GetCapabilities,
  MCI1_Initialize,
  MCI1_Uninitialize,
  MCI1_PowerControl,
  MCI1_CardPower,
  MCI1_ReadCD,
  MCI1_ReadWP,
  MCI1_SendCommand,
  MCI1_SetupTransfer,
  MCI1_AbortTransfer,
  MCI1_Control,
  MCI1_GetStatus
};
#endif /* MX_MCI1 */

/*! \endcond */
