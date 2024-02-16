/* -----------------------------------------------------------------------------
 * Copyright (c) 2017-2022 Arm Limited (or its affiliates).
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
 * Project:      FDCAN 1/2 Driver Header File for ST STM32H7xx
 * --------------------------------------------------------------------------*/

#ifndef __FDCAN_STM32H7XX_H
#define __FDCAN_STM32H7XX_H

#include <stdint.h>

#include "Driver_CAN.h"

#include "MX_Device.h"

// Global functions and variables exported by driver .c module

#if (defined(MX_FDCAN1) && (MX_FDCAN1 == 1))
extern void FDCAN1_IT0_IRQHandler (void);
extern ARM_DRIVER_CAN Driver_CAN1;
#endif

#if (defined(MX_FDCAN2) && (MX_FDCAN2 == 1))
extern void FDCAN2_IT0_IRQHandler (void);
extern ARM_DRIVER_CAN Driver_CAN2;
#endif

#endif /* __FDCAN_STM32H7XX_H */
