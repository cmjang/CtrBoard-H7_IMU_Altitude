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
File        : LCDConf.c
Purpose     : Display controller configuration

              Display:        STM32H743I-EVAL

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
#define COLOR_MODE_ARGB8888 1 // GUICC_M8888I - GUIDRV_LIN_32
#define COLOR_MODE_RGB888   2 // GUICC_M888   - GUIDRV_LIN_24
#define COLOR_MODE_RGB565   3 // GUICC_M565   - GUIDRV_LIN_16
#define COLOR_MODE_ARGB1555 4 // GUICC_M1555I - GUIDRV_LIN_16
#define COLOR_MODE_ARGB4444 5 // GUICC_M4444I - GUIDRV_LIN_16
#define COLOR_MODE_L8       6 // GUICC_8666   - GUIDRV_LIN_8
#define COLOR_MODE_AL44     7 // GUICC_1616I  - GUIDRV_LIN_8
#define COLOR_MODE_AL88     8 // GUICC_88666I - GUIDRV_LIN_16

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
#define XSIZE_PHYS 640
#define YSIZE_PHYS 480

//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used (at least 1 buffer)
#define NUM_VSCREENS 1 // Number of virtual  screens to be used (at least 1 screen)

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  NUM_LAYERS
#define NUM_LAYERS 1

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
#define TOUCH_X_MAX 0x0280
#define TOUCH_Y_MIN 0x0000
#define TOUCH_Y_MAX 0x01E0

//
// Video RAM Address
//
#define VRAM_ADDR 0xD0200000

//
// DMA2D Buffer Address
//
#define DMA2D_BUFFER_ADDR 0xD0000000

/*********************************************************************
*
*       Layer 0
*/
//
// Color mode layer 0. Should be one of the above defined color modes
//
#define COLOR_MODE_0 COLOR_MODE_RGB888

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
#define COLOR_MODE_1 COLOR_MODE_RGB888

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
  #define BYTE_PER_PIXEL_0    4
#elif (COLOR_MODE_0 == COLOR_MODE_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
  #define BYTE_PER_PIXEL_0    4
#elif (COLOR_MODE_0 == COLOR_MODE_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
  #define BYTE_PER_PIXEL_0    2
#elif (COLOR_MODE_0 == COLOR_MODE_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
  #define BYTE_PER_PIXEL_0    2
#elif (COLOR_MODE_0 == COLOR_MODE_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
  #define BYTE_PER_PIXEL_0    2
#elif (COLOR_MODE_0 == COLOR_MODE_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
  #define BYTE_PER_PIXEL_0    2
#elif (COLOR_MODE_0 == COLOR_MODE_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
  #define BYTE_PER_PIXEL_0    2
#elif (COLOR_MODE_0 == COLOR_MODE_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
  #define BYTE_PER_PIXEL_0    2
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_0 == COLOR_MODE_ARGB8888)
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
  #define BYTE_PER_PIXEL_1    4
#elif (COLOR_MODE_1 == COLOR_MODE_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
  #define BYTE_PER_PIXEL_1    4
#elif (COLOR_MODE_1 == COLOR_MODE_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
  #define BYTE_PER_PIXEL_1    2
#elif (COLOR_MODE_1 == COLOR_MODE_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
  #define BYTE_PER_PIXEL_1    2
#elif (COLOR_MODE_1 == COLOR_MODE_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
  #define BYTE_PER_PIXEL_1    2
#elif (COLOR_MODE_1 == COLOR_MODE_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
  #define BYTE_PER_PIXEL_1    2
#elif (COLOR_MODE_1 == COLOR_MODE_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
  #define BYTE_PER_PIXEL_1    2
#elif (COLOR_MODE_1 == COLOR_MODE_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
  #define BYTE_PER_PIXEL_1    2
#else
  #error Illegal color mode 1!
#endif

#if   (COLOR_MODE_1 == COLOR_MODE_ARGB8888)
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
#define HBP  (143)
#define VBP  ( 34)
#define HSW  ( 28)
#define VSW  (  2)
#define HFP  ( 16)
#define VFP  ( 10)

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
//
// Array for speeding up nibble conversion for A4 bitmaps
//
static const U8 _aMirror[] = {
  0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
  0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1,
  0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xA2, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
  0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
  0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4,
  0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xA5, 0xB5, 0xC5, 0xD5, 0xE5, 0xF5,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
  0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8,
  0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xA9, 0xB9, 0xC9, 0xD9, 0xE9, 0xF9,
  0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A, 0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA,
  0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
  0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C, 0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
  0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D, 0x6D, 0x7D, 0x8D, 0x9D, 0xAD, 0xBD, 0xCD, 0xDD, 0xED, 0xFD,
  0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0x8E, 0x9E, 0xAE, 0xBE, 0xCE, 0xDE, 0xEE, 0xFE,
  0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
};


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

