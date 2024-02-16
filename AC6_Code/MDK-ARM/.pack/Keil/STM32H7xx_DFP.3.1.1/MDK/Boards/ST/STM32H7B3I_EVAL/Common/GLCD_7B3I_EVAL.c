/*-----------------------------------------------------------------------------
 * Name:    GLCD_7B3I_EVAL.c
 * Purpose: Graphic LCD interface for STM32H7B3I-EVAL Board
 * Rev.:    1.0.1
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2020 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include "stm32h7b3i_eval.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"

#include "stm32h7b3i_eval_lcd.h"        // Keil.STM32H7B3I-EVAL::Board Support:Drivers:LCD

#if defined DONOTUSESTBSP__
#include "stm32h7b3i_eval_sdram.h"      // Keil.STM32H7B3I-EVAL::Board Support:Drivers:SDRAM

/* RK070ER9427 Timing */
#define  LCD_HSYNC            46U          /* Horizontal synchronization */
#define  LCD_HBP               2U          /* Horizontal back porch */
#define  LCD_HFP             210U          /* Horizontal front porch */
#define  LCD_VSYNC            23U          /* Vertical synchronization */
#define  LCD_VBP               2U          /* Vertical back porch */
#define  LCD_VFP              22U          /* Vertical front porch */

#define LCD_RST_PORT         GPIOA
#define LCD_RST_PIN          GPIO_PIN_2

#define LCD_BL_PORT          GPIOA
#define LCD_BL_PIN           GPIO_PIN_1

#define LCD_DE_PORT          GPIOK
#define LCD_DE_PIN           GPIO_PIN_7
#endif

#define LCD_LAYER0_BASE_ADDR LCD_LAYER_0_ADDRESS    /* LCD layer 0 SDRAM address */


#ifndef      __MEMORY_AT
  #if     (defined (__CC_ARM))
    #define  __MEMORY_AT(x)     __attribute__((at(x)))
  #elif   (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    #define  __MEMORY_AT__(x)   __attribute__((section(".bss.ARM.__AT_"#x)))
    #define  __MEMORY_AT(x)     __MEMORY_AT__(x)
  #else
    #define  __MEMORY_AT(x)
    #warning Position memory containing __MEMORY_AT macro at absolute address!
  #endif
#endif


#if defined DONOTUSESTBSP__
#if !defined DONOTUSELCDBRIGHTNESS__
/*
   Without this configuration the LCD generates some noise!
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
#endif /* defined DONOTUSESTBSP__ */


/*---------------------------- Global variables ------------------------------*/

static uint16_t frame_buf[GLCD_WIDTH * GLCD_HEIGHT] __MEMORY_AT(LCD_LAYER0_BASE_ADDR);
static uint16_t foreground_color = GLCD_COLOR_BLACK;
static uint16_t background_color = GLCD_COLOR_WHITE;
static GLCD_FONT *active_font    = NULL;
#if defined DONOTUSESTBSP__
static LTDC_HandleTypeDef LTDC_Handle;
#endif


/**
  \fn          int32_t GLCD_Initialize (void)
  \brief       Initialize Graphic LCD
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/

int32_t GLCD_Initialize (void) {

#if !defined DONOTUSESTBSP__
  BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LTDC_PIXEL_FORMAT_RGB565, LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);
#else
  GPIO_InitTypeDef         GPIO_InitStructure;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStructure;
  LTDC_LayerCfgTypeDef     LTDC_LayerCfg;

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

  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStructure) != HAL_OK) {
    return -1;
  }


  /* MB1370A: RK070ER9427 800x480 LCD */
  /* Configure LTDC common part */
  LTDC_Handle.Instance = LTDC;

  LTDC_Handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  LTDC_Handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  LTDC_Handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  LTDC_Handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  LTDC_Handle.Init.HorizontalSync     = LCD_HSYNC - 1U;
  LTDC_Handle.Init.AccumulatedHBP     = LCD_HSYNC + LCD_HBP - 1U;
  LTDC_Handle.Init.AccumulatedActiveW = LCD_HSYNC + GLCD_SIZE_X + LCD_HBP - 1U;
  LTDC_Handle.Init.TotalWidth         = LCD_HSYNC + GLCD_SIZE_X + LCD_HBP + LCD_HFP - 1U;
  LTDC_Handle.Init.VerticalSync       = LCD_VSYNC - 1U;
  LTDC_Handle.Init.AccumulatedVBP     = LCD_VSYNC + LCD_VBP - 1U;
  LTDC_Handle.Init.AccumulatedActiveH = LCD_VSYNC + GLCD_SIZE_Y + LCD_VBP - 1U;
  LTDC_Handle.Init.TotalHeigh         = LCD_VSYNC + GLCD_SIZE_Y + LCD_VBP + LCD_VFP - 1U;

  LTDC_Handle.Init.Backcolor.Blue  = 0xFF;
  LTDC_Handle.Init.Backcolor.Green = 0xFF;
  LTDC_Handle.Init.Backcolor.Red   = 0xFF;

  if (HAL_LTDC_Init(&LTDC_Handle) != HAL_OK) {
    return -1;
  }


  /* Before configuring LTDC layer, ensure SDRAM is initialized */
