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
 * $Date:        14. April 2023
 * $Revision:    V1.6
 *
 * Driver:       Driver_I2C1/2/3/4/5
 *
 * Configured:   via CubeMX
 * Project:      I2C Driver for STMicroelectronics STM32H7xx
 * -------------------------------------------------------------------------- */

/*! \page stm32h7_i2c I2C

# Revision History

- Version 1.6
  - Updated I2C_GetPeriClock function implementation
- Version 1.5
  - Corrected MasterTransmit and MasterReceive functions when not generating STOP condition (when xfer_pending is "true")
  - Corrected Bus Clear functionality
  - Corrected check if I2C peripheral is enabled in the CubeMx and improved user experience if it is not enabled
  - Added support for DMA and Data Cache handling
  - Updated function to retrieve peripheral clock (simplified)
  - Made variables status and xfer_sz volatile (solved potential LTO problems)
- Version 1.4
  - Added I2C5 interface accessed by Driver_I2C5
- Version 1.3
  - Corrected pin configuration: MX_I2Cx_yyy_GPIO_PuPd replaced with MX_I2Cx_yyy_GPIO_Pu
  - Disabled DMA transfers
- Version 1.2
  - Synchronized to HAL V1.10.0 (HAL_I2C_Master_Seq_Receive_IT() and HAL_I2C_Slave_Seq_Receive_IT())
- Version 1.1
  - Master clock frequency generation corrected
- Version 1.0
  - Initial release

# Capabilities

This driver supports **Master** and **Slave** modes.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_I2C1     | I2C1
Driver_I2C2     | I2C2
Driver_I2C3     | I2C3
Driver_I2C4     | I2C4
Driver_I2C5     | I2C5

# Limitations

Hardware limitations:
 - I2C4 with BDMA can access only SRAM4 memory.
   If BDMA is used on I2C4 ensure that Tx and Rx buffers are be positioned in SRAM4 memory.

# Configuration

## Compile-time

Definitions used for compile-time configuration of this driver are shown in the table below:

Definition                         | Default value | Value | Description
:----------------------------------|:-------------:|:-----:|:-----------
I2C_DCACHE_MAINTENANCE             |     **1**     |   1   | I2C DCache maintenance operations: **enabled**
^                                  |       ^       |   0   | I2C DCache maintenance operations: **disabled**
I2C_DCACHE_DATA_RX_ALIGNMENT       |     **1**     |   1   | I2C DCache data Rx alignment check: **enabled**
^                                  |       ^       |   0   | I2C DCache data Rx alignment check: **disabled**
I2C_DCACHE_DATA_RX_SIZE            |     **1**     |   1   | I2C DCache data Rx size check: **enabled**
^                                  |       ^       |   0   | I2C DCache data Rx size check: **disabled**
I2C_DCACHE_DATA_TX_ALIGNMENT       |     **1**     |   1   | I2C DCache data Tx alignment check: **enabled**
^                                  |       ^       |   0   | I2C DCache data Tx alignment check: **disabled**
I2C_DCACHE_DATA_TX_SIZE            |     **1**     |   1   | I2C DCache data Tx size check: **enabled**
^                                  |       ^       |   0   | I2C DCache data Tx size check: **disabled**
I2C1_ANF_ENABLE                    |     **0**     |   0   | I2C1 analog noise filter: **disabled**
^                                  |       ^       |   1   | I2C1 analog noise filter: **enabled**
I2C2_ANF_ENABLE                    |     **0**     |   0   | I2C2 analog noise filter: **disabled**
^                                  |       ^       |   1   | I2C2 analog noise filter: **enabled**
I2C3_ANF_ENABLE                    |     **0**     |   0   | I2C3 analog noise filter: **disabled**
^                                  |       ^       |   1   | I2C3 analog noise filter: **enabled**
I2C4_ANF_ENABLE                    |     **0**     |   0   | I2C4 analog noise filter: **disabled**
^                                  |       ^       |   1   | I2C4 analog noise filter: **enabled**
I2C5_ANF_ENABLE                    |     **0**     |   0   | I2C5 analog noise filter: **disabled**
^                                  |       ^       |   1   | I2C5 analog noise filter: **enabled**
I2C1_DNF_COEFFICIENT               |     **0**     |   0   | I2C1 digital noise filter coefficient: **disabled**
^                                  |       ^       | 1..15 | I2C1 digital noise filter coefficient number
I2C2_DNF_COEFFICIENT               |     **0**     |   0   | I2C2 digital noise filter coefficient: **disabled**
^                                  |       ^       | 1..15 | I2C2 digital noise filter coefficient number
I2C3_DNF_COEFFICIENT               |     **0**     |   0   | I2C3 digital noise filter coefficient: **disabled**
^                                  |       ^       | 1..15 | I2C3 digital noise filter coefficient number
I2C4_DNF_COEFFICIENT               |     **0**     |   0   | I2C4 digital noise filter coefficient: **disabled**
^                                  |       ^       | 1..15 | I2C4 digital noise filter coefficient number
I2C5_DNF_COEFFICIENT               |     **0**     |   0   | I2C5 digital noise filter coefficient: **disabled**
^                                  |       ^       | 1..15 | I2C5 digital noise filter coefficient number


## STM32CubeMX

The I2C driver requires:
  - **I2C peripheral clock**
  - **I2C** peripheral configured as **I2C**
  - **I2C SCL** and **SDA pins**
  - optional **DMA** configuration for transfers

# STM32H743I-EVAL Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [STM32H743I-EVAL board schematic](https://www.st.com/resource/en/schematic_pack/mb1246-h743-e03_schematic.pdf).

## STM32CubeMX

Required peripherals for the **STM32H743I-EVAL** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
I2C1       | I2C                          | I2C1 Controller
PB6        | Alternate Function Push Pull | I2C1 SCL Pin
PB7        | Alternate Function Push Pull | I2C1 SDA Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PB6      | **I2C1_SCL**
        PB7      | **I2C1_SDA**
       \n

  2. Under **Categories**: **Connectivity** select **I2C1**:

     __Mode__:
       - Mode: **I2C**

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - DMA Settings: configured in later step (under Category: System Core: DMA)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PB6      | I2C1_SCL      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Low**      | n/a       | .
            PB7      | I2C1_SDA      | n/a          | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Low**      | n/a       | .
       \n

  3. Under **Categories**: **System Core** select **DMA**:

     __Configuration__:
       - DMA1, DMA2:
       \n
         Click **Add** button and add DMA Requests as in table below:
            DMA Request | Stream         | Direction            | Priority
            :----------:|:--------------:|:---------------------|:--------
            I2C1_RX     | DMA1 Stream 0  | Peripheral to Memory | Low
            I2C1_TX     | DMA1 Stream 1  | Memory to Peripheral | Low
       \n

  4. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
            :---------------------------------|:-----------:|:-------------------:|:------------:
            DMA1 stream0 global interrupt     | **checked** | 0                   | 0
            DMA1 stream1 global interrupt     | **checked** | 0                   | 0
            I2C1 event interrupt              | **checked** | 0                   | 0
            I2C1 error interrupt              | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            DMA1 stream0 global interrupt     | unchecked   | checked             | checked          | checked
            DMA1 stream1 global interrupt     | unchecked   | checked             | checked          | checked
            I2C1 event interrupt              | unchecked   | checked             | checked          | checked
            I2C1 error interrupt              | unchecked   | checked             | checked          | checked

### Clock Configuration tab

  1. Configure **To I2C1,2,3 (MHz)**: **100**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_I2C1_Init                | I2C1             | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "I2C_STM32H7xx.h"
#include "DCache_STM32H7xx.h"

#ifndef I2C_CUBE_MX_ENABLED
#error "I2C peripheral not enabled in the STM32CubeMX! Please enable I2C peripheral in the STM32CubeMX!"
#endif

#ifdef  I2C_CUBE_MX_ENABLED

#define ARM_I2C_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,6)    /* driver version */

