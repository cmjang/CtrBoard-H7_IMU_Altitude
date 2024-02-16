/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32F4xx Flash                   */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures
#include "STM32H70x.h" 

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

#define FLASH_FCR_A_STRT1                   ((uint32_t)0x00000020)
#define FLASH_FCR_B_STRT1                   ((uint32_t)0x00000020)

#define FLASH_FOPTCR_MER                   ((uint32_t)0x00000010)

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
	//((int *)RCC)[(0x194/4)] = 1<<8;
	//FLASH->FACR |= 7;
	
	FLASH->FCCR_A = 0x0FAF0000;	
	while (FLASH->FSR_A & FLASH_FLAG_BSY);									// Reset Error Flags of Bank A
	
	/* Unlcock FLASH A Registers access */
	FLASH->FKEYR_A = FLASH_KEY1;
	FLASH->FKEYR_A = FLASH_KEY2;
	
	FLASH->FCCR_B = 0x0FAF0000;															// Reset Error Flags Bank B
	while (FLASH->FSR_B & FLASH_FLAG_BSY);
	
	/* Unlcock FLASH B Registers access */
	FLASH->FKEYR_B = FLASH_KEY1;
	FLASH->FKEYR_B = FLASH_KEY2;
	
  if ((FLASH->FOPTSR_CUR & 0x30) == 0x00) {                  // Test if IWDG/WWDG is running (IWDG in HW mode)
    // Set IWDG time out to ~32.768 second
//    IWDG1->KR  = 0x5555;                                 // Enable write access to IWDG_PR and IWDG_RLR     
//    IWDG1->PR  = 0x06;                                   // Set prescaler to 256  
//    IWDG1->RLR = 4095;                                   // Set reload value to 4095
  }

	/*********/
	/* Tests */
	/*********/
	

//	  EraseSector(0x08100000);
//	EraseSector(0x081C27A0);
//	  ProgramPage(0x8102000,128,(char*)0x20000000);
//	EraseSector(0x08000000);
	
//	SCB_DisableDCache();
//  SCB_DisableICache();
//	EraseSector(0x08000000);
//	EraseChip();
//	EraseSector(0x08100000);
//	ProgramPage(0x08100000,128,(char*)0x20000000);
//	//EraseSector(0x08100000);
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
	
	
	while (FLASH->FSR_A & FLASH_FLAG_BSY) {
  }


		/* Mass erase */
