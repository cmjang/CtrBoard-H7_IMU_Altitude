/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.50 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCD_X.c
Purpose     : Port routines for STM32H743
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************
*/

#include "stm32h7xx_hal.h"

#define LCD_RST_PORT         GPIOA
#define LCD_RST_PIN          GPIO_PIN_2

#define LCD_BL_PORT          GPIOA
#define LCD_BL_PIN           GPIO_PIN_1

#define LCD_DE_PORT          GPIOK
#define LCD_DE_PIN           GPIO_PIN_7

#if !defined DONOTUSELCDBRIGHTNESS__
/*
   Without TIM2 PWM configuration the LCD generates some noise!
 */

#include "stm32h7xx_hal_tim.h"

static TIM_HandleTypeDef LCD_BL_TIM_Handle;

static void TIMx_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  GPIO_InitTypeDef gpio_init_structure;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* Timer channel configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF1_TIM2;
  gpio_init_structure.Pin       = LCD_BL_PIN; /* BL_CTRL */

  HAL_GPIO_Init(LCD_BL_PORT, &gpio_init_structure);
}

static void TIMx_PWM_Init(TIM_HandleTypeDef *htim)
{
  TIM_OC_InitTypeDef LCD_TIM_Config;

  /* Timer_Clock = 2 x  APB1_clock = 280 MHz */
  /* PWM_freq = Timer_Clock /(Period x (Prescaler + 1))*/
  /* PWM_freq = 280 MHz /(200 x (27 + 1)) = 50000 Hz*/
  htim->Instance = TIM2;
  (void)HAL_TIM_PWM_DeInit(htim);

  TIMx_PWM_MspInit(htim);

  htim->Init.Prescaler         = ((uint32_t)27);
  htim->Init.Period            = ((uint32_t)200) - 1U;
  htim->Init.ClockDivision     = 0;
  htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim->Init.RepetitionCounter = 0;
  htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  (void)HAL_TIM_PWM_Init(htim);

  /* Common configuration for all channels */
  LCD_TIM_Config.OCMode       = TIM_OCMODE_PWM1;
  LCD_TIM_Config.OCPolarity   = TIM_OCPOLARITY_HIGH;
  LCD_TIM_Config.OCFastMode   = TIM_OCFAST_DISABLE;
  LCD_TIM_Config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  LCD_TIM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  LCD_TIM_Config.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the default pulse value for channel: 50% duty cycle */
  LCD_TIM_Config.Pulse = 100;

  (void)HAL_TIM_PWM_ConfigChannel(&LCD_BL_TIM_Handle, &LCD_TIM_Config, TIM_CHANNEL_2);

  /* Start PWM Timer channel */
  (void)HAL_TIM_PWM_Start(&LCD_BL_TIM_Handle, TIM_CHANNEL_2);
}
#endif /* !defined DONOTUSELCDBRIGHTNESS__ */


