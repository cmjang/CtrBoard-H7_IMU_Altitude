/*-----------------------------------------------------------------------------
 * Name:    GLCD_743I_EVAL.c
 * Purpose: Graphic LCD interface for STM32H743I-EVAL Board
 * Rev.:    1.1.1
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2019 - 2020 Arm Limited

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
#include "stm32h743i_eval_sdram.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"


/* AMPIRE640480 Timing */    
#define  HSYNC            ( 30)     /* Horizontal synchronization */
#define  HBP              (114)     /* Horizontal back porch */
#define  HFP              ( 16)     /* Horizontal front porch */
#define  VSYNC            (  3)     /* Vertical synchronization */
#define  VBP              ( 32)     /* Vertical back porch */
#define  VFP              ( 10)     /* Vertical front porch */


#ifndef SDRAM_BASE_ADDR
#define SDRAM_BASE_ADDR       0xD0000000
#endif

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


/*---------------------------- Global variables ------------------------------*/

static uint16_t frame_buf[GLCD_WIDTH*GLCD_HEIGHT] __MEMORY_AT(SDRAM_BASE_ADDR);
static uint16_t foreground_color = GLCD_COLOR_BLACK;
static uint16_t background_color = GLCD_COLOR_WHITE;
static GLCD_FONT *active_font    = NULL;
static LTDC_HandleTypeDef LTDC_Handle;


/**
  \fn          int32_t GLCD_Initialize (void)
  \brief       Initialize Graphic LCD
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_Initialize (void) {
  GPIO_InitTypeDef         GPIO_InitStructure;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStructure;
  LTDC_LayerCfgTypeDef     LTDC_LayerCfg;

#if !defined(DATA_IN_ExtSDRAM)
  /* Initialize the SDRAM */
  BSP_SDRAM_Init(0);
#endif

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /* GPIOs configuration */
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
   |  LCD_INT   <-> MFX_GPIO14   |
   ------------------------------------------------------------
   |  LCD_SCL <-> PB6 (I2C1 SCL) | LCD_SDA <-> PB7 (I2C1 SDA) |
   ------------------------------------------------------------
   |  LCD_BL_CTRL <-> PA6 (GPIO) |
   -------------------------------
  */
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
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
  GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                           GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStructure);

  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;

  /* GPIOA PA6 configuration */
#ifdef USE_LCD_BL_CTRL
  GPIO_InitStructure.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

  /* LCD clock configuration */
  /* MB1063: Ampire 640x480 LCD */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/32 = 25Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 25 Mhz */
  RCC_PeriphClkInitStructure.PLL3.PLL3R = 32;
  RCC_PeriphClkInitStructure.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  RCC_PeriphClkInitStructure.PLL3.PLL3M = 5;		
  RCC_PeriphClkInitStructure.PLL3.PLL3N = 160;
  RCC_PeriphClkInitStructure.PLL3.PLL3P = 2;
  RCC_PeriphClkInitStructure.PLL3.PLL3Q = 2;	
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStructure);

  // Enable the LTDC Clock
  __HAL_RCC_LTDC_CLK_ENABLE();

  // LTDC configuration
  LTDC_Handle.Instance = LTDC;

  /* MB1063: Ampire 640x480 LCD */
  /* Timing configuration */
  LTDC_Handle.Init.HorizontalSync     = (HSYNC - 1);
  LTDC_Handle.Init.VerticalSync       = (VSYNC - 1);
  LTDC_Handle.Init.AccumulatedHBP     = (HSYNC + HBP - 1);
  LTDC_Handle.Init.AccumulatedVBP     = (VSYNC + VBP - 1);  
  LTDC_Handle.Init.AccumulatedActiveH = (GLCD_SIZE_Y + VSYNC + VBP - 1);
  LTDC_Handle.Init.AccumulatedActiveW = (GLCD_SIZE_X + HSYNC + HBP - 1);
  LTDC_Handle.Init.TotalHeigh         = (GLCD_SIZE_Y + VSYNC + VBP + VFP - 1);
  LTDC_Handle.Init.TotalWidth         = (GLCD_SIZE_X + HSYNC + HBP + HFP - 1); 
  
  /* Initialize the LCD pixel width and pixel height */
  LTDC_Handle.LayerCfg->ImageWidth  = GLCD_SIZE_X;
  LTDC_Handle.LayerCfg->ImageHeight = GLCD_SIZE_Y;

  // Configure R,G,B component values for LCD background color
  LTDC_Handle.Init.Backcolor.Red   = 0;
  LTDC_Handle.Init.Backcolor.Blue  = 0;
  LTDC_Handle.Init.Backcolor.Green = 0;

  // Polarity
  LTDC_Handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  LTDC_Handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  LTDC_Handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  LTDC_Handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  HAL_LTDC_Init(&LTDC_Handle);

  LTDC_LayerCfg.WindowX0 = 0;
  LTDC_LayerCfg.WindowX1 = GLCD_SIZE_X - 1;
  LTDC_LayerCfg.WindowY0 = 0;
  LTDC_LayerCfg.WindowY1 = GLCD_SIZE_Y - 1;
  LTDC_LayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  LTDC_LayerCfg.Alpha  = 255;
  LTDC_LayerCfg.Alpha0 = 0;
  LTDC_LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  LTDC_LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  LTDC_LayerCfg.FBStartAdress = SDRAM_BASE_ADDR;
  LTDC_LayerCfg.ImageWidth  = GLCD_SIZE_X;
  LTDC_LayerCfg.ImageHeight = GLCD_SIZE_Y;
  LTDC_LayerCfg.Backcolor.Red   = 0;
  LTDC_LayerCfg.Backcolor.Green = 0;
  LTDC_LayerCfg.Backcolor.Blue  = 0;
  HAL_LTDC_ConfigLayer(&LTDC_Handle, &LTDC_LayerCfg, 0);

  /* Turn display and backlight on */
#ifdef USE_LCD_BL_CTRL
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
#endif

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
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Turn display and backlight off */
#ifdef USE_LCD_BL_CTRL
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
#endif

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