/* Analog noise filter state: 0 - disable, 1 - enable */
#ifndef I2C1_ANF_ENABLE
#define I2C1_ANF_ENABLE                 0
#endif
#ifndef I2C2_ANF_ENABLE
#define I2C2_ANF_ENABLE                 0
#endif
#ifndef I2C3_ANF_ENABLE
#define I2C3_ANF_ENABLE                 0
#endif
#ifndef I2C4_ANF_ENABLE
#define I2C4_ANF_ENABLE                 0
#endif
#ifndef I2C5_ANF_ENABLE
#define I2C5_ANF_ENABLE                 0
#endif

/* Digital noise filter coefficient: 0 - disable, [1:15] - enabled */
#ifndef I2C1_DNF_COEFFICIENT
#define I2C1_DNF_COEFFICIENT            0
#endif
#ifndef I2C2_DNF_COEFFICIENT
#define I2C2_DNF_COEFFICIENT            0
#endif
#ifndef I2C3_DNF_COEFFICIENT
#define I2C3_DNF_COEFFICIENT            0
#endif
#ifndef I2C4_DNF_COEFFICIENT
#define I2C4_DNF_COEFFICIENT            0
#endif
#ifndef I2C5_DNF_COEFFICIENT
#define I2C5_DNF_COEFFICIENT            0
#endif

#ifndef I2C_DCACHE_MAINTENANCE
#define I2C_DCACHE_MAINTENANCE                 (1U)
#endif
#ifndef I2C_DCACHE_DATA_RX_ALIGNMENT
#define I2C_DCACHE_DATA_RX_ALIGNMENT           (1U)
#endif
#ifndef I2C_DCACHE_DATA_RX_SIZE
#define I2C_DCACHE_DATA_RX_SIZE                (1U)
#endif
#ifndef I2C_DCACHE_DATA_TX_ALIGNMENT
#define I2C_DCACHE_DATA_TX_ALIGNMENT           (1U)
#endif
#ifndef I2C_DCACHE_DATA_TX_SIZE
#define I2C_DCACHE_DATA_TX_SIZE                (1U)
#endif

#define DRIVER_DCACHE_MAINTENANCE              (I2C_DCACHE_MAINTENANCE)
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((I2C_DCACHE_DATA_RX_ALIGNMENT == 0U) || (I2C_DCACHE_DATA_RX_SIZE == 0U))
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_RX_USER_GUARANTEED       (0U)
#endif
#if    (DRIVER_DCACHE_MAINTENANCE == 1U) && ((I2C_DCACHE_DATA_TX_ALIGNMENT == 0U) || (I2C_DCACHE_DATA_TX_SIZE == 0U))
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (1U)
#else
#define DRIVER_DCACHE_TX_USER_GUARANTEED       (0U)
#endif

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_I2C_API_VERSION,
  ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = {
  0U,                   /* Does not support 10-bit addressing */
  0U                    /* Reserved bits */
};


#if defined(MX_I2C1)
extern I2C_HandleTypeDef hi2c1;

/* I2C1 Information (Run-Time) */
static I2C_INFO I2C1_Info = { NULL, { 0U, 0U, 0U, 0U, 0U, 0U, 0U }, 0U, 0U, 0U, 0U, NULL, 0U };

/* I2C1 Resources */
static I2C_RESOURCES I2C1_Resources = {
  &hi2c1,
  I2C1,
  {
    MX_I2C1_SCL_GPIOx,
    MX_I2C1_SDA_GPIOx,
    MX_I2C1_SCL_GPIO_Pin,
    MX_I2C1_SDA_GPIO_Pin,
    MX_I2C1_SCL_GPIO_Pu,
    MX_I2C1_SDA_GPIO_Pu,
    MX_I2C1_SCL_GPIO_AF,
    MX_I2C1_SDA_GPIO_AF
  },
  I2C1_ANF_ENABLE,
  I2C1_DNF_COEFFICIENT,
  &I2C1_Info
};

#endif /* MX_I2C1 */

#if defined(MX_I2C2)
extern I2C_HandleTypeDef hi2c2;

/* I2C2 Information (Run-Time) */
static I2C_INFO I2C2_Info = { NULL, { 0U, 0U, 0U, 0U, 0U, 0U, 0U }, 0U, 0U, 0U, 0U, NULL, 0U };

/* I2C2 Resources */
static I2C_RESOURCES I2C2_Resources = {
  &hi2c2,
  I2C2,
  {
    MX_I2C2_SCL_GPIOx,
    MX_I2C2_SDA_GPIOx,
    MX_I2C2_SCL_GPIO_Pin,
    MX_I2C2_SDA_GPIO_Pin,
    MX_I2C2_SCL_GPIO_Pu,
    MX_I2C2_SDA_GPIO_Pu,
    MX_I2C2_SCL_GPIO_AF,
    MX_I2C2_SDA_GPIO_AF
  },
  I2C2_ANF_ENABLE,
  I2C2_DNF_COEFFICIENT,
  &I2C2_Info
};

#endif /* MX_I2C2 */

#if defined(MX_I2C3)
extern I2C_HandleTypeDef hi2c3;

/* I2C3 Information (Run-Time) */
static I2C_INFO I2C3_Info = { NULL, { 0U, 0U, 0U, 0U, 0U, 0U, 0U }, 0U, 0U, 0U, 0U, NULL, 0U };

