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

** emWin V6.10 - Graphical user interface for embedded applications **
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
File        : LCDConf.c
Purpose     : Display controller configuration

              Display:        STM32H7B3I-EVAL

              Display driver: GUIDRV_Lin

The part between 'DISPLAY CONFIGURATION START' and 'DISPLAY CONFIGURA-
TION END' can be used to configure the following for each layer:

- Color mode
- Layer size
- Layer orientation

Further the background color used on positions without a valid layer
can be set here.

---------------------------END-OF-HEADER------------------------------
*/

#ifdef _RTE_
#include "RTE_Components.h"
#endif
#include "GUI.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"
#include "LCD_X.h"

#include "stm32h7xx_hal.h"

/*********************************************************************
*
*       Supported orientation modes (not to be changed)
*/
#define ROTATION_0   0
#define ROTATION_CW  1
#define ROTATION_180 2
#define ROTATION_CCW 3

/*********************************************************************
*
*       Supported color modes (not to be changed)
*/
//      Color mode               Conversion     Driver (default orientation)
//      ---------------------------------------------
#define COLOR_MODE_ARGB8888 0 // GUICC_M8888I - GUIDRV_LIN_32
#define COLOR_MODE_RGB888   1 // GUICC_M888   - GUIDRV_LIN_24
#define COLOR_MODE_RGB565   2 // GUICC_M565   - GUIDRV_LIN_16
#define COLOR_MODE_ARGB1555 3 // GUICC_M1555I - GUIDRV_LIN_16
#define COLOR_MODE_ARGB4444 4 // GUICC_M4444I - GUIDRV_LIN_16
#define COLOR_MODE_L8       5 // GUICC_8666   - GUIDRV_LIN_8
#define COLOR_MODE_AL44     6 // GUICC_1616I  - GUIDRV_LIN_8
#define COLOR_MODE_AL88     7 // GUICC_88666I - GUIDRV_LIN_16

/*********************************************************************
**********************************************************************
*
*       DISPLAY CONFIGURATION START (TO BE MODIFIED)
*
**********************************************************************
**********************************************************************
*/
/*********************************************************************
*
*       Common
*/
//
// Physical display size
//
#define XSIZE_PHYS 800
#define YSIZE_PHYS 480

//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used (at least 1 buffer)
#define NUM_VSCREENS 2 // Number of virtual  screens to be used (at least 1 screen)

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  NUM_LAYERS
#define NUM_LAYERS 2

//
// Touch screen
//
#ifdef RTE_Graphics_Touchscreen
#define USE_TOUCH   1
#else
#define USE_TOUCH   0
#endif
//
// Touch screen calibration
#define TOUCH_X_MIN 0x0000
#define TOUCH_X_MAX 0x0320
#define TOUCH_Y_MIN 0x0000
#define TOUCH_Y_MAX 0x01E0

//
// Video RAM Address
// buffer size: XSIZE_0 * YSIZE_0 * BYTES_PER_PIXEL_0 * NUM_VSCREENS * NUM_BUFFERS + layer 1 : 800 * 480 * 2
//              XSIZE_1 * YSIZE_1 * BYTES_PER_PIXEL_1 * NUM_VSCREENS * NUM_BUFFERS   layer 2
#define VRAM_ADDR         0xD0200000

//
// DMA2D Buffer Address used for DMA2D color conversion buffers
// buffer size: XSIZE_PHYS * sizeof(U32) * 3 :  800 * 4 * 3  = 9600   0x2580
#define DMA2D_BUFFER_ADDR 0xD0500000

/*********************************************************************
*
*       Layer 0
*/
//
// Color mode layer 0. Should be one of the above defined color modes
//
#define COLOR_MODE_0 COLOR_MODE_RGB565

//
// Size of layer 0. Ignored and replaced by XSIZE_PHYS and YSIZE_PHYS if NUM_LAYERS == 1
//
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS

//
// Orientation of layer 0. Should be one of the above defined display orientations.
//
#define ORIENTATION_0 ROTATION_0

/*********************************************************************
*
*       Layer 1
*/
//
// Color mode layer 1. Should be one of the above defined color modes
//
#define COLOR_MODE_1 COLOR_MODE_RGB565

//
// Size of layer 1
//
#define XSIZE_1 XSIZE_PHYS
#define YSIZE_1 YSIZE_PHYS

//
// Orientation of layer 1. Should be one of the above defined display orientations.
//
#define ORIENTATION_1 ROTATION_0

/*********************************************************************
*
*       Background color shown where no layer is active
*/
#define BK_COLOR GUI_DARKBLUE

/*********************************************************************
**********************************************************************
*
*       DISPLAY CONFIGURATION END
*
**********************************************************************
**********************************************************************
*/