#if !defined(DATA_IN_ExtSDRAM)
  if (BSP_SDRAM_Init(0) != BSP_ERROR_NONE) {
    return -1;
  }
#endif /* DATA_IN_ExtSDRAM */

  /* Configure LTDC Layer 0 */
  LTDC_LayerCfg.WindowX0        = 0;
  LTDC_LayerCfg.WindowX1        = GLCD_SIZE_X;
  LTDC_LayerCfg.WindowY0        = 0;
  LTDC_LayerCfg.WindowY1        = GLCD_SIZE_Y;
  LTDC_LayerCfg.PixelFormat     = LTDC_PIXEL_FORMAT_RGB565;
  LTDC_LayerCfg.Alpha           = 255;
  LTDC_LayerCfg.Alpha0          = 0;
  LTDC_LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  LTDC_LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  LTDC_LayerCfg.FBStartAdress   = LCD_LAYER0_BASE_ADDR;
  LTDC_LayerCfg.ImageWidth      = GLCD_SIZE_X;
  LTDC_LayerCfg.ImageHeight     = GLCD_SIZE_Y;
  LTDC_LayerCfg.Backcolor.Blue  = 0;
  LTDC_LayerCfg.Backcolor.Green = 0;
  LTDC_LayerCfg.Backcolor.Red   = 0;

  if (HAL_LTDC_ConfigLayer(&LTDC_Handle, &LTDC_LayerCfg, 0) != HAL_OK) {
    return -1;
  }

  /* Initialize TIM in PWM mode to control brightness */
#if !defined DONOTUSELCDBRIGHTNESS__
  /* Without this configuration the LCD generates some noise! */
  TIMx_PWM_Init(&LCD_BL_TIM_Handle);
#endif

  /* GLCD should work now :-) */
#endif /* !defined DONOTUSESTBSP__ */

  return 0;
}


/**
  \fn          int32_t GLCD_Uninitialize (void)
  \brief       De-initialize Graphic LCD
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_Uninitialize (void) {

#if !defined DONOTUSESTBSP__
  BSP_LCD_Reset(0);
#else
  GPIO_InitTypeDef GPIO_InitStructure;

  /* De-Assert LCD_DISP pin */
  HAL_GPIO_WritePin(LCD_DE_PORT, LCD_DE_PIN, GPIO_PIN_RESET);

  /* De-Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET);

  LTDC_Handle.Instance = LTDC;

 /* Disable LTDC block */
  __HAL_LTDC_DISABLE(&LTDC_Handle);

  /* DeInit the LTDC */
  HAL_LTDC_DeInit(&LTDC_Handle);

  /* Disable LTDC clock */
  __HAL_RCC_LTDC_CLK_DISABLE();

  /* GPIOI deactivation */
  GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOI, GPIO_InitStructure.Pin);

  /* GPIOJ deactivation */
  GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                           GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  |
                           GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
                           GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOJ, GPIO_InitStructure.Pin);

  /* GPIOK deactivation */
  GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                           GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7;
  HAL_GPIO_DeInit(GPIOK, GPIO_InitStructure.Pin);
#endif

  return 0;
}