#if (NUM_LAYERS == 2)
#define VRAM_SIZE \
 ((XSIZE_0 * YSIZE_0 * BYTE_PER_PIXEL_0 * NUM_VSCREENS * NUM_BUFFERS) + \
  (XSIZE_1 * YSIZE_1 * BYTE_PER_PIXEL_1 * NUM_VSCREENS * NUM_BUFFERS))
#else
#define VRAM_SIZE \
  (XSIZE_0 * YSIZE_0 * BYTE_PER_PIXEL_0 * NUM_VSCREENS * NUM_BUFFERS)
#endif

static const U32 _aAddr[NUM_LAYERS] = {
  VRAM_ADDR,
#if (NUM_LAYERS > 1)
  VRAM_ADDR + XSIZE_0 * YSIZE_0 * BYTE_PER_PIXEL_0 * NUM_VSCREENS * NUM_BUFFERS
#endif
};

static volatile int _aPendingBuffer[2] = { -1, -1 };  // Important: Needs to be volatile

static int _aBufferIndex[NUM_LAYERS];
static int _axSize[NUM_LAYERS];
static int _aySize[NUM_LAYERS];
static int _aBytesPerPixels[NUM_LAYERS];

static int _SupressCopyBuffer;

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

#if (GUI_USE_ARGB == 0)
static U32 _aBuffer[XSIZE_PHYS * sizeof(U32) * 3] __MEMORY_AT(DMA2D_BUFFER_ADDR);