/*********************************************************************
*
*       Automatic selection of driver and color conversion
*/
#if   (COLOR_MODE_0 == COLOR_MODE_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
#elif (COLOR_MODE_0 == COLOR_MODE_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
#elif (COLOR_MODE_0 == COLOR_MODE_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
#elif (COLOR_MODE_0 == COLOR_MODE_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
#elif (COLOR_MODE_0 == COLOR_MODE_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
#elif (COLOR_MODE_0 == COLOR_MODE_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
#elif (COLOR_MODE_0 == COLOR_MODE_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
#elif (COLOR_MODE_0 == COLOR_MODE_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_0 == COLOR_MODE_ARGB8888)
  #define BYTES_PER_PIXEL_0 4
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_0 == COLOR_MODE_RGB888)
  #define BYTES_PER_PIXEL_0 3
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_0 == COLOR_MODE_RGB565)   \
   || (COLOR_MODE_0 == COLOR_MODE_ARGB1555) \
   || (COLOR_MODE_0 == COLOR_MODE_ARGB4444) \
   || (COLOR_MODE_0 == COLOR_MODE_AL88)
  #define BYTES_PER_PIXEL_0 2
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_0 == COLOR_MODE_L8)   \
   || (COLOR_MODE_0 == COLOR_MODE_AL44)
  #define BYTES_PER_PIXEL_0 1
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_8
  #endif
#endif

#if (NUM_LAYERS > 1)

#if   (COLOR_MODE_1 == COLOR_MODE_ARGB8888)
  #define COLOR_CONVERSION_1 GUICC_M8888I
#elif (COLOR_MODE_1 == COLOR_MODE_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
#elif (COLOR_MODE_1 == COLOR_MODE_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
#elif (COLOR_MODE_1 == COLOR_MODE_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
#elif (COLOR_MODE_1 == COLOR_MODE_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
#elif (COLOR_MODE_1 == COLOR_MODE_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
#elif (COLOR_MODE_1 == COLOR_MODE_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
#elif (COLOR_MODE_1 == COLOR_MODE_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
#else
  #error Illegal color mode 1!
#endif

#if   (COLOR_MODE_1 == COLOR_MODE_ARGB8888)
  #define BYTES_PER_PIXEL_1 4
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_1 == COLOR_MODE_RGB888)
  #define BYTES_PER_PIXEL_1 3
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_1 == COLOR_MODE_RGB565)   \
   || (COLOR_MODE_1 == COLOR_MODE_ARGB1555) \
   || (COLOR_MODE_1 == COLOR_MODE_ARGB4444) \
   || (COLOR_MODE_1 == COLOR_MODE_AL88)
  #define BYTES_PER_PIXEL_1 2
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_1 == COLOR_MODE_L8)   \
   || (COLOR_MODE_1 == COLOR_MODE_AL44)
  #define BYTES_PER_PIXEL_1 1
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_8
  #endif
#endif

#else

/*********************************************************************
*
*       Use complete display automatically in case of only one layer
*/
#undef XSIZE_0
#undef YSIZE_0
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS

#endif

/*********************************************************************
*
*       Touch screen defintions
*/

#if ((ORIENTATION_0 == ROTATION_CW) || (ORIENTATION_0 == ROTATION_CCW))
#define WIDTH  YSIZE_PHYS  /* Screen Width (in pixels) */
#define HEIGHT XSIZE_PHYS  /* Screen Height (in pixels)*/
#else
#define WIDTH  XSIZE_PHYS  /* Screen Width (in pixels) */
#define HEIGHT YSIZE_PHYS  /* Screen Height (in pixels)*/
#endif

#if   (ORIENTATION_0 == ROTATION_CW)
  #define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_X)
  #define TOUCH_LEFT   TOUCH_Y_MIN
  #define TOUCH_RIGHT  TOUCH_Y_MAX
  #define TOUCH_TOP    TOUCH_X_MAX
  #define TOUCH_BOTTOM TOUCH_X_MIN
#elif (ORIENTATION_0 == ROTATION_CCW)
  #define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_Y)
  #define TOUCH_LEFT   TOUCH_Y_MAX
  #define TOUCH_RIGHT  TOUCH_Y_MIN
  #define TOUCH_TOP    TOUCH_X_MIN
  #define TOUCH_BOTTOM TOUCH_X_MAX
#elif (ORIENTATION_0 == ROTATION_180)
  #define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
  #define TOUCH_LEFT   TOUCH_X_MAX
  #define TOUCH_RIGHT  TOUCH_X_MIN
  #define TOUCH_TOP    TOUCH_Y_MAX
  #define TOUCH_BOTTOM TOUCH_Y_MIN
#else
  #define DISPLAY_ORIENTATION 0
  #define TOUCH_LEFT   TOUCH_X_MIN
  #define TOUCH_RIGHT  TOUCH_X_MAX
  #define TOUCH_TOP    TOUCH_Y_MIN
  #define TOUCH_BOTTOM TOUCH_Y_MAX
#endif

/*********************************************************************
*
*       Redirect bulk conversion to DMA2D routines
*/
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}

/*********************************************************************
*
*       H/V front/backporch and synchronization width/height
*/
#define HSW  ( 46U)
#define HFP  (210U)
#define HBP  (  2U)
#define VSW  ( 23U)
#define VFP  ( 22U)
#define VBP  (  2U)

/*********************************************************************
*
*       Configuration checking
*/
#if NUM_BUFFERS > 3
  #error More than 3 buffers make no sense and are not supported in this configuration file!
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_BUFFERS
  #define NUM_BUFFERS 1
