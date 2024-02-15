/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2020 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32h7B3I/B0-EVAL Flash          */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        
#include "STM32H7OSPI.h"

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

 
	if(Init_OSPI ()!=0)
   return 0;
 else 
   return 1;
}
#endif


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int UnInit (unsigned long fnc) {


  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {
	

  return (MassErase());                                          
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
	
    
 int result = 0;
	uint32_t block_start=adr;
	uint32_t block_size;
	
  result = SectorErase ((uint32_t) block_start & 0xffffffff ,((uint32_t) block_start & 0xffffffff) + block_size);
  if (result == 1)   
    return 0;   
  else 
    return result;	
}
#endif


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int ProgramPage (unsigned long block_start, unsigned long size, unsigned char *buffer) {
                                       
	if(Write(block_start ,size, buffer)!=0)
  return 0;
	 else
  return 1;
}



unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf){
	
	while (sz-->0)
	{
		if ( *(char*)adr++ != *((char*)buf++))
			return (adr);
	}
		
  return adr;
}
#endif
