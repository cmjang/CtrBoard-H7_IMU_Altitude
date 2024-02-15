/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device - Human Interface Device example
 *----------------------------------------------------------------------------*/

#include "main.h"
#include "rl_usb.h"                     // Keil.MDK-Pro::USB:CORE

#include "Board_LED.h"
#include "Board_Joystick.h"

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

__NO_RETURN void app_main (void *arg);

/*------------------------------------------------------------------------------
 *        Application
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  uint8_t joy, joy_last;

  (void)arg;

  LED_Initialize();                     /* LED Initialization                 */
  Joystick_Initialize();                /* Joystick Initialization            */

  USBD_Initialize(0U);                  /* USB Device 0 Initialization        */
  USBD_Connect   (0U);                  /* USB Device 0 Connect               */

  USBD_Initialize(1U);                  /* USB Device 1 Initialization        */
  USBD_Connect   (1U);                  /* USB Device 1 Connect               */

  joy = joy_last = 0U;
  while (1) {                           /* Loop forever                       */
    joy = (uint8_t)(Joystick_GetState ());
    if ((joy ^ joy_last) & 0x1FU) {
      joy_last = joy & 0x1FU;
      if (USBD_Configured (0)) { USBD_HID_GetReportTrigger(0U, 0U, &joy, 1U); }
      if (USBD_Configured (1)) { USBD_HID_GetReportTrigger(1U, 0U, &joy, 1U); }
    }
    osDelay(100U);                      /* 100 ms delay for sampling buttons  */
  }
}
