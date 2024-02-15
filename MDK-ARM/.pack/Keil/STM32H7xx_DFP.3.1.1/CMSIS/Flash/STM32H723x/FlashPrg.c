/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32H723xx Flash                   */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char   vu8;
typedef          unsigned char    u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;
typedef volatile unsigned long long    vu64;
typedef          unsigned long long     u64;


typedef unsigned       int uint64_t;

#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))


#define FLASH_BASE      0x52002000

#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)



typedef struct
{
  vu32 ACR;             /*!< FLASH access control register,                           Address offset: 0x00 */
  vu32 KEYR1;           /*!< Flash Key Register for bank1,                            Address offset: 0x04 */
  vu32 OPTKEYR;         /*!< Flash Option Key Register,                                Address offset: 0x08 */
  vu32 CR1;             /*!< Flash Control Register for bank1,                        Address offset: 0x0C */
  vu32 SR1;             /*!< Flash Status Register for bank1,                         Address offset: 0x10 */
  vu32 CCR1;            /*!< Flash Control Register for bank1,                        Address offset: 0x14 */
  vu32 OPTCR;           /*!< Flash Option Control Register,                            Address offset: 0x18 */
  vu32 OPTSR_CUR;       /*!< Flash Option Status Current Register,                     Address offset: 0x1C */
  vu32 OPTSR_PRG;       /*!< Flash Option Status Current Register,                     Address offset: 0x20 */
  vu32 OPTCCR;          /*!< Flash Option Clear Control Register,                      Address offset: 0x24 */
  vu32 PRAR_CUR1;       /*!< Flash Current Protection Address Register for bank1,     Address offset: 0x28 */
  vu32 PRAR_PRG1;       /*!< Flash Protection Address to Program Register for bank1,  Address offset: 0x2C */
  vu32 SCAR_CUR1;       /*!< Flash Current Secure Address Register for bank1,         Address offset: 0x30 */
  vu32 SCAR_PRG1;       /*!< Flash Secure Address Register for bank1,                 Address offset: 0x34 */
  vu32 WPSN_CUR1;       /*!< Flash Current Write Protection Register on bank1,        Address offset: 0x38 */
  vu32 WPSN_PRG1;       /*!< Flash Write Protection to Program Register on bank1,     Address offset: 0x3C */
  vu32 BOOT_CUR;        /*!< Flash Current Boot Address for Pelican Core Register,     Address offset: 0x40 */
  vu32 BOOT_PRG;        /*!< Flash Boot Address to Program for Pelican Core Register,  Address offset: 0x44 */
  vu32 RESERVED0[2];    /*!< Reserved, 0x48 to 0x4C                                                        */
  vu32 CRCCR1;          /*!< Flash CRC Control register For Bank1 Register ,          Address offset: 0x50 */
  vu32 CRCSADD1;        /*!< Flash CRC Start Address Register for Bank1 ,             Address offset: 0x54 */
  vu32 CRCEADD1;        /*!< Flash CRC End Address Register for Bank1 ,               Address offset: 0x58 */
  vu32 CRCDATA;         /*!< Flash CRC Data Register for Bank1 ,                      Address offset: 0x5C */
  vu32 ECC_FA1;         /*!< Flash ECC Fail Address For Bank1 Register ,              Address offset: 0x60 */
  vu32 RESERVED1[40];   /*!< Reserved, 0x64 to 0x100                                                       */
  vu32 KEYR2;           /*!< Flash Key Register for bank2,                           Address offset: 0x104 */
  vu32 RESERVED2;       /*!< Reserved, 0x108                                                               */
  vu32 CR2;             /*!< Flash Control Register for bank2,                       Address offset: 0x10C */
  vu32 SR2;             /*!< Flash Status Register for bank2,                        Address offset: 0x110 */
  vu32 CCR2;            /*!< Flash Status Register for bank2,                        Address offset: 0x114 */
  vu32 RESERVED3[4];    /*!< Reserved, 0x118 to 0x124                                                      */
  vu32 PRAR_CUR2;       /*!< Flash Current Protection Address Register for bank2,    Address offset: 0x128 */
  vu32 PRAR_PRG2;       /*!< Flash Protection Address to Program Register for bank2, Address offset: 0x12C */
  vu32 SCAR_CUR2;       /*!< Flash Current Secure Address Register for bank2,        Address offset: 0x130 */
  vu32 SCAR_PRG2;       /*!< Flash Secure Address Register for bank2,                Address offset: 0x134 */
  vu32 WPSN_CUR2;       /*!< Flash Current Write Protection Register on bank2,       Address offset: 0x138 */
  vu32 WPSN_PRG2;       /*!< Flash Write Protection to Program Register on bank2,    Address offset: 0x13C */
  vu32 RESERVED4[4];    /*!< Reserved, 0x140 to 0x14C                                                      */
  vu32 CRCCR2;          /*!< Flash CRC Control register For Bank2 Register ,         Address offset: 0x150 */
  vu32 CRCSADD2;        /*!< Flash CRC Start Address Register for Bank2 ,            Address offset: 0x154 */
  vu32 CRCEADD2;        /*!< Flash CRC End Address Register for Bank2 ,              Address offset: 0x158 */
  vu32 CRCDATA2;        /*!< Flash CRC Data Register for Bank2 ,                     Address offset: 0x15C */
  vu32 ECC_FA2;         /*!< Flash ECC Fail Address For Bank2 Register ,             Address offset: 0x160 */
} FLASH_TypeDef;





/*******************  Bits definition for FLASH_CR register  ***********************/
#define FLASH_CR_LOCK                     0x00000001U
#define FLASH_CR_PG                       0x00000002U
#define FLASH_CR_SER                      0x00000004U
#define FLASH_CR_BER                      0x00000008U