#else
  #if (NUM_BUFFERS <= 0)
    #error At least one buffer needs to be defined!
  #endif
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens together with multiple buffers are not allowed!
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

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

static const U32 _aAddr[NUM_LAYERS] = {
  VRAM_ADDR,
#if (NUM_LAYERS > 1)
  VRAM_ADDR + XSIZE_0 * YSIZE_0 * BYTES_PER_PIXEL_0 * NUM_VSCREENS * NUM_BUFFERS
#endif
};

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[NUM_LAYERS];
static int _axSize[NUM_LAYERS];
static int _aySize[NUM_LAYERS];
static int _aBytesPerPixels[NUM_LAYERS];

static volatile int _WaitForDMA2D;

//
// Prototypes of DMA2D color conversion routines
//
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

//
// Color conversion routines using DMA2D
//
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_PIXEL_FORMAT_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_PIXEL_FORMAT_ARGB8888) // Internal pixel format of emWin is 32 bit, because of that ARGB8888
//DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_PIXEL_FORMAT_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_PIXEL_FORMAT_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_PIXEL_FORMAT_ARGB4444)

//
// Buffer for DMA2D color conversion, required because hardware does not support overlapping regions
//
static U32 _aBuffer[XSIZE_PHYS * sizeof(U32) * 3] __MEMORY_AT(DMA2D_BUFFER_ADDR);

static U32 * _pBuffer_DMA2D = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 0];
static U32 * _pBuffer_FG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 1];
static U32 * _pBuffer_BG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 2];

static uint32_t _CLUT[256];

