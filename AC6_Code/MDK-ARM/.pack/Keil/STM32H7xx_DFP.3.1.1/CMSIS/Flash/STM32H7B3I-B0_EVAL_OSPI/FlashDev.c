/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2020 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.c:  Device Description for ST STM32H7B3I-B0-EVAL Flash    */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEM
#if defined STM32H7B3
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MX25LM51245G_STM32H7B3I-EVAL",          // Device Name 
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
#endif
#if defined STM32H7B0
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "MX25LM51245G_STM32H7B0-EVAL",          // Device Name 
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
#endif

#endif // FLASH_MEM