/* I2C3 Resources */
static I2C_RESOURCES I2C3_Resources = {
  &hi2c3,
  I2C3,
  {
    MX_I2C3_SCL_GPIOx,
    MX_I2C3_SDA_GPIOx,
    MX_I2C3_SCL_GPIO_Pin,
    MX_I2C3_SDA_GPIO_Pin,
    MX_I2C3_SCL_GPIO_Pu,
    MX_I2C3_SDA_GPIO_Pu,
    MX_I2C3_SCL_GPIO_AF,
    MX_I2C3_SDA_GPIO_AF
  },
  I2C3_ANF_ENABLE,
  I2C3_DNF_COEFFICIENT,
  &I2C3_Info
};

#endif /* MX_I2C3 */

#if defined(MX_I2C4)
extern I2C_HandleTypeDef hi2c4;

/* I2C4 Information (Run-Time) */
static I2C_INFO I2C4_Info = { NULL, { 0U, 0U, 0U, 0U, 0U, 0U, 0U }, 0U, 0U, 0U, 0U, NULL, 0U };

/* I2C4 Resources */
static I2C_RESOURCES I2C4_Resources = {
  &hi2c4,
  I2C4,
  {
    MX_I2C4_SCL_GPIOx,
    MX_I2C4_SDA_GPIOx,
    MX_I2C4_SCL_GPIO_Pin,
    MX_I2C4_SDA_GPIO_Pin,
    MX_I2C4_SCL_GPIO_Pu,
    MX_I2C4_SDA_GPIO_Pu,
    MX_I2C4_SCL_GPIO_AF,
    MX_I2C4_SDA_GPIO_AF
  },
  I2C4_ANF_ENABLE,
  I2C4_DNF_COEFFICIENT,
  &I2C4_Info
};

#endif /* MX_I2C4 */

#if defined(MX_I2C5)
extern I2C_HandleTypeDef hi2c5;

/* I2C5 Information (Run-Time) */
static I2C_INFO I2C5_Info = { NULL, { 0U, 0U, 0U, 0U, 0U, 0U, 0U }, 0U, 0U, 0U, 0U, NULL, 0U };

/* I2C5 Resources */
static I2C_RESOURCES I2C5_Resources = {
  &hi2c5,
  I2C5,
  {
    MX_I2C5_SCL_GPIOx,
    MX_I2C5_SDA_GPIOx,
    MX_I2C5_SCL_GPIO_Pin,
    MX_I2C5_SDA_GPIO_Pin,
    MX_I2C5_SCL_GPIO_Pu,
    MX_I2C5_SDA_GPIO_Pu,
    MX_I2C5_SCL_GPIO_AF,
    MX_I2C5_SDA_GPIO_AF
  },
  I2C5_ANF_ENABLE,
  I2C5_DNF_COEFFICIENT,
  &I2C5_Info
};

#endif /* MX_I2C5 */

/* I2C standard timing specification */
static I2C_STD_TIME i2c_spec_standard = {
  100000, /* clk_max    */
  300,    /* tf_max     */
  1000,   /* tr_max     */
  0,      /* hddat_min  */
  3450,   /* vddat_max  */
  250,    /* sudat_min  */
  4700,   /* scll_min   */
  4000,   /* sclh_min   */
};

static I2C_STD_TIME i2c_spec_fast = {
  400000, /* clk_max    */
  300,    /* tf_max     */
  300,    /* tr_max     */
  0,      /* hddat_min  */
  900,    /* vddat_max  */
  100,    /* sudat_min  */
  1300,   /* scll_min   */
  600,    /* sclh_min   */
};

static I2C_STD_TIME i2c_spec_fast_plus = {
  1000000, /* clk_max    */
  100,     /* tf_max     */
  120,     /* tr_max     */
  0,       /* hddat_min  */
  450,     /* vddat_max  */
  50,      /* sudat_min  */
  500,     /* scll_min   */
  260,     /* sclh_min   */
};

/* Private functions */
static I2C_RESOURCES *I2C_GetResources   (I2C_HandleTypeDef *hi2c);
static uint32_t       I2C_GetPeriClock   (I2C_TypeDef *i2c);
static int32_t        I2C_GetSCLRatio    (I2C_CLK_SETUP *setup, I2C_STD_TIME *spec, I2C_TIMING *cfg);
static uint32_t       I2C_GetTimingValue (I2C_CLK_SETUP *setup, I2C_STD_TIME *spec);

/* Retrieve pointer to I2C instance resources */
static I2C_RESOURCES *I2C_GetResources (I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c = NULL;

  #if defined(MX_I2C1)
  if (hi2c->Instance == I2C1) { i2c = &I2C1_Resources; }
  #endif

  #if defined(MX_I2C2)
  if (hi2c->Instance == I2C2) { i2c = &I2C2_Resources; }
  #endif

  #if defined(MX_I2C3)
  if (hi2c->Instance == I2C3) { i2c = &I2C3_Resources; }
  #endif

  #if defined(MX_I2C4)
  if (hi2c->Instance == I2C4) { i2c = &I2C4_Resources; }
  #endif

  #if defined(MX_I2C5)
  if (hi2c->Instance == I2C5) { i2c = &I2C5_Resources; }
  #endif

  return (i2c);
}

/* Get I2C peripheral clock */
static uint32_t I2C_GetPeriClock (I2C_TypeDef *i2c) {
  PLL3_ClocksTypeDef pll3;
  uint32_t src;
  uint32_t hclk, div;
  uint32_t clk = 0U;

  if (i2c == I2C4) {
    src = __HAL_RCC_GET_I2C4_SOURCE();
  }
  else /* I2C1, I2C2, I2C3 */ {
    src = __HAL_RCC_GET_I2C1_SOURCE();
  }

  if ((src == RCC_I2C1CLKSOURCE_D2PCLK1) || (src == RCC_I2C4CLKSOURCE_D3PCLK1)) {
    /* Get HCLK frequency (clock freq after HPRE prescaler) */
    hclk = HAL_RCC_GetHCLKFreq();
    div  = 1U;

    if (i2c == I2C4) {
      /* Determine D3PPRE value */
      if (RCC->D3CFGR & RCC_D3CFGR_D3PPRE_2) {
        div = 2U << ((RCC->D3CFGR >> 4U) & 0x03U);
      }
    }
    else {
      /* Determine D2PPRE1 value */
      if (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1_2) {
        div = 2U << ((RCC->D2CFGR >> 4U) & 0x03U);
      }
    }
    clk = hclk / div;
  }
  else if ((src == RCC_I2C1CLKSOURCE_PLL3) || (src == RCC_I2C4CLKSOURCE_PLL3)) {
    /* Get PLL3 frequencies */
    HAL_RCCEx_GetPLL3ClockFreq(&pll3);

    /* I2C kernel clock runs from PLL3 R divider */
    clk = pll3.PLL3_R_Frequency;
  }
  else if ((src == RCC_I2C1CLKSOURCE_HSI) || (src == RCC_I2C4CLKSOURCE_HSI)) {
    /* Get HSI frequency (64MHz) */
    clk = MX_HSI_VALUE;
  }
  else if ((src == RCC_I2C1CLKSOURCE_CSI) || (src == RCC_I2C4CLKSOURCE_CSI)) {
    /* Get CSI frequency (4MHz) */
    clk = CSI_VALUE;
  }

  return (clk);
}