//
// Array of color conversions for each layer
//
static const LCD_API_COLOR_CONV * _apColorConvAPI[] = {
  COLOR_CONVERSION_0,
#if NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

//
// Array of orientations for each layer
//
static const int _aOrientation[] = {
  ORIENTATION_0,
#if NUM_LAYERS > 1
  ORIENTATION_1,
#endif
};

static LTDC_HandleTypeDef  LTDC_Handle;
static DMA2D_HandleTypeDef DMA2D_Handle;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelformat
*/
static U32 _GetPixelformat(int LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if (LayerIndex >= (int)GUI_COUNTOF(_apColorConvAPI)) {
    return 0U;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) {
    return LTDC_PIXEL_FORMAT_ARGB8888;
  } else if (pColorConvAPI == GUICC_M888  ) {
    return LTDC_PIXEL_FORMAT_RGB888;
  } else if (pColorConvAPI == GUICC_M565  ) {
    return LTDC_PIXEL_FORMAT_RGB565;
  } else if (pColorConvAPI == GUICC_M1555I) {
    return LTDC_PIXEL_FORMAT_ARGB1555;
  } else if (pColorConvAPI == GUICC_M4444I) {
    return LTDC_PIXEL_FORMAT_ARGB4444;
  } else if (pColorConvAPI == GUICC_8666  ) {
    return LTDC_PIXEL_FORMAT_L8;
  } else if (pColorConvAPI == GUICC_1616I ) {
    return LTDC_PIXEL_FORMAT_AL44;
  } else if (pColorConvAPI == GUICC_88666I) {
    return LTDC_PIXEL_FORMAT_AL88;
  }
  while (1); // Error
}

/*********************************************************************
*
*       _DMA_ExecOperation
*/
static void _DMA_ExecOperation(void) {
  int timeout;

//  SCB_CleanInvalidateDCache();

  //
  // Set Flag which gets cleared when DMA2D transfer is completed
  //
  _WaitForDMA2D = 1;
  //
  // Execute operation
  //
  DMA2D->CR |= DMA2D_CR_START;                      // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  timeout = 10000000;
  while (_WaitForDMA2D && timeout) {
   timeout--;
  }
  if (timeout == 0) {
    DMA2D->CR |= DMA2D_CR_ABORT;                    // Abort
  }
//while (DMA2D->CR & DMA2D_CR_START) {
//  __WFI();                                        // Sleep until next interrupt
//}
}

/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00000000U | (1U << 9);         // Control Register (Memory to memory and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = (U32)OffLineSrc;                 // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = (U32)OffLineDst;                 // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormat;                     // Foreground PFC Control Register (Defines the input pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_Fill
*/
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00030000U | (1U << 9);         // Control Register (Register to memory and TCIE)
  DMA2D->OCOLR   = ColorIndex;                      // Output Color Register (Color to be used)
  //
  // Set up pointers
  //
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->OOR     = (U32)OffLine;                    // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->OPFCCR  = PixelFormat;                     // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_AlphaBlendingBulk
*/
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000U | (1U << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register
  DMA2D->BGOR    = 0;                               // Background Offset Register
  DMA2D->OOR     = 0;                               // Output Offset Register
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;      // Foreground PFC Control Register (Defines the FG pixel format)
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;      // Background PFC Control Register (Defines the BG pixel format)
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;      // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1U;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_MixColors
*
* Purpose:
*   Function for mixing up 2 colors with the given intensity.
*   If the background color is completely transparent the
*   foreground color should be used unchanged.
*/
static LCD_COLOR _DMA_MixColors(LCD_COLOR colorFG, LCD_COLOR colorBG, U8 Intens) {

//  if ((BkColor & 0xFF000000) == 0xFF000000) {
//    return colorFG;
//  }
  *_pBuffer_FG = colorFG ^ 0xFF000000;
  *_pBuffer_BG = colorBG ^ 0xFF000000;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000U | (1U << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)_pBuffer_FG;                // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)_pBuffer_BG;                // Background Memory Address Register
  DMA2D->OMAR    = (U32)_pBuffer_DMA2D;             // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(1 << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)

  //
  // Execute operation
  //
  _DMA_ExecOperation();
//
//  DMA2D->CR     |= DMA2D_CR_START;                  // Control Register (Start operation)
//  //
//  // Wait until transfer is done
//  //
//  while (DMA2D->CR & DMA2D_CR_START) {
//    __WFI();                                        // Sleep until next interrupt
//  }

  return _pBuffer_DMA2D[0] ^ 0xFF000000;
}

/*********************************************************************
*
*       _DMA_MixColorsBulk
*/
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000U | (1U << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_ConvertColor
*/
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000U | (1U << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = 0;                               // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmapL8
*/
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000U | (1U << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffSrc;                          // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffDst;                          // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_L8;            // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_LoadLUT
*/
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems) {
  DMA2D->FGCMAR  = (U32)pColor;                     // Foreground CLUT Memory Address Register
  //
  // Foreground PFC Control Register
  //
  DMA2D->FGPFCCR  = LTDC_PIXEL_FORMAT_RGB888        // Pixel format
                  | ((NumItems - 1) & 0xFF) << 8;   // Number of items to load
  DMA2D->FGPFCCR |= (1U << 5);                      // Start loading
  while( DMA2D->FGPFCCR & (1 << 5) )
  {
    // andbar01: the 8bpp grayscale bitmap will not display otherwise
  }
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa.
*/
static void _InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  U32 blkCnt;    /* Loop counter */

#if !defined (NO_LOOPUNROLL_)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = NumItems >> 2U;
  while (blkCnt > 0U)
  {
    // Part 1
    Color = *(pColorSrc + 0);
    Color = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
          |  (Color & 0x0000FF00)                // Green
          | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
          | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
    *(pColorDst + 0) = Color;

    // Part 2
    Color = *(pColorSrc + 1);
    Color = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
          |  (Color & 0x0000FF00)                // Green
          | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
          | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
    *(pColorDst + 1) = Color;

    // Part 3
    Color = *(pColorSrc + 2);
    Color = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
          |  (Color & 0x0000FF00)                // Green
          | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
          | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
    *(pColorDst + 2) = Color;

    // Part 4
    Color = *(pColorSrc + 3);
    Color = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
          |  (Color & 0x0000FF00)                // Green
          | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
          | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
    *(pColorDst + 3) = Color;

    pColorSrc += 4;
    pColorDst += 4;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = NumItems & 0x3U;
#else
  /* Initialize blkCnt with number of samples */
  blkCnt = NumItems;
#endif /* #if !defined (NOLOOPUNROLL_) */

  while (blkCnt > 0U)
  {
    // Part n
    Color = *pColorSrc++;
    Color = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
          |  (Color & 0x0000FF00)                // Green
          | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
          | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
    *pColorDst++ = Color;

    /* Decrement loop counter */
    blkCnt--;
  }
}

/*********************************************************************
*
*       _InvertAlpha
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*/
static void _InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  U32 blkCnt;    /* Loop counter */

#if !defined (NO_LOOPUNROLL_)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = NumItems >> 2U;
  while (blkCnt > 0U)
  {
    // Part 1
    Color = *(pColorSrc + 0);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 0) = Color;

    // Part 2
    Color = *(pColorSrc + 1);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 1) = Color;

    // Part 3
    Color = *(pColorSrc + 2);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 2) = Color;

    // Part 4
    Color = *(pColorSrc + 3);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 3) = Color;

    pColorSrc += 4;
    pColorDst += 4;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = NumItems & 0x3U;
#else
  /* Initialize blkCnt with number of samples */
  blkCnt = NumItems;
#endif /* #if !defined (NOLOOPUNROLL_) */

  while (blkCnt > 0U)
  {
    // Part n
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha

    /* Decrement loop counter */
    blkCnt--;
  }
}

/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Invert alpha values
  //
  _InvertAlpha(pColorFG, _pBuffer_FG, NumItems);
  _InvertAlpha(pColorBG, _pBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_pBuffer_FG, _pBuffer_BG, _pBuffer_DMA2D, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha(_pBuffer_DMA2D, pColorDst, NumItems);
}

/*********************************************************************
*
*       _DMA_Index2ColorBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given index values to 32 bit colors.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the color array needs to be converted after DMA2D has been used.
*/
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {

  GUI_USE_PARA(SizeOfIndex);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, _pBuffer_DMA2D, PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB(_pBuffer_DMA2D, pColor, NumItems);
}

/*********************************************************************
*
*       _DMA_Color2IndexBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given 32 bit color array to index values.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the given color array needs to be converted before DMA2D can be used.
*/
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {

  GUI_USE_PARA(SizeOfIndex);
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB(pColor, _pBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_pBuffer_DMA2D, pIndex, LTDC_PIXEL_FORMAT_ARGB8888, PixelFormat, NumItems);
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
  U32 y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);

  for (y = 0U; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha(pFG, _pBuffer_FG, xSize);
    _InvertAlpha(pBG, _pBuffer_BG, xSize);
    //
    //
    //
    _DMA_MixColorsBulk(_pBuffer_FG, _pBuffer_BG, _pBuffer_DMA2D, Intens, xSize);
    //
    //
    //
    _InvertAlpha(_pBuffer_DMA2D, pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}

/*********************************************************************
*
*       _LCD_DisplayOn
*/
static void _LCD_DisplayOn(void) {
  //
  // Display On
  //
  __HAL_LTDC_ENABLE(&LTDC_Handle);
  LCD_X_DisplayOn();
}

/*********************************************************************
*
*       _LCD_DisplayOff
*/
static void _LCD_DisplayOff(void) {
  //
  // Display Off
  //
  __HAL_LTDC_DISABLE(&LTDC_Handle);
  LCD_X_DisplayOff();
}

/*********************************************************************
*
*       _LCD_InitController
*/
static void _LCD_InitController(int LayerIndex) {
  LTDC_LayerCfgTypeDef LayerCfg;
  U32 i;
  int xSize, ySize, BitsPerPixel;
  static int Initialized = 0;

  if (LayerIndex >= NUM_LAYERS) {
    return;
  }
  if (Initialized == 0) {
    //
    // Configure the LCD pins and clocking
    //
    LCD_X_Init();
    //
    // Clock configuration
    //
    __LTDC_CLK_ENABLE();  // Enable LTDC Clock
    __DMA2D_CLK_ENABLE(); // Enable DMA2D Clock
    //
    // Initialize LTDC
    //
    HAL_LTDC_Init(&LTDC_Handle); 
    HAL_LTDC_ProgramLineEvent(&LTDC_Handle, 0);
    //
    // Enable LTDC interrupt
    //
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
    //
    // Enable DMA2D interrupt
    //
    __HAL_DMA2D_ENABLE_IT(&DMA2D_Handle, DMA2D_IT_TC);
    HAL_NVIC_EnableIRQ(DMA2D_IRQn);

    Initialized = 1;
  }
  //
  // Layer configuration
  //
  if (LCD_GetSwapXYEx(LayerIndex)) {
    xSize = LCD_GetYSizeEx(LayerIndex);
    ySize = LCD_GetXSizeEx(LayerIndex);
  } else {
    xSize = LCD_GetXSizeEx(LayerIndex);
    ySize = LCD_GetYSizeEx(LayerIndex);
  }
  //
  // Windowing configuration
  //
  LayerCfg.WindowX0    = 0U;
  LayerCfg.WindowX1    = (U32)xSize;
  LayerCfg.WindowY0    = 0U;
  LayerCfg.WindowY1    = (U32)ySize;
  LayerCfg.ImageWidth  = (U32)xSize;
  LayerCfg.ImageHeight = (U32)ySize;
  //
  // Pixel Format configuration
  //
  LayerCfg.PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Alpha constant (255 totally opaque)
  //
  LayerCfg.Alpha  = 255;
  LayerCfg.Alpha0 = 0;
  //
  // Back Color configuration
  //
  LayerCfg.Backcolor.Blue  = 0;
  LayerCfg.Backcolor.Green = 0;
  LayerCfg.Backcolor.Red   = 0;
  //
  // Configure blending factors
  //
  LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  //
  // Input Address configuration
  //
  LayerCfg.FBStartAdress = _aAddr[LayerIndex];
  //
  // Configure Layer
  //
  HAL_LTDC_ConfigLayer(&LTDC_Handle, &LayerCfg, (U32)LayerIndex);
  //
  // Enable LUT on demand
  //
  BitsPerPixel = LCD_GetBitsPerPixelEx(LayerIndex);
  if (BitsPerPixel <= 8) {
    //
    // Enable usage of LUT for all modes with <= 8bpp
    //
    HAL_LTDC_EnableCLUT(&LTDC_Handle, (U32)LayerIndex);
  } else {
    //
    // Optional CLUT initialization for AL88 mode (16bpp)
    //
    if (_apColorConvAPI[LayerIndex] == GUICC_88666I) {
      for (i = 0U; i < 256U; i++) {
        _CLUT[i] = LCD_API_ColorConv_8666.pfIndex2Color(i);
      }
      HAL_LTDC_ConfigCLUT(&LTDC_Handle, _CLUT, 256U, (U32)LayerIndex);
      HAL_LTDC_EnableCLUT(&LTDC_Handle, (U32)LayerIndex);
    }
  }
}

/*********************************************************************
*
*       _LCD_InitHandle
*/
static void _LCD_InitHandle(void) {

  //
  // LTDC configuration
  LTDC_Handle.Instance = LTDC;
  //
  //
  // Polarity configuration
  //
  LTDC_Handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;  // Horizontal synchronization polarity as active low
  LTDC_Handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;  // Vertical synchronization polarity as active low
  LTDC_Handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;  // Data enable polarity as active low
  LTDC_Handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC; // Pixel clock polarity as input pixel clock
  //
  // Configure R,G,B component values for LCD background color
  //
  LTDC_Handle.Init.Backcolor.Red   = (BK_COLOR >>  0) & 0xFF;
  LTDC_Handle.Init.Backcolor.Green = (BK_COLOR >>  8) & 0xFF;
  LTDC_Handle.Init.Backcolor.Blue  = (BK_COLOR >> 16) & 0xFF;
  //
  // Timing configuration
  //
  LTDC_Handle.Init.HorizontalSync     = HSW;                    // Horizontal synchronization width
  LTDC_Handle.Init.VerticalSync       = VSW;                    // Vertical synchronization height
  LTDC_Handle.Init.AccumulatedHBP     = HBP;                    // Accumulated horizontal back porch
  LTDC_Handle.Init.AccumulatedVBP     = VBP;                    // Accumulated vertical back porch
  LTDC_Handle.Init.AccumulatedActiveW = HBP + XSIZE_PHYS;       // Accumulated active width
  LTDC_Handle.Init.AccumulatedActiveH = VBP + YSIZE_PHYS;       // Accumulated active height
  LTDC_Handle.Init.TotalWidth         = HBP + XSIZE_PHYS + HFP; // Total width
  LTDC_Handle.Init.TotalHeigh         = VBP + YSIZE_PHYS + VFP; // Total height

  //
  // DMA2D configuration
  //
  DMA2D_Handle.Instance = DMA2D;
}

/*********************************************************************
*
*       _GetBufferSize
*/
static U32 _GetBufferSize(int LayerIndex) {
  U32 BufferSize;

  BufferSize = (U32)(_axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex]);
  return BufferSize;
}

/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, AddrSrc, AddrDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = _aAddr[LayerIndex] + BufferSize * (U32)IndexSrc;
  AddrDst    = _aAddr[LayerIndex] + BufferSize * (U32)IndexDst;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  _aBufferIndex[LayerIndex] = IndexDst; // After this function has been called all drawing operations are routed to Buffer[IndexDst]!
}

