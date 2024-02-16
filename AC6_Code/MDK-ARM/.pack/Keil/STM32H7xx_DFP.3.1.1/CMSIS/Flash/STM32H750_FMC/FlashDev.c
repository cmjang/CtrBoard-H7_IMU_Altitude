/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.c:  Device Description for ST STM32F4xx Flash             */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "M29W128GL_STM32H743I-EVAL",          // Device Name 
   EXT16BIT,                     // Device Type
   0x60000000,                 // Device Start Address
   0x01000000,                 // Device Size in Bytes (2048kB)
   0x00001000,                       // Programming Page Size
   0x00,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   10000,                        // Program Page Timeout 100 mSec
   6000,                       // Erase Sector Timeout 6000 mSec

// Specify Size and Address of Sectors
   0x20000, 0x000000,          // Sector Size  128kB (16 Sectors)
   SECTOR_END
};
#endif // FLASH_MEM
