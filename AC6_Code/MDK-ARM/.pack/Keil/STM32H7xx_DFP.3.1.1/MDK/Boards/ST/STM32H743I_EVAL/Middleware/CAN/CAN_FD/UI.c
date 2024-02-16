/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::CAN
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    UI.c
 * Purpose: CAN FD Example - User Interface module
 *----------------------------------------------------------------------------*/

#include "UI.h"

#include <stdio.h>

#include "main.h"
#include "CAN_FD.h"


// COMPILE-TIME USER INTERFACE CONFIGURATION -----------------------------------

#define UI_USE_GLCD            (0)      // 0 = no graphic LCD, 1 = use graphic LCD

//------------------------------------------------------------------------------

#if    (UI_USE_GLCD == 1)
#include "Board_GLCD.h"
#include "GLCD_Config.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
#endif

// Global functions
bool UI_Initialize (void);
void UI_Thread     (void *arg);


//------------------------------------------------------------------------------
//  User Interface Initialization
//------------------------------------------------------------------------------
bool UI_Initialize (void) {

#if (UI_USE_GLCD == 1)
  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (0U, 0U * 24U, "   CAN_FD Example   ");
  GLCD_DrawString         (0U, 2U * 24U, "Tx data:            ");
  GLCD_DrawString         (0U, 3U * 24U, "                    ");
  GLCD_DrawString         (0U, 5U * 24U, "Rx data:            ");
  GLCD_DrawString         (0U, 6U * 24U, "                    ");
  GLCD_DrawString         (0U, 8U * 24U, "  Keil Tools by ARM ");
  GLCD_DrawString         (0U, 9U * 24U, "    www.keil.com    ");
#endif
  printf ("CAN_FD Example\n");

  return true;
}

//------------------------------------------------------------------------------
//  User Interface Thread Function
//------------------------------------------------------------------------------
__NO_RETURN void UI_Thread (void *arg) {
  uint32_t flags;
#if (UI_USE_GLCD == 1)
  char     disp_str[16];
#endif

  (void)arg;

  for (;;) {
    flags = osThreadFlagsWait(0x00000003U, osFlagsWaitAny, osWaitForever);
    if ((flags & 0x80000000U) == 0U) {          // If no error
      if ((flags & 1U) != 0U) {                 // If receive event happened
        printf ("Rx data = %02X,%02X,%02X,%02X,%02X,%02X, .. %02X\n", 
                rx_data[0], rx_data[1], rx_data[2], rx_data[3], 
                rx_data[4], rx_data[5],             rx_data[63]);
#if (UI_USE_GLCD == 1)
        sprintf(disp_str, "%02X,%02X,%02X,%02X", rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
        GLCD_DrawString(8*16, 5*24, disp_str);
        sprintf(disp_str, "%02X,%02X, ..  %02X", rx_data[4], rx_data[5],             rx_data[63]);
        GLCD_DrawString(8*16, 6*24, disp_str);
#endif
      }
      if ((flags & 2U) != 0U) {                 // If transmit event happened
        printf ("Tx data = %02X,%02X,%02X,%02X,%02X,%02X, .. %02X\n", 
                tx_data[0], tx_data[1], tx_data[2], tx_data[3], 
                tx_data[4], tx_data[5],             tx_data[63]);
#if (UI_USE_GLCD == 1)
        sprintf(disp_str, "%02X,%02X,%02X,%02X", tx_data[0], tx_data[1], tx_data[2], tx_data[3]);
        GLCD_DrawString(8*16, 2*24, disp_str);
        sprintf(disp_str, "%02X,%02X, ..  %02X", tx_data[4], tx_data[5],             tx_data[63]);
        GLCD_DrawString(8*16, 3*24, disp_str);
#endif
      }
    }
  }
}