/* TIMING setup: Evaluate SCL low/high ratio */
static int32_t I2C_GetSCLRatio (I2C_CLK_SETUP *setup, I2C_STD_TIME *spec, I2C_TIMING *cfg) {
  uint32_t clk_max, clk_min;
  uint32_t tpresc, tsync;
  uint32_t tscl, tscll, tsclh;
  uint32_t scll, sclh;
  int32_t  err;

  /* Set minimum bus clock frequency to 80% of max */
  clk_min = (spec->clk_max * 80) / 100;

  /* Convert values to ns */
  clk_max = 1000000000 / clk_min;
  clk_min = 1000000000 / spec->clk_max;
  tsync   = setup->afd_min + setup->dfd + (2 * setup->i2cclk);
  tpresc = (cfg->presc + 1) * setup->i2cclk;

  err = 0;

  /* Evaluate all values of SCLL and SCLH */
  for (scll = 0; scll < I2C_TIMINGR_SCLL_MAX; scll++) {
    tscll = ((scll + 1) * tpresc) + tsync;

    if ((tscll >= spec->scll_min) && (setup->i2cclk < ((tscll - setup->afd_min - setup->dfd) / 4))) {
      /* SCL low meets specification */

      for (sclh = 0; sclh < I2C_TIMINGR_SCLH_MAX; sclh++) {
        tsclh = ((sclh + 1) * tpresc) + tsync;

        if ((tsclh >= spec->sclh_min) && (tsclh > setup->i2cclk)) {
          /* SCL high meets specification */
          tscl = tscll + tsclh;

          if ((tscl >= clk_min) && (tscl <= clk_max)) {
            /* Evaluate SCL period error */
            err = (int32_t)(tscl - setup->busclk);

            if (err >= 0) {
              cfg->sclh = (uint8_t)sclh;
              cfg->scll = (uint8_t)scll;
              return (err);
            }
          }
        }
      }
    }
  }
  /* No solution */
  return (-1);
}