/*********************************************************************
*
*       _LCD_CopyRect
*/
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
  U32 BufferSize, AddrSrc, AddrDst;
  int OffLine;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = _aAddr[LayerIndex] + BufferSize * (U32)_aBufferIndex[LayerIndex] + (U32)(y0 * _axSize[LayerIndex] + x0) * (U32)_aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * (U32)_aBufferIndex[LayerIndex] + (U32)(y1 * _axSize[LayerIndex] + x1) * (U32)_aBytesPerPixels[LayerIndex];
  OffLine = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, OffLine, OffLine);
}

/*********************************************************************
*
*       _LCD_FillRect
*/
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  U32 BufferSize, AddrDst;
  int xSize, ySize;

  if (GUI_GetDrawMode() == GUI_DM_XOR) {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = _aAddr[LayerIndex] + BufferSize * (U32)_aBufferIndex[LayerIndex] + (U32)(y0 * _axSize[LayerIndex] + x0) * (U32)_aBytesPerPixels[LayerIndex];
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/*********************************************************************
*
*       _LCD_DrawBitmap16bpp
*/
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * (U32)_aBufferIndex[LayerIndex] + (U32)(y * _axSize[LayerIndex] + x) * (U32)_aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)(U32)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap8bpp
*/
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * (U32)_aBufferIndex[LayerIndex] + (U32)(y * _axSize[LayerIndex] + x) * (U32)_aBytesPerPixels[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  PixelFormat = _GetPixelformat(LayerIndex);
  _DMA_DrawBitmapL8((void *)(U32)p, (void *)AddrDst, (U32)OffLineSrc, (U32)OffLineDst, PixelFormat, (U32)xSize, (U32)ySize);
}

