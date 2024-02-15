/* -----------------------------------------------------------------------------
 * Copyright (c) 2019 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        20. May 2019
 * $Revision:    V1.0.0
 *
 * Project:      Flash Device Description for ST STM32H7xx
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */

#include "..\FlashOS.h"        /* FlashOS Structures */

#ifdef FLASH_MEM

#ifdef STM32H7x_2048
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,             /* Driver Version, do not modify! */
    "STM32H7x_2048",            /* Device Name */
    ONCHIP,                     /* Device Type */
    0x08000000,                 /* Device Start Address */
    0x00200000,                 /* Device Size in Bytes (2048kB) */
    1024,                       /* Programming Page Size */
    0,                          /* Reserved, must be 0 */
    0xFF,                       /* Initial Content of Erased Memory */
    100,                        /* Program Page Timeout 1000 mSec */
    6000,                       /* Erase Sector Timeout 1000 mSec */
    /* Specify Size and Address of Sectors */
    0x20000, 0x000000,          /* Sector Size  128kB (16 Sectors) */
    SECTOR_END
  };
#endif /* STM32H7x_2048 */

#endif /* FLASH_MEM */