/**
  \fn          int32_t GLCD_SetForegroundColor (uint32_t color)
  \brief       Set foreground color
  \param[in]   color  Color value
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetForegroundColor (uint32_t color) {
  foreground_color = (uint16_t)color;
  return 0;
}

/**
  \fn          int32_t GLCD_SetBackgroundColor (uint32_t color)
  \brief       Set background color
  \param[in]   color  Color value
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetBackgroundColor (uint32_t color) {
  background_color = (uint16_t)color;
  return 0;
}

/**
  \fn          int32_t GLCD_ClearScreen (void)
  \brief       Clear screen (with active background color)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_ClearScreen (void) {
  uint32_t  i;
  uint16_t *ptr_frame_buf;

  ptr_frame_buf = frame_buf;
  for (i = 0; i < (GLCD_WIDTH * GLCD_HEIGHT); i++) {
    *ptr_frame_buf++ = background_color;
  }

  return 0;
}

/**
  \fn          int32_t GLCD_SetFont (GLCD_FONT *font)
  \brief       Set active font
  \param[in]   font   Pointer to font structure
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetFont (GLCD_FONT *font) {
  active_font = font;
  return 0;
}

/**
  \fn          int32_t GLCD_DrawPixel (uint32_t x, uint32_t y)
  \brief       Draw pixel (in active foreground color)
  \param[in]   x  x position in pixels (0 = left corner)
  \param[in]   y  y position in pixels (0 = upper corner)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawPixel (uint32_t x, uint32_t y) {

#if (GLCD_LANDSCAPE != 0)
  frame_buf[(y * GLCD_WIDTH) + x] = foreground_color;
#else
  frame_buf[((GLCD_WIDTH - x) * GLCD_HEIGHT) + y] = foreground_color;
#endif

  return 0;
}

/**
  \fn          int32_t GLCD_DrawHLine (uint32_t x, uint32_t y, uint32_t length)
  \brief       Draw horizontal line (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   length Line length
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawHLine (uint32_t x, uint32_t y, uint32_t length) {
  uint32_t dot;

#if (GLCD_LANDSCAPE != 0)
  dot = (y * GLCD_WIDTH) + x;
#else
  dot = ((GLCD_WIDTH - x) * GLCD_HEIGHT) + y;
#endif

  while (length--) {
    frame_buf[dot] = foreground_color;
#if (GLCD_LANDSCAPE != 0)
    dot += 1;
#else
    dot -= GLCD_HEIGHT;
#endif
  }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawVLine (uint32_t x, uint32_t y, uint32_t length)
  \brief       Draw vertical line (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   length Line length in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawVLine (uint32_t x, uint32_t y, uint32_t length) {
  uint32_t dot;

#if (GLCD_LANDSCAPE != 0)
  dot = (y * GLCD_WIDTH) + x;
#else
  dot = ((GLCD_WIDTH - x) * GLCD_HEIGHT) + y;
#endif

  while (length--) {
    frame_buf[dot] = foreground_color;
#if (GLCD_LANDSCAPE != 0)
    dot += GLCD_WIDTH;
#else
    dot += 1;
#endif
  }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawRectangle (uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  \brief       Draw rectangle (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Rectangle width in pixels
  \param[in]   height Rectangle height in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawRectangle (uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

  GLCD_DrawHLine (x,         y,          width);
  GLCD_DrawHLine (x,         y + height, width);
  GLCD_DrawVLine (x,         y,          height);
  GLCD_DrawVLine (x + width, y,          height);

  return 0;
}

/**
  \fn          int32_t GLCD_DrawChar (uint32_t x, uint32_t y, int32_t ch)
  \brief       Draw character (in active foreground color)
  \param[in]   x      x position in pixels (0 = left corner)
  \param[in]   y      y position in pixels (0 = upper corner)
  \param[in]   ch     Character
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawChar (uint32_t x, uint32_t y, int32_t ch) {
  uint32_t i, j;
  uint32_t wb, dot;
  const uint8_t *ptr_ch_bmp;

  if (active_font == NULL) return -1;

  ch        -= active_font->offset;
  wb         = (active_font->width + 7)/8;
  ptr_ch_bmp = active_font->bitmap + ((uint32_t)ch * wb * active_font->height);
#if (GLCD_LANDSCAPE != 0)
  dot        = (y * GLCD_WIDTH) + x;
#else
  dot        = ((GLCD_WIDTH - x) * GLCD_HEIGHT) + y;
#endif

  for (i = 0; i < active_font->height; i++) {
    for (j = 0; j < active_font->width; j++) {
      frame_buf[dot] = (((*ptr_ch_bmp >> (j & 7)) & 1) ? foreground_color : background_color);
#if (GLCD_LANDSCAPE != 0)
      dot += 1;
#else
      dot -= GLCD_HEIGHT;
#endif
      if (((j & 7) == 7) && (j != (active_font->width - 1))) ptr_ch_bmp++;
    }
#if (GLCD_LANDSCAPE != 0)
    dot +=  GLCD_WIDTH - j;
#else
    dot += (GLCD_HEIGHT * j) + 1;
#endif
    ptr_ch_bmp++;
  }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawString (uint32_t x, uint32_t y, const char *str)
  \brief       Draw string (in active foreground color)
  \param[in]   x      x position in pixels (0 = left corner)
  \param[in]   y      y position in pixels (0 = upper corner)
  \param[in]   str    Null-terminated String
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawString (uint32_t x, uint32_t y, const char *str) {

  while (*str) { GLCD_DrawChar(x, y, *str++); x += active_font->width; }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawBargraph (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t val)
  \brief       Draw bargraph (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Full bargraph width in pixels
  \param[in]   height Full bargraph height in pixels
  \param[in]   val    Active bargraph value (maximum value is 100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawBargraph (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t val) {
  uint32_t i, j;
  uint32_t dot;

  val = (val * width) / 100;            /* Scale value                        */
