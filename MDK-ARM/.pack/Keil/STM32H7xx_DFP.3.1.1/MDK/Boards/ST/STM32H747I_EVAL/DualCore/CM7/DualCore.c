/*----------------------------------------------------------------------------
 * Name:    DualCore.c
 * Purpose: DualCore LED Flasher (CM7 part)
 *----------------------------------------------------------------------------*/

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval.h"
#include "common.h"


/*----------------------------------------------------------------------------
  main loop
 *----------------------------------------------------------------------------*/
int app_main (void)
{
  uint32_t hsemId = LED2;                                  /* HW semaphore to use */

  BSP_LED_Init(LED1);
  BSP_LED_On(LED1);
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  while (1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);

    if (BSP_PB_GetState(BUTTON_TAMPER) == RESET)
    {
      HAL_Delay(10);
      while (BSP_PB_GetState(BUTTON_TAMPER) == RESET);

      if (hsemId > LED4)
      {
        hsemId = LED2;
      }

      HAL_HSEM_FastTake(hsemId);                          /* Take HSEM */
      HAL_HSEM_Release(hsemId, 0);                        /* Release HSEM in order to notify the CPU2 (CM4)*/
      hsemId += 1;
    }
  }
}
