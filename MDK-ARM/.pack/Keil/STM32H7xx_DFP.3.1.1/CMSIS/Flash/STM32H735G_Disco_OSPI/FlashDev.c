/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2020 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.c:  Device Description for ST STM32H735G-DISCO Flash      */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MX25LM51245G_STM32H735-Disco",          // Device Name 
   EXTSPI,                     // Device Type
   0x90000000,                   // Device Start Address
   0x04000000,                   // Device Size in Bytes (64MB)
   0x00001000,                   // Programming Page Size 4096 Bytes
   0x00,                         // Reserved, must be 0
   0xFF,                         // Initial Content of Erased Memory
   10000,                        // Program Page Timeout 100 mSec
   6000,                         // Erase Sector Timeout 6000 mSec

// Specify Size and Address of Sectors
   0x10000, 0x000000,            // Sector Size  64kB, Sector Num : 1024
   SECTOR_END
};
#endif // FLASH_MEM
