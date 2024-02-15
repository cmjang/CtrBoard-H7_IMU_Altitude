/*------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2019 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS:Keil RTX5
#include "Board_LED.h"                  // Board Support:LED
#include "Board_Buttons.h"              // Board Support:Buttons

static osThreadId_t tid_thrLED;         // Thread id of thread: LED
static osThreadId_t tid_thrBUT;         // Thread id of thread: BUT

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

/*------------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
__NO_RETURN void thrLED (void *argument) {
  uint32_t active_flag = 1U;

  (void)argument;

  for (;;) {
    if (osThreadFlagsWait (1U, osFlagsWaitAny, 0U) == 1U) {
      active_flag ^=1U; 
    }

    if (active_flag == 1U){
      LED_On (0U);                                // Switch LED on
      osDelay (500U);                             // Delay 500 ms
      LED_Off (0U);                               // Switch off
      osDelay (500U);                             // Delay 500 ms
    }
    else {
      osDelay (500U);                             // Delay 500 ms
    }
  }
}

/*------------------------------------------------------------------------------
  thrBUT: check button state
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrBUT(void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    state = (Buttons_GetState () & 1U);           // Get pressed button state
    if (state != last){
      if (state != 0){
        osThreadFlagsSet (tid_thrLED, 1U);        // Set flag to thrLED
      }
    }
    last = state;
    osDelay (100U);
  }
}

/*------------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
void app_main (void *argument) {

  (void)argument;

  LED_Initialize ();                    // Initialize LEDs
  Buttons_Initialize ();                // Initialize Buttons
  
  tid_thrLED = osThreadNew (thrLED, NULL, NULL);  // Create LED thread
  if (tid_thrLED == NULL) { /* add error handling */ }

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);  // Create BUT thread
  if (tid_thrBUT == NULL) { /* add error handling */ }

  osThreadExit();
}