/*********************************************************************
*
*       Exported code
*
*********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values
*/
void LCD_X_Init(void) {
  GPIO_InitTypeDef         GPIO_InitStructure;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStructure;

  /*
   +------------------+-------------------+-------------------+
   +                   LCD pins assignment                    +
   +------------------+-------------------+-------------------+
   |  LCD_R0 <-> PI15 |  LCD_G0 <-> PJ7   |  LCD_B0 <-> PJ12  |
   |  LCD_R1 <-> PJ0  |  LCD_G1 <-> PJ8   |  LCD_B1 <-> PJ13  |
   |  LCD_R2 <-> PJ1  |  LCD_G2 <-> PJ9   |  LCD_B2 <-> PJ14  |
   |  LCD_R3 <-> PJ2  |  LCD_G3 <-> PJ10  |  LCD_B3 <-> PJ15  |
   |  LCD_R4 <-> PJ3  |  LCD_G4 <-> PJ11  |  LCD_B4 <-> PK3   |
   |  LCD_R5 <-> PJ4  |  LCD_G5 <-> PK0   |  LCD_B5 <-> PK4   |
   |  LCD_R6 <-> PJ5  |  LCD_G6 <-> PK1   |  LCD_B6 <-> PK5   |
   |  LCD_R7 <-> PJ6  |  LCD_G7 <-> PK2   |  LCD_B7 <-> PK6   |
   ------------------------------------------------------------
   |  LCD_HSYNC <-> PI12         |  LCD_VSYNC <-> PI13        |
   |  LCD_CLK   <-> PI14         |  LCD_DE    <-> PK7         |
   |  LCD_INT   <-> PH2          |                            |
   ------------------------------------------------------------
   |  LCD_BL_CTRL <-> PA1 (GPIO) | LCD_RESET <-> PA2 (GPIO)   |
   ------------------------------------------------------------
   |  LCD_SCL <-> PH4 (I2C1 SCL) | LCD_SDA <-> PH5 (I2C1 SDA) |
   ------------------------------------------------------------
  */

  /* Enable GPIO, LTDC clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /* Configure the LCD control Pins */
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  /* Configure the LCD Reset (LCD_RESET) pin */
  GPIO_InitStructure.Pin = LCD_RST_PIN;
  HAL_GPIO_Init(LCD_RST_PORT , &GPIO_InitStructure);

  /* Configure the LCD Display Enable (LCD_DE) pin */
  GPIO_InitStructure.Pin = LCD_DE_PIN;
  HAL_GPIO_Init(LCD_DE_PORT, &GPIO_InitStructure);

  /* Configure the LCD Backlight (LCD_BL_CTRL) pin */
  GPIO_InitStructure.Pin = LCD_BL_PIN;
  HAL_GPIO_Init(LCD_BL_PORT, &GPIO_InitStructure);

  /* Configure the LTDC Pins */
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF14_LTDC;

  /* GPIOI configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);
  
  /* GPIOJ configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                           GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  |
                           GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
                           GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStructure);

  /* GPIOK configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3 | \
                           GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStructure);
  

  /* Toggle LCD HW Reset (active low) */
  HAL_GPIO_WritePin(LCD_RST_PORT , LCD_RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(LCD_RST_PORT , LCD_RST_PIN, GPIO_PIN_SET);
  HAL_Delay(10);

  /* Assert LCD_DISP  pin */
  HAL_GPIO_WritePin(LCD_DE_PORT, LCD_DE_PIN, GPIO_PIN_SET);

  /* Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);

  /* Toggle LTDC IP SW reset  */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();


  /* Configure LCD clock
     PLL3_VCO Input = HSE_VALUE/PLL3M = 2 Mhz 
     PLL3_VCO Output = PLL3_VCO Input * PLL3N = 400 Mhz 
     PLLLCDCLK = PLL3_VCO Output/PLL3R = 400/12 = 33.333 Mhz 
     LTDC clock frequency = PLLLCDCLK = 33.333 Mhz
   */
  RCC_PeriphClkInitStructure.PLL3.PLL3M =  12U;
  RCC_PeriphClkInitStructure.PLL3.PLL3N = 200U;
  RCC_PeriphClkInitStructure.PLL3.PLL3P =   2U;
  RCC_PeriphClkInitStructure.PLL3.PLL3Q =   2U;
  RCC_PeriphClkInitStructure.PLL3.PLL3R =  12U;
  RCC_PeriphClkInitStructure.PeriphClockSelection = RCC_PERIPHCLK_LTDC;

  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStructure);

#if !defined DONOTUSELCDBRIGHTNESS__
  /* Without this configuration the LCD generates some noise! */
  TIMx_PWM_Init(&LCD_BL_TIM_Handle);
#endif
}

/*********************************************************************
*
*       LCD_X_DisplayOn
*/
void LCD_X_DisplayOn(void) {
  HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_DE_PORT, LCD_DE_PIN, GPIO_PIN_SET);
}

/*********************************************************************
*
*       LCD_X_DisplayOff
*/
void LCD_X_DisplayOff(void) {
  HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DE_PORT, LCD_DE_PIN, GPIO_PIN_RESET);
}

/*************************** End of file ****************************/