/*********************************************************************
*
*       _LCD_GetpPalConvTable
*
* Purpose:
*   The emWin function LCD_GetpPalConvTable() normally translates the given colors into
*   index values for the display controller. In case of index based bitmaps without
*   transparent pixels we load the palette only to the DMA2D LUT registers to be
*   translated (converted) during the process of drawing via DMA2D.
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) {
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Convert palette colors from ARGB to ABGR
  //
  _InvertAlpha_SwapRB((U32 *)(U32)pLogPal->pPalEntries, _pBuffer_DMA2D, (U32)pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_pBuffer_DMA2D, (U32)pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _pBuffer_DMA2D;
}

/*********************************************************************
*
*       _LCD_SetOrg
*/
static void _LCD_SetOrg(int LayerIndex, int xPos, int yPos) {
  uint32_t Address;

  Address = _aAddr[LayerIndex] + (U32)(xPos + yPos * _axSize[LayerIndex]) * (U32)_aBytesPerPixels[LayerIndex];
  HAL_LTDC_SetAddress(&LTDC_Handle, Address, (U32)LayerIndex);
}

/*********************************************************************
*
*       _LCD_SetLUTEntry
*/
static void _LCD_SetLUTEntry(int LayerIndex, LCD_COLOR Color, U8 Pos) {
  _CLUT[Pos] = ((Color & 0xFF0000) >> 16) |
                (Color & 0x00FF00)        |
               ((Color & 0x0000FF) << 16);
  HAL_LTDC_ConfigCLUT(&LTDC_Handle, _CLUT, 256, (U32)LayerIndex);
}