//		FLASH->FCCR_A = 0x0FAF0000;	
//		FLASH->FCCR_B = 0x0FAF0000;	
//	  FLASH->FCR_A |= FLASH_FCR_A_LOCK_A;
//	  FLASH->FCR_B |= FLASH_FCR_B_LOCK_B;	
//	  FLASH->FOPTCR |= FLASH_FCR_A_LOCK_A;
//	  FLASH->FOPTCR |= FLASH_FOPTCR_MER; 
	
	
	
	/* Bank1 erase */
	//FLASH->FCR_A &= (~FLASH_FCR_A_PSIZE_A);
	
	
	FLASH->FCCR_A = 0x0FAF0000;	
	FLASH->FCR_A &= ~FLASH_FCR_A_LOCK_A;
	
	FLASH->FCR_A |= FLASH_FCR_A_BER_A; // Paralellism = 64bit
	FLASH->FCR_A |= FLASH_FCR_A_STRT_A;
	
  while (FLASH->FSR_A & FLASH_FLAG_BSY) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
	
	FLASH->FCR_A &= ~FLASH_FCR_A_BER_A;

	
	while (FLASH->FSR_B & FLASH_FLAG_BSY) {
    //IWDG1->KR = 0xAAAA;                                  // Reload IWDG
  }
		
	/* Bank2 erase */
	 // FLASH->FCR_B &= (~FLASH_FCR_B_PSIZE_B);
		FLASH->FCCR_B = 0x0FAF0000;	
		FLASH->FCR_B &= ~FLASH_FCR_B_LOCK_B;
	  FLASH->FCR_B |= FLASH_FCR_B_BER_B;
	  FLASH->FCR_B |= FLASH_FCR_B_STRT_B;

  while (FLASH->FSR_B & FLASH_FLAG_BSY) {
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
	uint8_t Sector = (adr & 0x00FFF000)/0x2000;
	
	if (IS_FLASH_BANK1_ADDRESS(adr))
  {
		FLASH->FCCR_A |= 0x0FEF0000;	
		while (FLASH->FSR_A & FLASH_FLAG_QW);
		 FLASH->FCR_A &= ~(0x7F<<6);
    FLASH->FCR_A |= (FLASH_FCR_A_SER_A | VOLTAGE_RANGE | (Sector << 6));	
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
				 FLASH->FCR_B &= ~(0x7F<<6);
    //FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
    FLASH->FCR_B |= (FLASH_FCR_B_SER_B | VOLTAGE_RANGE  | ((Sector - 128) << 6));
    FLASH->FCR_B |= FLASH_FCR_B_STRT_B;	
		
		while (FLASH->FSR_B & FLASH_FLAG_QW);
		
		FLASH->FCR_B &= ~FLASH_FCR_B_SER_B;
		
		if (FLASH->FSR_B & FLASH_FLAG_EOPERR != 0)							// Check for Error
			return 1;
  }
	    DSB();

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
  
//	if (IS_FLASH_BANK1_ADDRESS(adr))
//	{
		while (FLASH->FSR_A & FLASH_FLAG_BSY);
		FLASH->FCCR_A = 0x0FAF0000;															// Reset Error Flags

//	}
//	else
//	{
		while (FLASH->FSR_B & FLASH_FLAG_BSY);
		FLASH->FCCR_B = 0x0FAF0000;															// Reset Error Flags

//	}
	
  while (sz) {
		
////		if (IS_FLASH_BANK1_ADDRESS(adr))
////		{
	//		FLASH->FCR_A &= ~(FLASH_FCR_A_PSIZE_A | 0x07F00);
		  FLASH->FCR_A &= ~FLASH_FCR_A_LOCK_A;
			FLASH->FCR_A |= FLASH_FCR_A_PG_A | VOLTAGE_RANGE;			 // Programming Enabled 
/////		}
/////		else
/////		{	
	//	  FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
		  FLASH->FCR_B &= ~FLASH_FCR_B_LOCK_B;
			FLASH->FCR_B |= FLASH_FCR_B_PG_B | VOLTAGE_RANGE;
////		}
	
		if (sz>=16)
		{
			/* Program the 128 bits flash */
			for (i=0;i<2;i++)
			{
				*dest++ = *src++;
			//	dest+=8;
			//	src+=8;
			}
			
			sz  -= 16;
		}
		else
		{
			/* Program the 128 bits flash */
			//if ((sz%8) == 0) //Write in 64bit
			//{
				for (i=0;i<(sz);i++)
				{
					*((uint8_t*)dest)++ = *((uint8_t*)src)++;
				}
				for (i=0;i<((16 - sz));i++)
				{
					*((uint8_t*)dest)++ = 0xFF;
				}
				
//			}
//			else // Write in 32bit
//			{
//				for (i=0;i<(sz/4);i++)
//				{
//					*((uint32_t*)dest)++ = *((uint32_t*)src)++;
//				}
//			}
			
			if (IS_FLASH_BANK1_ADDRESS(adr))
				FLASH->FCR_A |= FLASH_FCR_A_FW_A;		
			else
				FLASH->FCR_B |= FLASH_FCR_B_FW_B;	
			
			sz  = 0;
		}
		
		DSB();
	
	if (IS_FLASH_BANK1_ADDRESS(adr))
		while (FLASH->FSR_A & FLASH_FLAG_BSY);
	else
		while (FLASH->FSR_B & FLASH_FLAG_BSY);

	
		if ((FLASH->FSR_A & FLASH_FLAG_EOP == 0) | (FLASH->FSR_B & FLASH_FLAG_EOP == 0))							// Check for Error
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