/* TIMING setup: Determine TIMING register settings based on input structures */
static uint32_t I2C_GetTimingValue (I2C_CLK_SETUP *setup, I2C_STD_TIME *spec) {
  I2C_TIMING time;
  uint32_t dnf_en;
  uint32_t presc;
  uint32_t sdadel, sdadel_min, sdadel_max;
  uint32_t scldel, scldel_min;
  uint32_t p, l, a;
  uint32_t timing;
  int32_t  val, err;

  /* Set digital noise filter enabled flag */
  if (setup->dfd > 0U) {
    dnf_en = 1U;
  } else {
    dnf_en = 0U;
  }

  /* SDADEL (max) */
  val = (int32_t)(spec->vddat_max - spec->tr_max - setup->afd_max - ((dnf_en + 4) * setup->i2cclk));

  if (val > 0) {
    sdadel_max = (uint32_t)val;
  } else {
    sdadel_max = 0U;
  }

  /* SDADEL (min) */
  val = (int32_t)(spec->tf_max + spec->hddat_min - setup->afd_min - ((dnf_en + 3) * setup->i2cclk));

  if (val > 0) {
    sdadel_min = (uint32_t)val;
  } else {
    sdadel_min = 0U;
  }

  /* SCLDEL (min) */
  scldel_min = spec->tr_max + spec->sudat_min;

  /* Set timing register max value */
  timing = 0xF0FFFFFF;

  /* Evaluate all values of PRESC, SCLDEL and SDADEL */
  for (p = 0; p < I2C_TIMINGR_PRESC_MAX; p++) {
    presc = (p + 1) * setup->i2cclk;

    for (l = 0; l < I2C_TIMINGR_SCLDEL_MAX; l++) {
      /* tSCLDEL = (SCLDEL + 1) * ((PRESC + 1) * tI2CCLK) */
      scldel = (l + 1) * presc;

      if (scldel >= scldel_min) {
        /* SCLDEL is above low limit, evaluate SDADEL */
        for (a = 0; a < I2C_TIMINGR_SDADEL_MAX; a++) {
          /* tSDADEL = SDADEL * ((PRESC + 1) * tI2CCLK) */
          sdadel = a * presc;

          if ((sdadel >= sdadel_min) && (sdadel <= sdadel_max)) {
            /* Valid presc (p), scldel (l) and sdadel (a) */
            time.presc  = (uint8_t)p;
            time.scldel = (uint8_t)l;
            time.sdadel = (uint8_t)a;

            /* Determine SCLL and SCLH values */
            err = I2C_GetSCLRatio (setup, spec, &time);

            if (err >= 0) {
              if (err < setup->error) {
                /* Truncate error since it will never be bigger than 16-bit */
                setup->error = (uint16_t)err;

                /* Save timing settings */
                timing  = (time.scll   & 0xFFU);
                timing |= (time.sclh   & 0xFFU) <<  8;
                timing |= (time.sdadel & 0x0FU) << 16;
                timing |= (time.scldel & 0x0FU) << 20;
                timing |= (time.presc  & 0x0FU) << 28;
              }
            }
          }
        }
      }
    }
  }

  return (timing);
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

// I2C Driver functions

/**
  \fn          ARM_DRV_VERSION I2C_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION I2CX_GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_I2C_CAPABILITIES I2C_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_I2C_CAPABILITIES
*/
static ARM_I2C_CAPABILITIES I2CX_GetCapabilities (void) {
  return DriverCapabilities;
}


/**
  \fn          int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c)
  \brief       Initialize I2C Interface.
  \param[in]   cb_event  Pointer to \ref ARM_I2C_SignalEvent
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref ARM_I2C_STATUS
*/
static int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c) {

  if (i2c->info->flags & I2C_INIT) { return ARM_DRIVER_OK; }

  i2c->h->Instance = i2c->reg;

  i2c->h->Init.Timing           = 0U;
  i2c->h->Init.OwnAddress1      = 0U;
  i2c->h->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  i2c->h->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  i2c->h->Init.OwnAddress2      = 0U;
  i2c->h->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  i2c->h->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  i2c->h->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

  /* Reset Run-Time information structure */
  memset ((void *)i2c->info, 0x00, sizeof (I2C_INFO));

  i2c->info->cb_event = cb_event;
  i2c->info->flags    = I2C_INIT;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_Uninitialize (I2C_RESOURCES *i2c)
  \brief       De-initialize I2C Interface.
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Uninitialize (I2C_RESOURCES *i2c) {

  i2c->h->Instance = NULL;
  i2c->info->flags = 0U;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t ARM_I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface Power.
  \param[in]   state  Power state
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c) {
  uint32_t val;

  switch (state) {
    case ARM_POWER_OFF:

      if (HAL_I2C_DeInit (i2c->h) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.direction        = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;

      i2c->info->flags &= ~I2C_POWER;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((i2c->info->flags & I2C_INIT)  == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((i2c->info->flags & I2C_POWER) != 0U) {
        return ARM_DRIVER_OK;
      }

      if (HAL_I2C_Init (i2c->h) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      /* Configure analog noise filter */
      if (i2c->anf_enable != 0) {
        val = I2C_ANALOGFILTER_ENABLE;
      } else {
        val = I2C_ANALOGFILTER_DISABLE;
      }

      if (HAL_I2CEx_ConfigAnalogFilter (i2c->h, val) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      /* Configure digital noise filter */
      if (HAL_I2CEx_ConfigDigitalFilter (i2c->h, i2c->dnf_coef) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      if (i2c->h->hdmarx != NULL) {
        i2c->info->flags |= I2C_DMA_RX;
      }
      if (i2c->h->hdmatx != NULL) {
        i2c->info->flags |= I2C_DMA_TX;
      }

      /* Ready for operation */
      i2c->info->flags |= I2C_POWER;
      break;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_MasterTransmit (uint32_t       addr,
                                           const uint8_t *data,
                                           uint32_t       num,
                                           bool           xfer_pending,
                                           I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to send to I2C Slave
  \param[in]   num           Number of data bytes to send
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterTransmit (uint32_t       addr,
                                   const uint8_t *data,
                                   uint32_t       num,
                                   bool           xfer_pending,
                                   I2C_RESOURCES *i2c) {
  HAL_StatusTypeDef status;
  uint32_t buf = (uint32_t)data;
  uint16_t cnt, saddr;
  uint32_t opt;
  uint8_t  tx_dma;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((addr & ~((uint32_t)ARM_I2C_ADDRESS_10BIT | (uint32_t)ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (num > UINT16_MAX) {
    /* HAL does not handle 32-bit count in transfer */
    return ARM_DRIVER_ERROR;
  }

  cnt = (uint16_t)num;

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  saddr = (addr & 0x3FFU);
  if (i2c->h->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT) {
    saddr <<= 1;
  }

  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 0U;
  i2c->info->status.bus_error        = 0U;
  i2c->info->status.arbitration_lost = 0U;

  i2c->info->xfer_sz = cnt;

  if (xfer_pending == true) {           // If transfer should not generate STOP at the end
    if ((i2c->info->flags & I2C_XFER_NO_STOP) == 0U) {  // First transfer without STOP generation
      i2c->info->flags |= I2C_XFER_NO_STOP;
      opt = I2C_FIRST_FRAME;
    } else {                                            // Any further transfer without STOP generation
      opt = I2C_OTHER_FRAME;
    }
  } else {                              // If transfer should generate STOP at the end
    if ((i2c->info->flags & I2C_XFER_NO_STOP) != 0U) {
      // If previous request did not generate STOP after transfer, terminate sequence
      i2c->info->flags &= ~I2C_XFER_NO_STOP;
      opt = I2C_OTHER_AND_LAST_FRAME;
    } else {
      opt = I2C_LAST_FRAME;
    }
  }

  tx_dma = 0U;
  if (i2c->h->hdmatx != NULL) {
    // Determine if DMA should be used for the transfer
    tx_dma = CheckDmaForTx(i2c->h->hdmatx, data, num);
  }

  if (tx_dma != 0U) {
    status = HAL_I2C_Master_Seq_Transmit_DMA(i2c->h, saddr, (uint8_t *)buf, cnt, opt);
  } else {
    status = HAL_I2C_Master_Seq_Transmit_IT (i2c->h, saddr, (uint8_t *)buf, cnt, opt);
  }

  if (status != HAL_OK) {
    i2c->info->status.busy = 0U;
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2C_MasterReceive (uint32_t       addr,
                                          uint8_t       *data,
                                          uint32_t       num,
                                          bool           xfer_pending,
                                          I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[out]  data          Pointer to buffer for data to receive from I2C Slave
  \param[in]   num           Number of data bytes to receive
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterReceive (uint32_t       addr,
                                  uint8_t       *data,
                                  uint32_t       num,
                                  bool           xfer_pending,
                                  I2C_RESOURCES *i2c) {
  HAL_StatusTypeDef status;
  uint16_t cnt, saddr;
  uint32_t opt;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((addr & ~((uint32_t)ARM_I2C_ADDRESS_10BIT | (uint32_t)ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (num > UINT16_MAX) {
    /* HAL does not handle 32-bit count in transfer */
    return ARM_DRIVER_ERROR;
  }

  cnt = (uint16_t)num;

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  saddr = (addr & 0x3FFU);
  if (i2c->h->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT) {
    saddr <<= 1;
  }

  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 1U;
  i2c->info->status.bus_error        = 0U;
  i2c->info->status.arbitration_lost = 0U;

  addr = (addr & 0x3FFU);

  if (i2c->h->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT) {
    addr <<= 1;
  }

  i2c->info->xfer_sz = cnt;
  i2c->info->rx_data = data;

  if (xfer_pending == true) {           // If transfer should not generate STOP at the end
    if ((i2c->info->flags & I2C_XFER_NO_STOP) == 0U) {  // First transfer without STOP generation
      i2c->info->flags |= I2C_XFER_NO_STOP;
      opt = I2C_FIRST_FRAME;
    } else {                                            // Any further transfer without STOP generation
      opt = I2C_OTHER_FRAME;
    }
  } else {                              // If transfer should generate STOP at the end
    if ((i2c->info->flags & I2C_XFER_NO_STOP) != 0U) {
      // If previous request did not generate STOP after transfer, terminate sequence
      i2c->info->flags &= ~I2C_XFER_NO_STOP;
      opt = I2C_OTHER_AND_LAST_FRAME;
    } else {
      opt = I2C_LAST_FRAME;
    }
  }

  i2c->info->rx_dma  = 0U;
  if (i2c->h->hdmarx != NULL) {
    // Determine if DMA should be used for the transfer
    i2c->info->rx_dma = CheckDmaForRx(i2c->h->hdmarx, data, num);
  }

  if (i2c->info->rx_dma != 0U) {
    status = HAL_I2C_Master_Seq_Receive_DMA(i2c->h, saddr, data, cnt, opt);
  } else {
    status = HAL_I2C_Master_Seq_Receive_IT (i2c->h, saddr, data, cnt, opt);
  }

  if (status != HAL_OK) {
    i2c->info->status.busy = 0U;
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Slave.
  \param[in]   data          Pointer to buffer with data to send to I2C Master
  \param[in]   num           Number of data bytes to send
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {
  HAL_StatusTypeDef status;
  uint32_t buf = (uint32_t)data;
  uint16_t cnt;
  uint8_t  tx_dma;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (num > UINT16_MAX) {
    /* HAL does not handle 32-bit count in transfer */
    return ARM_DRIVER_ERROR;
  }

  cnt = (uint16_t)num;

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  if (HAL_I2C_GetState (i2c->h) != HAL_I2C_STATE_LISTEN) {
    return ARM_DRIVER_ERROR;
  }

  i2c->info->status.busy         = 1U;
  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->flags   = I2C_XFER_SET;
  i2c->info->xfer_sz = cnt;

  tx_dma = 0U;
  if (i2c->h->hdmatx != NULL) {
    // Determine if DMA should be used for the transfer
    tx_dma = CheckDmaForTx(i2c->h->hdmatx, data, num);
  }

  if (tx_dma != 0U) {
    status = HAL_I2C_Slave_Seq_Transmit_DMA(i2c->h, (uint8_t *)buf, cnt, I2C_NEXT_FRAME);
  } else {
    status = HAL_I2C_Slave_Seq_Transmit_IT (i2c->h, (uint8_t *)buf, cnt, I2C_NEXT_FRAME);
  }

  if (status == HAL_ERROR) {
    i2c->info->status.busy = 0U;
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Slave.
  \param[out]  data          Pointer to buffer for data to receive from I2C Master
  \param[in]   num           Number of data bytes to receive
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {
  HAL_StatusTypeDef status;
  uint16_t cnt;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (num > UINT16_MAX) {
    /* HAL does not handle 32-bit count in transfer */
    return ARM_DRIVER_ERROR;
  }

  cnt = (uint16_t)num;

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  i2c->info->status.busy         = 1U;
  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->flags   = I2C_XFER_SET;
  i2c->info->xfer_sz = cnt;
  i2c->info->rx_data = data;

  i2c->info->rx_dma  = 0U;
  if (i2c->h->hdmarx != NULL) {
    // Determine if DMA should be used for the transfer
    i2c->info->rx_dma = CheckDmaForRx(i2c->h->hdmarx, data, num);
  }

  if (i2c->info->rx_dma != 0U) {
    status = HAL_I2C_Slave_Seq_Receive_DMA(i2c->h, (uint8_t *)data, cnt, I2C_NEXT_FRAME);
  } else {
    status = HAL_I2C_Slave_Seq_Receive_IT (i2c->h, (uint8_t *)data, cnt, I2C_NEXT_FRAME);
  }

  if (status == HAL_ERROR) {
    i2c->info->status.busy = 0U;
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_GetDataCount (void)
  \brief       Get transferred data count.
  \return      number of data bytes transferred; -1 when Slave is not addressed by Master
*/
static int32_t I2C_GetDataCount (I2C_RESOURCES *i2c) {
  int32_t cnt        = 0;
  int32_t cnt_xfered = 0;
  int8_t  dma_used   = 0;

  if (HAL_I2C_GetState (i2c->h) == HAL_I2C_STATE_LISTEN) {
    cnt = -1;
  } else {
    if (i2c->h->hdmarx != NULL) {
      if (i2c->h->hdmarx->State == HAL_DMA_STATE_BUSY) {
        // If DMA Rx is in progress
        dma_used   = 1;
        cnt_xfered = (int32_t)(__HAL_DMA_GET_COUNTER(i2c->h->hdmarx));
      }
    }
    if (i2c->h->hdmatx != NULL) {
      if (i2c->h->hdmatx->State == HAL_DMA_STATE_BUSY) {
        // If DMA Tx is in progress
        dma_used   = 1;
        cnt_xfered = (int32_t)(__HAL_DMA_GET_COUNTER(i2c->h->hdmatx));
      }
    }
    if (dma_used == 0) {
      // If DMA is not in progress
      cnt_xfered = (int32_t)(i2c->h->XferCount);
    }

    cnt = (int32_t)(i2c->info->xfer_sz) - cnt_xfered;
  }

  return cnt;
}


/**
  \fn          int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   i2c      pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_PinState state;
  uint32_t i, val;
  uint32_t fpclk, fscl;

  I2C_CLK_SETUP  clk_setup;
  I2C_STD_TIME  *clk_spec;

  if ((i2c->info->flags & I2C_POWER) == 0U) {
    /* I2C not powered */
    return ARM_DRIVER_ERROR;
  }

  switch (control) {
    case ARM_I2C_OWN_ADDRESS:
      if (arg == 0) {
        /* Disable slave */
        HAL_I2C_DisableListen_IT (i2c->h);
      }
      else {
        if ((arg & ARM_I2C_ADDRESS_GC) != 0) {
          /* Enable general call */
          i2c->h->Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;
        } else {
          /* Disable general call */
          i2c->h->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        }

        if ((arg & ARM_I2C_ADDRESS_10BIT) != 0) {
          /* Own address is a 10-bit address */
          i2c->h->Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
        } else {
          /* Own address is a 7-bit address */
          i2c->h->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        }

        i2c->h->Init.OwnAddress1 = (arg << 1) & 0x03FFU;

        HAL_I2C_Init (i2c->h);
        HAL_I2C_EnableListen_IT (i2c->h);
      }
      break;

    case ARM_I2C_BUS_SPEED:
      fpclk = I2C_GetPeriClock(i2c->reg);
      if (fpclk == 0U) {
        return ARM_DRIVER_ERROR;
      }
      switch (arg) {
        case ARM_I2C_BUS_SPEED_STANDARD: /* Clock = 100kHz */
          clk_spec = &i2c_spec_standard;
          fscl = 100000;
          break;

        case ARM_I2C_BUS_SPEED_FAST: /* Clock = 400kHz */
          clk_spec = &i2c_spec_fast;
          fscl = 400000;
          break;

        case ARM_I2C_BUS_SPEED_FAST_PLUS: /* Clock = 1MHz */
          clk_spec = &i2c_spec_fast_plus;
          fscl = 1000000;
          break;

        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      /* Determine peripheral and bus clock period (ns) */
      clk_setup.i2cclk = (uint16_t)((1000000000 + (fpclk / 2)) / fpclk);
      clk_setup.busclk = (uint16_t)((1000000000 + (fscl  / 2)) / fscl);

      /* Determine digital filter delay (ns) */
      clk_setup.dfd = clk_setup.i2cclk * i2c->dnf_coef;

      /* Set analog filter delay (ns) */
      clk_setup.afd_min = ((i2c->anf_enable != 0U) ? (I2C_ANALOG_FILTER_DELAY_MIN) : (0));
      clk_setup.afd_max = ((i2c->anf_enable != 0U) ? (I2C_ANALOG_FILTER_DELAY_MAX) : (0));

      /* Set max iteration error */
      clk_setup.error  = 0xFFFF;

      /* Get TIMING register values */
      val = I2C_GetTimingValue (&clk_setup, clk_spec);

      /* Set register values */
      i2c->reg->CR1    &= ~I2C_CR1_PE;
      i2c->reg->TIMINGR = val;
      i2c->reg->CR1    |=  I2C_CR1_PE;
      break;

    case ARM_I2C_BUS_CLEAR:
      /* Configure SCL and SDA pins as GPIO pin */
      GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull  = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_LOW;

      GPIO_InitStruct.Pin = i2c->io.scl_pin;
      HAL_GPIO_Init(i2c->io.scl_port, &GPIO_InitStruct);
      GPIO_InitStruct.Pin = i2c->io.sda_pin;
      HAL_GPIO_Init(i2c->io.sda_port, &GPIO_InitStruct);

      /* Pull SCL and SDA high */
      HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (i2c->io.sda_port, i2c->io.sda_pin, GPIO_PIN_SET);

      HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD);

      for (i = 0U; i < 9U; i++) {
        if (HAL_GPIO_ReadPin (i2c->io.sda_port, i2c->io.sda_pin) == GPIO_PIN_SET) {
          /* Break if Slave released SDA line */
          break;
        }
        /* Clock high */
        HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_SET);
        HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD/2);

        /* Clock low */
        HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_RESET);
        HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD/2);
      }

      /* Check SDA state */
      state = HAL_GPIO_ReadPin (i2c->io.sda_port, i2c->io.sda_pin);

      /* Configure SDA and SCL pins as I2C peripheral pins */
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;

      GPIO_InitStruct.Pin       = i2c->io.scl_pin;
      GPIO_InitStruct.Pull      = i2c->io.scl_pull;
      GPIO_InitStruct.Alternate = i2c->io.scl_af;

      HAL_GPIO_Init (i2c->io.scl_port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin       = i2c->io.sda_pin;
      GPIO_InitStruct.Pull      = i2c->io.sda_pull;
      GPIO_InitStruct.Alternate = i2c->io.sda_af;

      HAL_GPIO_Init (i2c->io.sda_port, &GPIO_InitStruct);

      if (i2c->info->cb_event != NULL) {
        i2c->info->cb_event (ARM_I2C_EVENT_BUS_CLEAR);
      }

      return (state == GPIO_PIN_SET) ? ARM_DRIVER_OK : ARM_DRIVER_ERROR;

    case ARM_I2C_ABORT_TRANSFER:
      if (HAL_I2C_GetMode (i2c->h) == HAL_I2C_MODE_SLAVE) {
        /* Generate NACK when in slave mode */
        __HAL_I2C_GENERATE_NACK (i2c->h);
      } else {
        /* Get slave address */
        i2c->info->abort = 0U;
        val = i2c->reg->CR2 & 0x3FFU;

        HAL_I2C_Master_Abort_IT (i2c->h, (uint16_t)val);

        /* Wait until abort operation complete */
        while (i2c->info->abort == 0);
      }

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.direction        = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;
      break;

    default: return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}


/**
  \fn          ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c)
  \brief       Get I2C status.
  \param[in]   i2c      pointer to I2C resources
  \return      I2C status \ref ARM_I2C_STATUS
*/
static ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c) {
  return (i2c->info->status);
}


/* Master tx transfer completed */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event != NULL) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
  }
}

/* Master Rx transfer completed */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c->info->rx_dma == 3U) {        // If DMA was used for Rx and cache Refresh should be performed
    RefreshDCacheAfterDmaRx(i2c->h->hdmarx, i2c->info->rx_data, i2c->info->xfer_sz);
  }

  if (i2c != NULL) {
    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event != NULL) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
  }
}

/* Slave addressed */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  I2C_RESOURCES *i2c;
  uint32_t event;

  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    i2c->info->status.mode = 0U;

    if ((i2c->info->flags & I2C_XFER_SET) == 0U) {

      if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        /* Master is transmitter, slave enters receiver mode */
        event = ARM_I2C_EVENT_SLAVE_RECEIVE;
        i2c->info->status.direction = 1U;
      }
      else {
        /* Master is receiver, slave enters transmitter mode */
        event = ARM_I2C_EVENT_SLAVE_TRANSMIT;
        i2c->info->status.direction = 0U;
      }

      if (AddrMatchCode == 0U) {
        /* General call address */
        event |= ARM_I2C_EVENT_GENERAL_CALL;
        i2c->info->status.general_call = 1U;
      }

      if (i2c->info->cb_event != NULL) {
        i2c->info->cb_event (event);
      }
    }

    if ((i2c->info->flags & I2C_XFER_SET) == 0) {
      __HAL_I2C_GENERATE_NACK (i2c->h);
      __HAL_I2C_ENABLE_IT (i2c->h, I2C_IT_ADDRI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ERRI);
    }
    else {
      i2c->info->status.busy = 1U;
    }
    __HAL_I2C_CLEAR_FLAG(i2c->h, I2C_FLAG_ADDR);
  }
}

/* Slave tx transfer completed */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    i2c->info->flags &= ~I2C_XFER_SET;

    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event != NULL) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
    /* Re-enable listen mode */
    __HAL_I2C_ENABLE_IT (i2c->h, I2C_IT_ADDRI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ERRI);
  }
}

/* Slave rx transfer completed */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c->info->rx_dma == 3U) {        // If DMA was used for Rx and cache Refresh should be performed
    RefreshDCacheAfterDmaRx(i2c->h->hdmarx, i2c->info->rx_data, i2c->info->xfer_sz);
  }

  if (i2c != NULL) {
    i2c->info->flags &= ~I2C_XFER_SET;

    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event != NULL) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
    /* Re-enable listen mode */
    __HAL_I2C_ENABLE_IT (i2c->h, I2C_IT_ADDRI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ERRI);
  }
}

/* Listen transfer complete */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    /* Re-enable listen mode */
    HAL_I2C_EnableListen_IT (i2c->h);
  }
}

/* Transfer error */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;
  uint32_t err, event;

  err = HAL_I2C_GetError (hi2c);
  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    event = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

    if ((err & HAL_I2C_ERROR_BERR) != 0U) {
      /* Bus error */
      event |= ARM_I2C_EVENT_BUS_ERROR;
      i2c->info->status.bus_error = 1U;
    }

    if ((err & HAL_I2C_ERROR_ARLO) != 0U) {
      /* Arbitration lost */
      event |= ARM_I2C_EVENT_ARBITRATION_LOST;
      i2c->info->status.arbitration_lost = 1U;
    }

    if((err & HAL_I2C_ERROR_AF) != 0U) {
      /* Acknowledge not received */
      if ((hi2c->XferCount == 0) && (hi2c->XferSize > 0)) {
        /* Slave address was not acknowledged */
        event |= ARM_I2C_EVENT_ADDRESS_NACK;
      }
    }

    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event != NULL) {
      i2c->info->cb_event (event);
    }
  }
}

