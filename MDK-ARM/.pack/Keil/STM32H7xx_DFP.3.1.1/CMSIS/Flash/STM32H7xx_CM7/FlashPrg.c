/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32H7xx Flash                   */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures
#include "Loader_Src.h"

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
 __disable_irq	(	 		)	;

  ((int *)RCC)[(0x0d4/4)] = 1<<8;//((int *)RCC)[(0x194/4)] = 1<<8;
	
  *(__IO uint32_t*) 0x52002000 = (uint32_t) 7;

  FLASH->FCCR_A = 0x006F0000;	
  while (FLASH->FSR_A & FLASH_FLAG_QW);		// Reset Error Flags of Bank A

  /* Unlcock FLASH A Registers access */
  if(FLASH->FCR_A & 1 == 1) 
  {     
    FLASH->FKEYR_A = FLASH_KEY1;
    FLASH->FKEYR_A = FLASH_KEY2;
  }
  FLASH->FCCR_B = 0x006F0000;		
  while (FLASH->FSR_B & FLASH_FLAG_QW);
  if(FLASH->FCR_B & 1 == 1)
  { 
    /* Unlcock FLASH B Registers access */
    FLASH->FKEYR_B = FLASH_KEY1;
    FLASH->FKEYR_B = FLASH_KEY2;
  }  
  resetWDG();
  *(__IO uint32_t*) 0x52002014 = (uint32_t) 0x0FEE0000;
  *(__IO uint32_t*) 0x52002114 = (uint32_t) 0x0FEE0000;
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
	
	uint32_t StartAddress =  (uint32_t)adr;
  uint32_t EndAddress = (uint32_t)adr+ 0x20000 ;