#define FLASH_CR_PSIZE                    0x00000030U
#define FLASH_CR_START                    0x00000080U

#define FLASH_CR_SNB                      0x00000700U



/*******************  Bits definition for FLASH_SR register  ***********************/
#define FLASH_SR_BSY                      0x00000001U


#define FLASH_KEY1                         ((vu32)0x45670123) /*!< Flash key1 */
#define FLASH_KEY2                         ((vu32)0xCDEF89AB) /*!< Flash key2: used with FLASH_KEY1 
                                                                       to unlock the FLASH registers access */
#define FLASH_PDKEY1                       ((vu32)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2                       ((vu32)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                                       to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_OPTKEY1                      ((vu32)0x08192A3B) /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                      ((vu32)0x4C5D6E7F) /*!< Flash option byte key2: used with FLASH_OPTKEY1*/ 


#define FLASH_FLAG_EOP										 ((vu32)0x00010000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOPERR									 ((vu32)0x00400000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_QW			                 ((vu32)0x00000004) /*!< Flash BUSY FLAG_wait queue flag*/

#define FLASH_CR1_SNB1                      ((vu32)0x00000700)

#define FLASH_BANK1_BASE          ((vu32)0x08000000) /*!< Base address of : Flash Bank1 accessible over AXI */ 
//#define FLASH_BANK2_BASE          ((vu32)0x08100000) /*!< Base address of : Flash Bank2  accessible over AXI */ 



#define IS_FLASH_BANK1_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) <= (FLASH_BANK1_BASE + 0x000FFFFF) ))



/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM 
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {


  FLASH->CR1 = 0x006F0000; // Reset Error Flags
	while (FLASH->SR1 & FLASH_FLAG_QW);									
	
	/* Unlock FLASH A Registers access */
	FLASH->KEYR1 = FLASH_KEY1;
	FLASH->KEYR1 = FLASH_KEY2;
	

	
#if 0
    
  EraseSector((unsigned long)  0x08000000);
	ProgramPage ((unsigned long) 0x08000000, 128, (unsigned char *)0x20000000);
    
#endif
  return (0);
}
#endif



/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM 
int UnInit (unsigned long fnc) {

  FLASH->CR1 |=  1;                             // Lock Flash BAnk A
	//FLASH->CR2 |=  1;                             // Lock Flash BAnk B

  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {
	

	while (FLASH->SR1 & FLASH_SR_BSY);									// Reset Error Flags of Bank 1
	

	while (FLASH->SR1 & FLASH_SR_BSY) {	               // Wait BSY Flag
  }
	
	
	/* Bank1 erase */
	FLASH->CR1 &= (~FLASH_CR_PSIZE);
	FLASH->CR1 |= FLASH_CR_BER; 
	FLASH->CR1 |= FLASH_CR_START;
	
  while (FLASH->SR1 & FLASH_SR_BSY) {                // Wait BSY Flag Bank 1
  }
	
	FLASH->CR1 &= ~FLASH_CR_BER;

	
  return (0);                                          
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
	
	u8 Sector = ((adr & 0x00FFFFFF) >> 17)& 0xF;
	
	
	/* BankA erase */
	if (IS_FLASH_BANK1_ADDRESS(adr))
  {

	  while (FLASH->SR1 & FLASH_FLAG_QW){
		}
			
		FLASH->CR1 &= (~FLASH_CR1_SNB1);   //clear the SNB (sector erase selection number) BIT
    FLASH->CR1 &= ~(FLASH_CR_PSIZE | 0x07F00);
    FLASH->CR1 |= (FLASH_CR_SER | (Sector << 8));	
    FLASH->CR1 |= FLASH_CR_START;	
		
		while (FLASH->SR1 & FLASH_FLAG_QW);                  // Wait BSY Flag Bank 1
		
		FLASH->CR1 &= ~FLASH_CR_SER;
		
		if ((FLASH->SR1 & FLASH_FLAG_EOPERR) != 0)							// Check for Error
			return 1;
  }
		
  return (0);                                           
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

	 u64 *dest = ( u64 *)adr;
   u64 *src = ( u64*)((u32)buf);
	int i = 0;
	
	
  	/* BankA  */
	if (IS_FLASH_BANK1_ADDRESS(adr))
	{
		while (FLASH->SR1 & FLASH_FLAG_QW);
		while (FLASH->SR1 & FLASH_SR_BSY);
														
	}
	
	 FLASH->CR1 = 0x006F0000; // Reset Error Flags
   
	while (sz) {
		if (IS_FLASH_BANK1_ADDRESS(adr))
		{
			FLASH->CR1 &= ~(FLASH_CR_PSIZE | 0x07F00);
			FLASH->CR1 = FLASH_CR_PG;			 // Programming Enabled 
		
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
      
		    u8 *dest8 = ( u8*)dest;
        u8 *src8 = ( u8*)(src);
			
			 /*write a word completed with 0xFF*/		
				for (i=0;i<(sz);i++)
				{
				 	*(dest8)++ = *(src8)++;
				}
				for (i=0;i<((32 - sz));i++)
				{
					*(dest8)++ = 0xFF;
				}
							
			sz  = 0;
		}
			

			while (FLASH->SR1 & FLASH_FLAG_QW){
			}
		  while (FLASH->SR1 & FLASH_SR_BSY);
	
		if ((FLASH->SR1 & FLASH_FLAG_EOP) == 0)							// Check for Error
		{
			if (IS_FLASH_BANK1_ADDRESS(adr))
				FLASH->CR1 &= ~FLASH_CR_PG;			 				 	// Programming disabled 
			
			return (1);
		}
	  		
  }
}
  return (0);                                           
}
#endif