/* Abort completed */
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c) {
  I2C_RESOURCES *i2c;

  i2c = I2C_GetResources (hi2c);

  if (i2c != NULL) {
    i2c->info->abort = 1U;
  }
}


#if defined(MX_I2C1)
/* I2C1 Driver wrapper functions */
static int32_t I2C1_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C1_Resources);
}
static int32_t I2C1_Uninitialize (void) {
  return I2C_Uninitialize(&I2C1_Resources);
}
static int32_t I2C1_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C1_Resources);
}
static int32_t I2C1_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C1_Resources);
}
static int32_t I2C1_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C1_Resources);
}
static int32_t I2C1_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C1_Resources);
}
static int32_t I2C1_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C1_Resources);
}
static int32_t I2C1_GetDataCount (void) {
  return I2C_GetDataCount(&I2C1_Resources);
}
static int32_t I2C1_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C1_Resources);
}
static ARM_I2C_STATUS I2C1_GetStatus (void) {
  return I2C_GetStatus(&I2C1_Resources);
}

/* I2C1 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C1 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C1_Initialize,
  I2C1_Uninitialize,
  I2C1_PowerControl,
  I2C1_MasterTransmit,
  I2C1_MasterReceive,
  I2C1_SlaveTransmit,
  I2C1_SlaveReceive,
  I2C1_GetDataCount,
  I2C1_Control,
  I2C1_GetStatus
};
#endif /* MX_I2C1 */