uint32_t  Parallelism = 0; // Forced to 64-Bit inshark, since it's indepandent from target voltage  
  while (StartAddress <= EndAddress)
  {	
    uint8_t Sector = ((StartAddress & 0x00FFFFFF) >> 17)& 0xF;
  
    if (IS_FLASH_BANK1_ADDRESS(StartAddress))
    {
      FLASH->FCCR_A |= 0x0FEF000E;	
      while (FLASH->FSR_A & FLASH_FLAG_QW)
        resetWDG();
                  
      FLASH->FCR_A &= ~(FLASH_FCR_A_PSIZE_A | 0x07F00);
      FLASH->FCR_A |= (FLASH_FCR_A_SER_A | Parallelism | (Sector << 8) | (3<<4) );	
      FLASH->FCR_A |= FLASH_FCR_A_STRT_A;	
                  
      while (FLASH->FSR_A & FLASH_FLAG_QW)
        resetWDG();
                  
      FLASH->FCR_A &= ~FLASH_FCR_A_SER_A;
                  
      if (FLASH->FSR_A & FLASH_FLAG_EOPERR != 0)				// Check for Error
        return 1;
    }
    else
    {
      FLASH->FCCR_B |= 0x00FEF000E;	
      while (FLASH->FSR_B & FLASH_FLAG_QW)
        resetWDG();
                  
      FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
      FLASH->FCR_B |= (FLASH_FCR_B_SER_B | Parallelism  | ((Sector - 8) << 8) | (3<<4) );
      FLASH->FCR_B |= FLASH_FCR_B_STRT_B;	
                  
      while (FLASH->FSR_B & FLASH_FLAG_QW)
        resetWDG();
                  
      FLASH->FCR_B &= ~FLASH_FCR_B_SER_B;
                  
      if (FLASH->FSR_B & FLASH_FLAG_EOPERR != 0)				// Check for Error
        return 1;
    }
                   
    StartAddress += 0x20000;
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
uint32_t Address = ( uint32_t)adr;
uint32_t Size = sz;
__IO uint64_t *dest =  (__IO uint64_t *)Address;
  __IO uint64_t *src = (__IO uint64_t*)((uint32_t)buf);
  __IO uint8_t *dest_8bit;
  __IO uint8_t *src_8bit;
  
  int i = 0;
  
  if (IS_FLASH_BANK1_ADDRESS(Address))
  {
    //while (FLASH->FSR_A & FLASH_FLAG_BSY);
    FLASH->FCCR_A |= 0x0FEF000E;			
  }
  else
  {
    //while (FLASH->FSR_B & FLASH_FLAG_BSY);
    FLASH->FCCR_B |= 0x0FEF000E;	
  }
	
  while (Size) 
  {
    resetWDG();
    if (IS_FLASH_BANK1_ADDRESS((uint32_t)dest))
      FLASH->FCR_A &= ~(FLASH_FCR_A_PSIZE_A | 0x07F00);
    else
      FLASH->FCR_B &= ~(FLASH_FCR_B_PSIZE_B | 0x07F00);
    
    if (IS_FLASH_BANK1_ADDRESS((uint32_t)dest))
      FLASH->FCR_A = FLASH_FCR_A_PG_A | (3<<4);	
    else
      FLASH->FCR_B = FLASH_FCR_B_PG_B | (3<<4);
	
    if (Size>=32)
    {
      /* Program the 256 bits flash */
      for (i=0;i<4;i++)
      {
        *dest++ = *src++;
      }
      Size  -= 32;
    }
    else
    {
      dest_8bit =(uint8_t*)dest;
      src_8bit =(uint8_t*)src;
      /* Program the 256 bits flash */
      for (i=0;i<Size;i++)
      {
        *(uint8_t*)dest_8bit++= *(uint8_t*)src_8bit++;
      }
      for (i=0;i<(32 - Size);i++)
      {
        *(uint8_t*)dest_8bit++ = 0xFF;
      }		
      Size  = 0;
    }
		
    DSB	(	);
	
    if (IS_FLASH_BANK1_ADDRESS((uint32_t)dest))
    {
      while (FLASH->FSR_A & FLASH_FLAG_QW)
        resetWDG();
    }
    else
    {
      while (FLASH->FSR_B & FLASH_FLAG_QW)
        resetWDG();
    }

    if (FLASH->FSR_A & FLASH_FLAG_EOP == 0)					// Check for Error
    {
      if (IS_FLASH_BANK1_ADDRESS((uint32_t)dest))
        FLASH->FCR_A &= ~FLASH_FCR_A_PG_A;	
      else
        FLASH->FCR_B &= ~FLASH_FCR_B_PG_B;
			
      return 0;
    }
	
    if (IS_FLASH_BANK1_ADDRESS((uint32_t)dest))
      FLASH->FCR_A &= ~FLASH_FCR_A_PG_A;	
    else
      FLASH->FCR_B &= ~FLASH_FCR_B_PG_B;		
  }

return (0);                                           // Done
}
#endif

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR &= 0xF87FC00C;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  /* Reset PREDIV1[3:0] bits */
 // RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

  /* Reset USARTSW[1:0], I2CSW and TIMs bits */
 // RCC->CFGR3 &= (uint32_t)0xFF00FCCC;
  
  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings ----------------------------------*/
  /* Enable Prefetch Buffer and Flash 0 wait state */
  //FLASH->ACR = FLASH_ACR_PRFTBE;

  RCC->CR &= 0xFFFFFFE7;
  
  /* HCLK = SYSCLK / 1 */
  RCC->CFGR |= (uint32_t)RCC_D1CFGR_HPRE_DIV1;

  /* PCLK2 = HCLK / 1 */
  RCC->CFGR |= (uint32_t)RCC_D1CFGR_D1PPRE_DIV1;

  /* PCLK1 = HCLK / 1 */
  RCC->CFGR |= (uint32_t)RCC_D1CFGR_D1CPRE_1;
  
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | 0; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | 0; /* Vector Table Relocation in Internal FLASH. */
#endif  
}
void waitBusy(void)
{
  uint32_t stat = 0;
  resetWDG();
  /* CRC busy */
  stat = *(__IO uint32_t*) 0x52002010;
  while(stat & 9 != 0)
    stat = *(__IO uint32_t*) 0x52002010;
  
  stat = *(__IO uint32_t*) 0x52002110;
  while(stat & 9 != 0)
    stat = *(__IO uint32_t*) 0x52002110;
  
  /* EOP */
  stat = *(__IO uint32_t*) 0x5200200C;
  while(stat & 0x10000 == 0)
    stat = *(__IO uint32_t*) 0x5200200C;
  
  stat = *(__IO uint32_t*) 0x5200210C;
  while(stat & 0x10000 == 0)
    stat = *(__IO uint32_t*) 0x5200210C;
}


void resetWDG(void)
{
  *(__IO uint32_t*) 0x58004800 = (uint32_t) 0xAAAA;
  *(__IO uint32_t*) 0x58004800 = (uint32_t) 0x5555;
  *(__IO uint32_t*) 0x58004804 = (uint32_t) 0x6;
  
  *(__IO uint32_t*) 0x58004C00 = (uint32_t) 0xAAAA;
  *(__IO uint32_t*) 0x58004C00 = (uint32_t) 0x5555;
  *(__IO uint32_t*) 0x58004C04 = (uint32_t) 0x6;
  
  *(__IO uint32_t*) 0x40002C04 = (uint32_t) 0x1FF;
  *(__IO uint32_t*) 0x40002C00 = (uint32_t) 0x7F;
}

