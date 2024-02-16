/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2022 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        24. June 2022
 * $Revision:    V1.1
 *
 * Project:      OTG Common Driver for ST STM32H7xx
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.1
 *    Removed include of stm32h7xx_hal_pcd.h header
 *  Version 1.0
 *    Initial release
 */

#include "OTG_STM32H7xx.h"

#include "Driver_USBH.h"
#include "Driver_USBD.h"

#include "stm32h7xx_hal.h"

#include "USBH_HS1_STM32H7xx.h"
#include "USBH_HS2_STM32H7xx.h"


#if   (OTG_FS_DEVICE_USED == 1) || (OTG_HS_DEVICE_USED == 1)
extern void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);
#endif

#if   (OTG_FS_DEVICE_USED == 1)
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
#endif
#if   (OTG_HS_DEVICE_USED == 1)
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
#endif

#if   (OTG_FS_USED == 1)
extern void OTG_FS_IRQHandler (void);
#endif

#if   (OTG_HS_USED == 1)
extern void OTG_HS_IRQHandler (void);
#endif


// Global variables
uint8_t otg_fs_role = ARM_USB_ROLE_NONE;
uint8_t otg_hs_role = ARM_USB_ROLE_NONE;

// OTG IRQ routine (common for Device and Host) ********************************

#if (OTG_FS_USED == 1)
/**
  \fn          void OTG_FS_IRQHandler (void)
  \brief       USB Interrupt Routine (IRQ).
*/
void OTG_FS_IRQHandler (void) {
#if (OTG_FS_DEVICE_USED == 1) && (OTG_FS_HOST_USED == 1)
  switch (otg_fs_role) {
    case ARM_USB_ROLE_HOST:
      USBH_HS2_IRQ (USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK);
      break;
    case ARM_USB_ROLE_DEVICE:
      HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
      break;
    default:
      break;
  }
#elif (OTG_FS_HOST_USED == 1)
  USBH_HS2_IRQ (USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK);
#else
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
#endif
}
#endif

#if (OTG_HS_USED == 1)
/**
  \fn          void OTG_HS_IRQHandler (void)
  \brief       USB Interrupt Routine (IRQ).
*/
void OTG_HS_IRQHandler (void) {
#if (OTG_HS_DEVICE_USED == 1) && (OTG_HS_HOST_USED == 1)
  switch (otg_hs_role) {
    case ARM_USB_ROLE_HOST:
      USBH_HS1_IRQ (USB_OTG_HS->GINTSTS & USB_OTG_HS->GINTMSK);
      break;
    case ARM_USB_ROLE_DEVICE:
      HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
      break;
    default:
      break;
  }
#elif (OTG_HS_HOST_USED == 1)
  USBH_HS1_IRQ (USB_OTG_HS->GINTSTS & USB_OTG_HS->GINTMSK);
#else
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
#endif
}
#endif