#if defined(MX_I2C2)
/* I2C2 Driver wrapper functions */
static int32_t I2C2_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C2_Resources);
}
static int32_t I2C2_Uninitialize (void) {
  return I2C_Uninitialize(&I2C2_Resources);
}
static int32_t I2C2_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C2_Resources);
}
static int32_t I2C2_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C2_Resources);
}
static int32_t I2C2_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C2_Resources);
}
static int32_t I2C2_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C2_Resources);
}
static int32_t I2C2_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C2_Resources);
}
static int32_t I2C2_GetDataCount (void) {
  return I2C_GetDataCount(&I2C2_Resources);
}
static int32_t I2C2_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C2_Resources);
}
static ARM_I2C_STATUS I2C2_GetStatus (void) {
  return I2C_GetStatus(&I2C2_Resources);
}

/* I2C2 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C2 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C2_Initialize,
  I2C2_Uninitialize,
  I2C2_PowerControl,
  I2C2_MasterTransmit,
  I2C2_MasterReceive,
  I2C2_SlaveTransmit,
  I2C2_SlaveReceive,
  I2C2_GetDataCount,
  I2C2_Control,
  I2C2_GetStatus
};
#endif /* MX_I2C2 */


#if defined(MX_I2C3)
/* I2C3 Driver wrapper functions */
static int32_t I2C3_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C3_Resources);
}
static int32_t I2C3_Uninitialize (void) {
  return I2C_Uninitialize(&I2C3_Resources);
}
static int32_t I2C3_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C3_Resources);
}
static int32_t I2C3_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C3_Resources);
}
static int32_t I2C3_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C3_Resources);
}
static int32_t I2C3_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C3_Resources);
}
static int32_t I2C3_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C3_Resources);
}
static int32_t I2C3_GetDataCount (void) {
  return I2C_GetDataCount(&I2C3_Resources);
}
static int32_t I2C3_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C3_Resources);
}
static ARM_I2C_STATUS I2C3_GetStatus (void) {
  return I2C_GetStatus(&I2C3_Resources);
}