#if (GLCD_LANDSCAPE != 0)
  dot = (y * GLCD_WIDTH) + x;
#else
  dot = ((GLCD_WIDTH - x) * GLCD_HEIGHT) + y;
#endif

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      frame_buf[dot] = ((j >= val) ? background_color : foreground_color);
#if (GLCD_LANDSCAPE != 0)
      dot += 1;
#else
      dot -= GLCD_HEIGHT;
#endif
    }
#if (GLCD_LANDSCAPE != 0)
    dot +=  GLCD_WIDTH - j;
#else
    dot += (GLCD_HEIGHT * j) + 1;
#endif
  }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawBitmap (uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t *bitmap)
  \brief       Draw bitmap (bitmap from BMP file without header)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Bitmap width in pixels
  \param[in]   height Bitmap height in pixels
  \param[in]   bitmap Bitmap data
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawBitmap (uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t *bitmap) {
  uint32_t        i, j;
  uint32_t        dot;
  const uint16_t *ptr_bmp;

  ptr_bmp = ((const uint16_t *)(uint32_t)bitmap) + (width * (height - 1));
#if (GLCD_LANDSCAPE != 0)
  dot     = (y * GLCD_WIDTH) + x;
#else
  dot     = ((GLCD_WIDTH - x) * GLCD_HEIGHT) + y;
#endif

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      frame_buf[dot] = *ptr_bmp++;
#if (GLCD_LANDSCAPE != 0)
      dot += 1;
#else
      dot -= GLCD_HEIGHT;
#endif
    }
#if (GLCD_LANDSCAPE != 0)
    dot +=  GLCD_WIDTH - j;
#else
    dot += (GLCD_HEIGHT * j) + 1;
#endif
    ptr_bmp -= 2 * width;
  }

  return 0;
}

/**
  \fn          int32_t GLCD_VScroll (uint32_t dy)
  \brief       Scroll picture on display vertically
  \param[in]   dy  Scroll size in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_VScroll (uint32_t dy) {
#if (GLCD_LANDSCAPE != 0)
  uint32_t i, len;

  dy *= GLCD_WIDTH;
  len = GLCD_WIDTH * GLCD_HEIGHT - dy;
  for (i = 0; i < len; i++) {
    frame_buf[i] = frame_buf[i+dy];
  }
  for (; i < GLCD_WIDTH * GLCD_HEIGHT; i++) {
    frame_buf[i] = background_color;
  }
#else
  uint32_t i, j;
  uint32_t dot;

  dot = 0;
  for (i = GLCD_SIZE_Y; i != 0; i--) {
    for (j = GLCD_SIZE_X - dy; j != 0; j--) {
      frame_buf[dot] = frame_buf[dot+dy];
      dot++;
    }
    for (j = dy; j != 0; j--) {
      frame_buf[dot] = background_color;
      dot++;
    }
  }
#endif

  return 0;
}

/**
  \fn          int32_t GLCD_FrameBufferAccess (bool enable)
  \brief       Enable or disable direct access to FrameBuffer
  \param[in]   enable Enable or disable request
                - true (1): enable
                - false(0): disable
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t  GLCD_FrameBufferAccess   (bool enable) {
  /* Direct data access is used with direct access to frame buffer            */
  (void)enable;
  return 0;
}

/**
  \fn          uint32_t GLCD_FrameBufferAddress (void)
  \brief       Retrieve FrameBuffer address
  \returns     FrameBuffer address
*/
uint32_t GLCD_FrameBufferAddress (void) {
  return ((uint32_t)frame_buf);
}
