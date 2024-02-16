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
 * $Revision:    V1.0
 *
 * Project:      USB High/Full/Low-Speed Host 0 Driver Header File for ST
 *               STM32H7xx
 * -------------------------------------------------------------------------- */

#ifndef __USBH0_STM32H7XX_H
#define __USBH0_STM32H7XX_H

#include <stdint.h>

#include "Driver_USBH.h"

// Global functions and variables exported by driver .c module
extern void USBH_HS1_IRQ (uint32_t gintsts);
extern ARM_DRIVER_USBH Driver_USBH0;

#endif /* __USBH0_STM32H7XX_H */