/* I2C3 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C3 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C3_Initialize,
  I2C3_Uninitialize,
  I2C3_PowerControl,
  I2C3_MasterTransmit,
  I2C3_MasterReceive,
  I2C3_SlaveTransmit,
  I2C3_SlaveReceive,
  I2C3_GetDataCount,
  I2C3_Control,
  I2C3_GetStatus
};
#endif /* MX_I2C3 */


#if defined(MX_I2C4)
/* I2C4 Driver wrapper functions */
static int32_t I2C4_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C4_Resources);
}
static int32_t I2C4_Uninitialize (void) {
  return I2C_Uninitialize(&I2C4_Resources);
}
static int32_t I2C4_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C4_Resources);
}
static int32_t I2C4_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C4_Resources);
}
static int32_t I2C4_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C4_Resources);
}
static int32_t I2C4_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C4_Resources);
}
static int32_t I2C4_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C4_Resources);
}
static int32_t I2C4_GetDataCount (void) {
  return I2C_GetDataCount(&I2C4_Resources);
}
static int32_t I2C4_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C4_Resources);
}
static ARM_I2C_STATUS I2C4_GetStatus (void) {
  return I2C_GetStatus(&I2C4_Resources);
}

/* I2C4 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C4 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C4_Initialize,
  I2C4_Uninitialize,
  I2C4_PowerControl,
  I2C4_MasterTransmit,
  I2C4_MasterReceive,
  I2C4_SlaveTransmit,
  I2C4_SlaveReceive,
  I2C4_GetDataCount,
  I2C4_Control,
  I2C4_GetStatus
};
#endif /* MX_I2C4 */


#if defined(MX_I2C5)
/* I2C5 Driver wrapper functions */
static int32_t I2C5_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C5_Resources);
}
static int32_t I2C5_Uninitialize (void) {
  return I2C_Uninitialize(&I2C5_Resources);
}
static int32_t I2C5_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C5_Resources);
}
static int32_t I2C5_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C5_Resources);
}
static int32_t I2C5_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C5_Resources);
}
static int32_t I2C5_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C5_Resources);
}
static int32_t I2C5_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C5_Resources);
}
static int32_t I2C5_GetDataCount (void) {
  return I2C_GetDataCount(&I2C5_Resources);
}
static int32_t I2C5_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C5_Resources);
}
static ARM_I2C_STATUS I2C5_GetStatus (void) {
  return I2C_GetStatus(&I2C5_Resources);
}

/* I2C5 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C5 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C5_Initialize,
  I2C5_Uninitialize,
  I2C5_PowerControl,
  I2C5_MasterTransmit,
  I2C5_MasterReceive,
  I2C5_SlaveTransmit,
  I2C5_SlaveReceive,
  I2C5_GetDataCount,
  I2C5_Control,
  I2C5_GetStatus
};
#endif /* MX_I2C5 */

#endif /* I2C_CUBE_MX_ENABLED */

/*! \endcond */