static U32 * _pBuffer_DMA2D = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 0];
static U32 * _pBuffer_FG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 1];
static U32 * _pBuffer_BG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 2];
#else
static U32 _aBuffer[40 * 40] __MEMORY_AT(DMA2D_BUFFER_ADDR);  // Only required for drawing AA4 characters
#endif

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
static U32 _GetPixelformat(unsigned LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if (LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) {
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
  //
  // Invalidate and clean the data cache before executing the DMA2D operation.
  // Otherwise we would have artifacts on the LCD.
  //
  SCB_CleanInvalidateDCache();
  //
  // Execute operation
  //
  DMA2D->CR |= DMA2D_CR_START;                      // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
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
*       _DMA_DrawBitmap
*/
static void _DMA_DrawBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormatSrc, int PixelFormatDst) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000U | (1U << 9);         // Control Register (Memory-to-memory with PFC and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->BGMAR   = (U32)pDst;                       // Background Memory Address Register (Destination address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->BGOR    = OffLineDst;                      // Background Offset Register (Destination line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output     PFC Control Register (Defines the output pixel format)
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
*       _DMA_DrawAlphaBitmap
*/
static void _DMA_DrawAlphaBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormat) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000U | (1U << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->BGMAR   = (U32)pDst;                       // Background Memory Address Register (Destination address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->BGOR    = OffLineDst;                      // Background Offset Register (Destination line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;      // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->BGPFCCR = PixelFormat;                     // Background PFC Control Register (Defines the destination pixel format)
  DMA2D->OPFCCR  = PixelFormat;                     // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW
/*********************************************************************
*
*       _DMA_CopyRGB565
*/
static void _DMA_CopyRGB565(const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
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
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;        // Foreground PFC Control Register (Defines the input pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}
#endif

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
//  U32 ColorDst;

#if (GUI_USE_ARGB == 0)
//  colorFG ^= 0xFF000000;
//  colorBG ^= 0xFF000000;
  _aBuffer[0] = colorFG ^ 0xFF000000;
  _aBuffer[1] = colorBG ^ 0xFF000000;
#else
  _aBuffer[0] = colorFG;
  _aBuffer[1] = colorBG;
#endif
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000U | (1U << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
//  DMA2D->FGMAR   = (U32)&Color;                     // Foreground Memory Address Register
//  DMA2D->BGMAR   = (U32)&BkColor;                   // Background Memory Address Register
//  DMA2D->OMAR    = (U32)&ColorDst;                  // Output Memory Address Register (Destination address)
  DMA2D->FGMAR   = (U32)&_aBuffer[0];               // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)&_aBuffer[1];               // Background Memory Address Register
  DMA2D->OMAR    = (U32)&_aBuffer[2];               // Output Memory Address Register (Destination address)
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

#if (GUI_USE_ARGB == 0)
//  ColorDst ^= 0xFF000000;
  _aBuffer[2] ^= 0xFF000000;
#endif
//  return ColorDst;
  return _aBuffer[2];
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
*       _DMA_DrawBitmapA4
*/
static int _DMA_DrawBitmapA4(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  U8 * pRD;
  U8 * pWR;
  U32 NumBytes, Color, Index;

  //
  // Check size of conversion buffer
  //
  NumBytes = (((xSize + 1) & (U32)~1) * ySize) >> 1;
  if ((NumBytes > sizeof(_aBuffer)) || (NumBytes == 0)) {
    return 1;
  }
  //
  // Conversion (swapping nibbles)
  //
  pWR = (U8 *)_aBuffer;
  pRD = (U8 *)pSrc;
  do {
    *pWR++ = _aMirror[*pRD++];
  } while (--NumBytes);
  //
  // Get current drawing color (ABGR)
  //
  Index = LCD_GetColorIndex();
  Color = LCD_Index2Color(Index);
  //
  // Set up operation mode
  //
  DMA2D->CR = 0x00020000U | (1U << 9);
  //
  // Set up source
  //
#if (GUI_USE_ARGB == 0)
  DMA2D->FGCOLR  = ((Color & 0xFF) << 16)  // Red
                 |  (Color & 0xFF00)       // Green
                 | ((Color >> 16) & 0xFF); // Blue
#else
  DMA2D->FGCOLR  = Color;
#endif
  DMA2D->FGMAR   = (U32)_aBuffer;
  DMA2D->FGOR    = 0;
  DMA2D->FGPFCCR = 0xA;                    // A4 bitmap
  DMA2D->NLR     = (U32)((xSize + OffSrc) << 16) | ySize;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->BGOR    = OffDst - OffSrc;
  DMA2D->BGPFCCR = PixelFormatDst;
  DMA2D->OMAR    = DMA2D->BGMAR;
  DMA2D->OOR     = DMA2D->BGOR;
  DMA2D->OPFCCR  = DMA2D->BGPFCCR;
  //
  // Execute operation
  //
  _DMA_ExecOperation();
  return 0;
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

  while( DMA2D->FGPFCCR & (1U << 5) )
  {
    // andbar01: the 8bpp grayscale bitmap will not display otherwise
  }
}

#if (GUI_USE_ARGB == 0)
/*********************************************************************
*
*       _InvertAlpha_SwapRB_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Result is located in destination buffer.
*
* Performance:
*   10.8 MPixel/s
*/
static void _InvertAlpha_SwapRB_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 3) = Color;

    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  };
  while (NumItems--) {
    Color = *pColorSrc++;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColorDst++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Conversion is done in given buffer.
*
* Performance:
*   10.9 MPixel/s
*/
static void _InvertAlpha_SwapRB_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColor++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*/
static void _InvertAlpha_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 3) = Color;

    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  }

  while (NumItems--) {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha
  };
}

/*********************************************************************
*
*       _InvertAlpha_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the given buffer.
*
* Performance:
*   18.0 MPixel/s
*/
static void _InvertAlpha_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color ^= 0xFF000000; // Invert alpha
    *pColor++ = Color;
  };
}
#endif

