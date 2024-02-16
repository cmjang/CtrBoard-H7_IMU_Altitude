/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32F4xx Flash                   */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.h"        // FlashOS Structures
#include "STM32H7xx.h" 

#define FLASH_KEY1                         ((uint32_t)0x45670123) /*!< Flash key1 */
#define FLASH_KEY2                         ((uint32_t)0xCDEF89AB) /*!< Flash key2: used with FLASH_KEY1 
                                                                       to unlock the FLASH registers access */
#define FLASH_PDKEY1                       ((uint32_t)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2                       ((uint32_t)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                                       to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_OPTKEY1                      ((uint32_t)0x08192A3B) /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                      ((uint32_t)0x4C5D6E7F) /*!< Flash option byte key2: used with FLASH_OPTKEY1*/ 

#define FLASH_FLAG_BSY										 ((uint32_t)0x00000001) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOP										 ((uint32_t)0x00010000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOPERR									 ((uint32_t)0x00400000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_QW										   ((uint32_t)0x00000004) /*!< Flash BUSY FLAG*/


#define VOLTAGE_RANGE											 0//FLASH_FCR_A_PSIZE_A

#define FLASH_BANK1_BASE  FLASH_BASE
#define FLASH_BANK2_BASE  (FLASH_BASE + 0x00100000)
#define IS_FLASH_BANK1_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) <= (FLASH_BANK1_BASE + 0x000FFFFF) ))

void DSB(void) {
    __asm("DSB");
}


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

		//RCC->D1AHB1ENR = 0x100;
		//FLASH_DBG = (FLASH_TypeDef *)  FLASH_R_BASE;
	
//	SCB_DisableDCache();
//	SCB_DisableICache();
//	SCB_EnableDCache();
//	SCB_EnableICache();

	
	//PWR->SCR = 3<<14;
	//while (!(PWR->SCR & 1<<13));
	((int *)RCC)[(0x194/4)] = 1<<8;
	FLASH->FACR |= 7;
	
	FLASH->FCCR_A = 0x0FEF0000;	
	while (FLASH->FSR_A & FLASH_FLAG_QW);									// Reset Error Flags of Bank A
	
	/* Unlcock FLASH A Registers access */
	FLASH->FKEYR_A = FLASH_KEY1;
	FLASH->FKEYR_A = FLASH_KEY2;
	
	FLASH->FCCR_B = 0x0FEF0000;															// Reset Error Flags Bank B
	while (FLASH->FSR_B & FLASH_FLAG_QW);
	
	/* Unlcock FLASH B Registers access */
	FLASH->FKEYR_B = FLASH_KEY1;
	FLASH->FKEYR_B = FLASH_KEY2;
	

	/*********/
	/* Tests */
	/*********/
#if 0	
	EraseSector(0x08020000);
	EraseSector(0x08040000);
	//EraseSector(0x081C27A0);
	ProgramPage(0x08020000,128,(char*)0x20000000);
	ProgramPage(0x08040000,128,(char*)0x20000000);
	EraseSector(0x08040000);
	
	SCB_DisableDCache();
  SCB_DisableICache();
	EraseSector(0x08000000);
	EraseChip();
	EraseSector(0x08100000);
	ProgramPage(0x08100000,128,(char*)0x20000000);
	//EraseSector(0x08100000);
