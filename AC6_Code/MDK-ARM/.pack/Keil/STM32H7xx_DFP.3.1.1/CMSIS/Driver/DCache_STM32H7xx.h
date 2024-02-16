/* -----------------------------------------------------------------------------
 * Copyright (c) 2023 Arm Limited (or its affiliates).
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
 * $Date:        10. February 2023
 * $Revision:    V1.0
 *
 * Project:      Data Cache header for CMSIS Drivers for ST STM32H7xx
 * -------------------------------------------------------------------------- */

#ifndef __DCACHE_STM32H7XX_H
#define __DCACHE_STM32H7XX_H

#include <stdint.h>

#include "stm32h7xx_hal.h"

/**
  Check if Data Cache is enabled in the MCU.
  \return       Data Cache enabled status (1 = is enabled, 0 = is not enabled))
*/
__STATIC_INLINE uint32_t DCache_IsEnabled (void) {
  uint32_t result = 0U;

  if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
    result = 1U;
  }

  return result;
}

/**
  Check if Data Cache operation can be used on the specified memory area.
  If memory is properly aligned and size is multiple of cache line size.
  \param[in]    addr   memory area start address
  \param[in]    size   memory area size
  \return       usage status (1 = can be used, 0 = cannot be used)
*/
__STATIC_INLINE uint32_t DCache_IsAligned (const void *addr, uint32_t size) {
  uint32_t result = 0U;

  if (((((uint32_t)addr) & 0x1FU) == 0U) && ((size & 0x1FU) == 0U)) {
    // If address is 32-byte aligned and if size is multiple of 32 bytes
    result = 1U;
  }

  return result;
}

/**
  Flush the content of the Data Cache to Memory.
  \param[in]    addr   memory area start address
  \param[in]    size   memory area size
*/
__STATIC_INLINE void DCache_Flush (const void *addr, uint32_t size) {
  uint32_t cache_addr;
  uint32_t cache_size;
  
  cache_addr =    (uint32_t)addr                              & (~0x1FU);
  cache_size = ((((uint32_t)addr + size) - cache_addr) + 31U) & (~0x1FU);

  SCB_CleanDCache_by_Addr((volatile void *)cache_addr, (int32_t)cache_size);
}

/**
  Refresh the content of the Data Cache from Memory.
  \param[in]    addr   memory area start address
  \param[in]    size   memory area size
*/
__STATIC_INLINE void DCache_Refresh (volatile void *addr, uint32_t size) {
  uint32_t cache_addr;
  uint32_t cache_size;
  
  cache_addr =    (uint32_t)addr                              & (~0x1FU);
  cache_size = ((((uint32_t)addr + size) - cache_addr) + 31U) & (~0x1FU);

  SCB_InvalidateDCache_by_Addr((volatile void *)cache_addr, (int32_t)cache_size);
}

#endif /* __DCACHE_STM32H7XX_H */