/*********************************************************************
*
*       _LCD_SetVis
*/
static void _LCD_SetVis(int LayerIndex, int OnOff) {
  if (OnOff) {
    __HAL_LTDC_LAYER_ENABLE (&LTDC_Handle, (U32)LayerIndex);
  } else {
    __HAL_LTDC_LAYER_DISABLE(&LTDC_Handle, (U32)LayerIndex);
  }
    __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handle); 
}

/*********************************************************************
*
*       _LCD_SetLayerPos
*/
static void _LCD_SetLayerPos(int LayerIndex, int xPos, int yPos) {
  HAL_LTDC_SetWindowPosition(&LTDC_Handle, (U32)xPos, (U32)yPos, (U32)LayerIndex);
}

/*********************************************************************
*
*       _LCD_SetLayerSize
*/
static void _LCD_SetLayerSize(int LayerIndex, int xSize, int ySize) {
  HAL_LTDC_SetWindowSize(&LTDC_Handle, (U32)xSize, (U32)ySize, (U32)LayerIndex);
}

/*********************************************************************
*
*       _LCD_SetLayerAlpha
*/
static void _LCD_SetLayerAlpha(int LayerIndex, int Alpha) {
  HAL_LTDC_SetAlpha(&LTDC_Handle, (U32)(255 - Alpha), (U32)LayerIndex);
}

/*********************************************************************
*
*       _LCD_SetChromaMode
*/
static void _LCD_SetChromaMode(int LayerIndex, int ChromaMode) {
  if (ChromaMode) {
    HAL_LTDC_EnableColorKeying (&LTDC_Handle, (U32)LayerIndex);
  } else {
    HAL_LTDC_DisableColorKeying(&LTDC_Handle, (U32)LayerIndex);
  }
}

/*********************************************************************
*
*       _LCD_SetChroma
*/
static void _LCD_SetChroma(int LayerIndex, LCD_COLOR ChromaMin, LCD_COLOR ChromaMax) {
  uint32_t RGB_Value;

  (void)ChromaMax;

  RGB_Value = ((ChromaMin & 0xFF0000) >> 16) |
               (ChromaMin & 0x00FF00)        |
              ((ChromaMin & 0x0000FF) << 16);
  HAL_LTDC_ConfigColorKeying(&LTDC_Handle, RGB_Value, (U32)LayerIndex);
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       DMA2D_IRQHandler
*
* Purpose:
*   Transfer-complete-interrupt of DMA2D
*/
extern 
void DMA2D_IRQHandler(void);
void DMA2D_IRQHandler(void) {
  __HAL_DMA2D_CLEAR_FLAG(&DMA2D_Handle, DMA2D_FLAG_TC);
  _WaitForDMA2D = 0;
}

/*********************************************************************
*
*       HAL_LTDC_LineEvenCallback
*
* Purpose:
*   Line Event callback for managing multiple buffering
*/
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc) {
  U32 Addr;
  int i;

  for (i = 0; i < NUM_LAYERS; i++) {
    if (_aPendingBuffer[i] >= 0) {
      //
      // Calculate address of buffer to be used  as visible frame buffer
      //
      Addr = _aAddr[i] + (U32)(_axSize[i] * _aySize[i] * _aPendingBuffer[i] * _aBytesPerPixels[i]);
      //
      // Set address
      //
      HAL_LTDC_SetAddress(hltdc, Addr, (U32)i);
      //
      // Reload configuration
      //
      __HAL_LTDC_RELOAD_CONFIG(hltdc);
      //
      // Tell emWin that buffer is used
      //
      GUI_MULTIBUF_ConfirmEx(i, _aPendingBuffer[i]);
      //
      // Clear pending buffer flag of layer
      //
      _aPendingBuffer[i] = -1;
    }
  }

  HAL_LTDC_ProgramLineEvent(hltdc, 0);
}

