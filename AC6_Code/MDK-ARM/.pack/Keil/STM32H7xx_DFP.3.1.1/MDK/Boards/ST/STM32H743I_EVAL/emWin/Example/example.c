/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2018  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.48 - Graphical user interface for embedded applications **
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
File        : example.c
Purpose     : Main program Template
---------------------------END-OF-HEADER------------------------------
*/

#include "main.h"
#include "GUI.h"                        // Segger.MDK-Pro::Graphics:CORE

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

__NO_RETURN void app_main (void *arg);

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *argument) {
  int32_t xPos, yPos, xSize, ySize;
  int32_t i = 0;

  (void)argument;

  GUI_Init();

  GUI_SetBkColor(GUI_RED);
  GUI_Clear();
  GUI_Delay(1000);
  GUI_SetBkColor(GUI_GREEN);
  GUI_Clear();
  GUI_Delay(1000);
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  GUI_Delay(1000);

  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();

  GUI_SetFont(&GUI_FontComic24B_1);
  GUI_SetColor(GUI_CYAN);
  GUI_DispStringHCenterAt("www.keil.com",   xSize / 2, 20);
  GUI_SetColor(GUI_DARKBLUE);
  GUI_DispStringHCenterAt("www.segger.com", xSize / 2, ySize - 40);

  xPos = xSize / 2;
  yPos = ySize / 3;
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_REV);
  GUI_SetFont(GUI_FONT_20F_ASCII);
  GUI_DispStringHCenterAt("Hello world!", xPos, yPos);
  GUI_SetFont(GUI_FONT_D24X32);
  xSize = GUI_GetStringDistX("0000");
  xPos -= xSize / 2;
  yPos += 24 + 10;
  while (1) {
    GUI_DispDecAt(i++, xPos, yPos, 4U);
    if (i > 9999) {
      i = 0;
    }
  } 
}