/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
#if (GUI_USE_ARGB)
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(pColorFG, pColorBG, pColorDst, NumItems);
#else
  //
  // Invert alpha values
  //
  _InvertAlpha_CPY(pColorFG, _pBuffer_FG, NumItems);
  _InvertAlpha_CPY(pColorBG, _pBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_pBuffer_FG, _pBuffer_BG, pColorDst, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha_MOD(pColorDst, NumItems);
#endif
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

#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888, NumItems);
#else
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB_MOD(pColor, NumItems);
#endif
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

#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pColor, pIndex, LTDC_PIXEL_FORMAT_ARGB8888, PixelFormat, NumItems);
#else
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB_CPY(pColor, _pBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_pBuffer_DMA2D, pIndex, LTDC_PIXEL_FORMAT_ARGB8888, PixelFormat, NumItems);
#endif
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
#if (GUI_USE_ARGB)
  U32 y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);

  for (y = 0; y < ySize; y++) {
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(pFG, pBG, pDst, Intens, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#else
  U32 y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);

  for (y = 0U; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha_CPY(pFG, _pBuffer_FG, xSize);
    _InvertAlpha_CPY(pBG, _pBuffer_BG, xSize);
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(_pBuffer_FG, _pBuffer_BG, pDst, Intens, xSize);
    //
    // Invert alpha values
    //
    _InvertAlpha_MOD(pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#endif
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
  //
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
  if (_SupressCopyBuffer == 0) {
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  }
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
  AddrSrc = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex];
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
*       _LCD_DrawBitmapM565
*/
static void _LCD_DrawBitmapM565(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormatDst;

  PixelFormatDst = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_RGB565, PixelFormatDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmapAlpha
*/
static void _LCD_DrawBitmapAlpha(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawAlphaBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, PixelFormat);
}

/*********************************************************************
*
*       _LCD_DrawBitmap32bpp
*/
static void _LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
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
*       _LCD_DrawBitmap4bpp
*/
static int _LCD_DrawBitmap4bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  if (x < GUI_pContext->ClipRect.x0) {
    return 1;
  }
  if ((x + xSize - 1) > GUI_pContext->ClipRect.x1) {
    return 1;
  }
  if (y < GUI_pContext->ClipRect.y0) {
    return 1;
  }
  if ((y + ySize - 1) > GUI_pContext->ClipRect.y1) {
    return 1;
  }
  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Check if destination has direct color mode
  //
  if (PixelFormat > LTDC_PIXEL_FORMAT_ARGB4444) {
    return 1;
  }
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine * 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;

  return _DMA_DrawBitmapA4((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);;
}

/*********************************************************************
*
*       _LCD_DrawMemdevAlpha
*/
static void _LCD_DrawMemdevAlpha(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 4) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawAlphaBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_ARGB8888);
}

/*********************************************************************
*
*       _LCD_DrawMemdevM565
*
* Purpose:
*   Copy data with conversion
*/
static void _LCD_DrawMemdevM565(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_RGB565, LTDC_PIXEL_FORMAT_ARGB8888);
}

#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW
/*********************************************************************
*
*       _LCD_DrawMemdev16bpp
*/
static void _LCD_DrawMemdev16bpp(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 2) - xSize;
  _DMA_CopyRGB565(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}
#endif

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
#if (GUI_USE_ARGB)
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
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT((U32 *)pLogPal->pPalEntries, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return (LCD_PIXELINDEX *)pLogPal->pPalEntries;//_pBuffer_DMA2D;/**/
#else
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
  _InvertAlpha_SwapRB_CPY((U32 *)pLogPal->pPalEntries, _pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _pBuffer_DMA2D;
#endif
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
      // Calculate address of buffer to be used as visible frame buffer
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

  if (LayerIndex < NUM_LAYERS) {
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the display controller and put it into operation.
    //
    _LCD_InitController((int)LayerIndex);
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
#if (NUM_BUFFERS == 2)
      _SwitchBuffersOnVSYNC(p->Index, LayerIndex);
#else
    _aPendingBuffer[LayerIndex] = p->Index;
#endif
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
  int i, NoRotationUsed;
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
      LCD_SetSizeEx (1, YSIZE_1, XSIZE_1);
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
  NoRotationUsed = 1;
  for (i = 0; i < NUM_LAYERS; i++) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);               // Set custom function for copying complete buffers (used by multiple buffering) using DMA2D
    if (_aOrientation[i] == ROTATION_0) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);                   // Set custom function for copy recxtangle areas (used by GUI_CopyRect()) using DMA2D
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
      //
      // Set up drawing routine for 32bpp bitmap using DMA2D
      //
      if (PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))_LCD_DrawBitmap32bpp);     // Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888
      }
    } else {
      NoRotationUsed = 0;
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
  // Set up custom function for mixing up single colors using DMA2D
  //
  GUI_SetFuncMixColors(_DMA_MixColors);
  //
  // Set up custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
  //
  // ChromeART drawing only possible with no rotation
  //
  if (NoRotationUsed) {
    //
    // Set up custom function for drawing AA4 characters
    //
    GUI_AA_SetpfDrawCharAA4(_LCD_DrawBitmap4bpp);
#if GUI_SUPPORT_MEMDEV
    //
    // Set up custom function for drawing 16bpp memory devices
    //
#if GUI_MEMDEV_SUPPORT_CUSTOMDRAW
    GUI_MEMDEV_SetDrawMemdev16bppFunc(_LCD_DrawMemdev16bpp);
#endif
    //
    // Set up custom functions for drawing 32bpp bitmaps and 32bpp memory devices
    //
    GUI_SetFuncDrawAlpha(_LCD_DrawMemdevAlpha, _LCD_DrawBitmapAlpha);
    //
    // Set up custom function for drawing M565 bitmaps
    //
    GUI_SetFuncDrawM565(_LCD_DrawMemdevM565, _LCD_DrawBitmapM565);
#endif
  }

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