/*********************************************************************
*
*       LTDC_IRQHandler
*/
extern 
void LTDC_IRQHandler(void);
void LTDC_IRQHandler(void) {
  HAL_LTDC_IRQHandler(&LTDC_Handle);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r = 0;

  /* initalize the various handles necessary for ST HAL */
  if (LTDC_Handle.Instance != LTDC) {     /* just check if one of the handles is initalized */
    _LCD_InitHandle();  
  }

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the display controller and put it into operation.
    //
    _LCD_InitController((int)LayerIndex);
    _LCD_SetVis(1, 0);
    break;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;

    p = (LCD_X_SETORG_INFO *)pData;
    _LCD_SetOrg((int)LayerIndex, p->xPos, p->yPos);
    break;
  }
  case LCD_X_SHOWBUFFER: {
    //
    // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
    //
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
    _aPendingBuffer[LayerIndex] = p->Index;
    break;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;

    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _LCD_SetLUTEntry((int)LayerIndex, p->Color, p->Pos);
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOn();
    break;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOff();
    break;
  }
  case LCD_X_SETVIS: {
    //
    // Required for setting the layer visibility which is passed in the 'OnOff' element of pData
    //
    LCD_X_SETVIS_INFO * p;

    p = (LCD_X_SETVIS_INFO *)pData;
    _LCD_SetVis((int)LayerIndex, p->OnOff);
    break;
  }
  case LCD_X_SETPOS: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETPOS_INFO * p;

    p = (LCD_X_SETPOS_INFO *)pData;
    _LCD_SetLayerPos((int)LayerIndex, p->xPos, p->yPos);
    break;
  }
  case LCD_X_SETSIZE: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETSIZE_INFO * p;
    int xPos, yPos;

    GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
    p = (LCD_X_SETSIZE_INFO *)pData;
    if (LCD_GetSwapXYEx((int)LayerIndex)) {
      _axSize[LayerIndex] = p->ySize;
      _aySize[LayerIndex] = p->xSize;
    } else {
      _axSize[LayerIndex] = p->xSize;
      _aySize[LayerIndex] = p->ySize;
    }
    _LCD_SetLayerSize((int)LayerIndex, xPos, yPos);
    break;
  }
  case LCD_X_SETALPHA: {
    //
    // Required for setting the alpha value which is passed in the 'Alpha' element of pData
    //
    LCD_X_SETALPHA_INFO * p;

    p = (LCD_X_SETALPHA_INFO *)pData;
    _LCD_SetLayerAlpha((int)LayerIndex, p->Alpha);
    break;
  }
  case LCD_X_SETCHROMAMODE: {
    //
    // Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData
    //
    LCD_X_SETCHROMAMODE_INFO * p;

    p = (LCD_X_SETCHROMAMODE_INFO *)pData;
    _LCD_SetChromaMode((int)LayerIndex, p->ChromaMode);
    break;
  }
  case LCD_X_SETCHROMA: {
    //
    // Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData
    //
    LCD_X_SETCHROMA_INFO * p;

    p = (LCD_X_SETCHROMA_INFO *)pData;
    _LCD_SetChroma((int)LayerIndex, p->ChromaMin, p->ChromaMax);
    break;
  }
  default:
    r = -1;
  }

  return r;
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  int i;
  U32 PixelFormat;

  //
  // At first initialize use of multiple buffers on demand
  //
  #if (NUM_BUFFERS > 1)
    for (i = 0; i < NUM_LAYERS; i++) {
      GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  //
  // Set size of 1st layer
  //
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_0, XSIZE_0);
    LCD_SetVSizeEx(0, YSIZE_0 * NUM_VSCREENS, XSIZE_0);
  } else {
    LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
    LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  }
  #if (NUM_LAYERS > 1)
    //
    // Set display driver and color conversion for 2nd layer
    //
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    //
    // Set size of 2nd layer
    //
    if (LCD_GetSwapXYEx(1)) {
      LCD_SetSizeEx (1, YSIZE_0, XSIZE_1);
      LCD_SetVSizeEx(1, YSIZE_1 * NUM_VSCREENS, XSIZE_1);
    } else {
      LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
      LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
    }
  #endif
  //
  // Setting up VRam address and remember pixel size
  //
  for (i = 0; i < NUM_LAYERS; i++) {
    LCD_SetVRAMAddrEx(i, (void *)(_aAddr[i]));                                                 // Setting up VRam address
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;                                       // Remember pixel size
  }
  //
  // Setting up custom functions
  //
  for (i = 0; i < NUM_LAYERS; i++) {
    if (_aOrientation[i] == ROTATION_0) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);               // Set custom function for copying complete buffers (used by multiple buffering) using DMA2D
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);                   // Set custom function for copy rectangular areas (used by GUI_CopyRect()) using DMA2D
      //
      // Set functions for direct color mode layers. Won't work with indexed color modes because of missing LUT for DMA2D destination
      //
      PixelFormat = _GetPixelformat(i);
      if (PixelFormat <= LTDC_PIXEL_FORMAT_ARGB4444) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);                 // Set custom function for filling operations using DMA2D
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp);       // Set up custom drawing routine for index based bitmaps using DMA2D
      }
      //
      // Set up drawing routine for 16bpp bitmap using DMA2D
      //
      if (PixelFormat == LTDC_PIXEL_FORMAT_RGB565) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
      }
    }
  }
  //
  // Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination
  //
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB1555
//GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB565 (does not speed up conversion, default method is slightly faster!)
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB4444
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB888
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB8888
  //
  // Set up custom alpha blending function using DMA2D
  //
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);                                                // Set up custom alpha blending function using DMA2D
  //
  // Set up custom function for translating a bitmap palette into index values.
  // Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
  //
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  //
  // Set up a custom function for mixing up single colors using DMA2D
  //
  GUI_SetFuncMixColors(_DMA_MixColors);
  //
  // Set up a custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);

#if (USE_TOUCH == 1)
  //
  // Set orientation of touch screen
  //
  GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);
  //
  // Calibrate touch screen
  //
  GUI_TOUCH_Calibrate(GUI_COORD_X, 0, WIDTH  - 1, TOUCH_LEFT, TOUCH_RIGHT);
  GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, HEIGHT - 1, TOUCH_TOP,  TOUCH_BOTTOM);
#endif
}

/*************************** End of file ****************************/