#endif
  return (0);
}
#endif


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int UnInit (unsigned long fnc) {

  FLASH->FCR_A |=  1;                             // Lock Flash
	FLASH->FCR_B |=  1;

  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {
	
	FLASH->FCCR_A = 0x0FEF0000;	
	while (FLASH->FSR_A & FLASH_FLAG_QW);									// Reset Error Flags of Bank A
	
	FLASH->FCCR_B = 0x0FEF0000;															// Reset Error Flags Bank B
	while (FLASH->FSR_B & FLASH_FLAG_QW);
	
	while (FLASH->FSR_A & FLASH_FLAG_QW) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
	/* Bank1 erase */
	FLASH->FCR_A &= (~FLASH_FCR_A_PSIZE_A);
	FLASH->FCR_A |= FLASH_FCR_A_BER_A | VOLTAGE_RANGE; // Paralellism = 64bit
	FLASH->FCR_A |= FLASH_FCR_A_STRT_A;
	
  while (FLASH->FSR_A & FLASH_FLAG_QW) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
	
	FLASH->FCR_A &= ~FLASH_FCR_A_BER_A;

	while (FLASH->FSR_B & FLASH_FLAG_QW) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
		
	/* Bank2 erase */
	FLASH->FCR_B &= (~FLASH_FCR_B_PSIZE_B);
	FLASH->FCR_B |= FLASH_FCR_B_BER_B | VOLTAGE_RANGE;
	FLASH->FCR_B |= FLASH_FCR_B_STRT_B;

  while (FLASH->FSR_B & FLASH_FLAG_QW) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
	
	FLASH->FCR_B &= ~FLASH_FCR_B_BER_B;

  return (0);                                           // Done
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
	
	uint8_t Sector = ((adr & 0x00FFFFFF) >> 17)& 0xF;
	
	if (IS_FLASH_BANK1_ADDRESS(adr))
  {
		FLASH->FCCR_A |= 0x0FEF0000;	
		while (FLASH->FSR_A & FLASH_FLAG_QW);
		
    FLASH->FCR_A &= ~(FLASH_FCR_A_PSIZE_A | 0x07F00);
    FLASH->FCR_A |= (FLASH_FCR_A_SER_A | VOLTAGE_RANGE | (Sector << 8));	
    FLASH->FCR_A |= FLASH_FCR_A_STRT_A;	
		
		while (FLASH->FSR_A & FLASH_FLAG_QW);
		
		FLASH->FCR_A &= ~FLASH_FCR_A_SER_A;
		
		if (FLASH->FSR_A & FLASH_FLAG_EOPERR != 0)							// Check for Error
			return 1;
  }
  else
  {
		FLASH->FCCR_B |= 0x0FEF0000;
		while (FLASH->FSR_B & FLASH_FLAG_QW);
		
    FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
    FLASH->FCR_B |= (FLASH_FCR_B_SER_B | VOLTAGE_RANGE  | ((Sector - 8) << 8));
    FLASH->FCR_B |= FLASH_FCR_B_STRT_B;	
		
		while (FLASH->FSR_B & FLASH_FLAG_QW);
		
		FLASH->FCR_B &= ~FLASH_FCR_B_SER_B;
		
		if (FLASH->FSR_B & FLASH_FLAG_EOPERR != 0)							// Check for Error
			return 1;
  }

  return (0);                                           // Done
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
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

	__IO uint64_t *dest = (__IO uint64_t *)adr;
  __IO uint64_t *src = (__IO uint64_t*)((uint32_t)buf);
	int i = 0;
	
  //sz = (sz + 3) & ~3;                                   	// Adjust size for Words
  
	if (IS_FLASH_BANK1_ADDRESS(adr))
	{
		while (FLASH->FSR_A & FLASH_FLAG_QW);
		FLASH->FCCR_A = 0x0FEF0000;															// Reset Error Flags
	}
	else
	{
		while (FLASH->FSR_B & FLASH_FLAG_QW);
		FLASH->FCCR_B = 0x0FEF0000;															// Reset Error Flags
	}
	
  while (sz) {
		
		if (IS_FLASH_BANK1_ADDRESS(adr))
		{
			FLASH->FCR_A &= ~(FLASH_FCR_A_PSIZE_A | 0x07F00);
			FLASH->FCR_A = FLASH_FCR_A_PG_A | VOLTAGE_RANGE;			 // Programming Enabled 
		}
		else
		{
			FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
			FLASH->FCR_B = FLASH_FCR_B_PG_B | VOLTAGE_RANGE;
		}
	
		if (sz>=32)
		{
			/* Program the 256 bits flash */
			for (i=0;i<4;i++)
			{
				*dest++ = *src++;

			}
			
			sz  -= 32;
		}
		else
		{
				for (i=0;i<(sz);i++)
				{
					*((uint8_t*)dest)++ = *((uint8_t*)src)++;
				}
				for (i=0;i<((32 - sz));i++)
				{
					*((uint8_t*)dest)++ = 0xFF;
				}
							
			sz  = 0;
		}
		
	      DSB();
	
	if (IS_FLASH_BANK1_ADDRESS(adr))
		while (FLASH->FSR_A & FLASH_FLAG_QW);
	else
		while (FLASH->FSR_B & FLASH_FLAG_QW);

	
		if (FLASH->FSR_A & FLASH_FLAG_EOP == 0)							// Check for Error
		{
			if (IS_FLASH_BANK1_ADDRESS(adr))
				FLASH->FCR_A &= ~FLASH_FCR_A_PG_A;			 				 	// Programming disabled 
			else
				FLASH->FCR_B &= ~FLASH_FCR_B_PG_B;
			
			return (1);
		}
	
    if (IS_FLASH_BANK1_ADDRESS(adr))
			FLASH->FCR_A &= ~FLASH_FCR_A_PG_A;			 				 	// Programming disabled 
		else
			FLASH->FCR_B &= ~FLASH_FCR_B_PG_B;
		
  }
  return (0);                                           // Done
}
#endif
