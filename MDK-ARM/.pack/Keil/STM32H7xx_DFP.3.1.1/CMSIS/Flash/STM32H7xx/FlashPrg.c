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
 * Project:      Flash Programming Functions for ST STM32H7xx
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */

#include "..\FlashOS.h"        /* FlashOS Structures */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))

/* Peripheral Memory Map */
#define FLASH_BASE      0x52002000U

/* Flash Registers */
#define FLASH           ((FLASH_TypeDef  *) FLASH_BASE)

/* Flash Registers */
typedef struct {
  vu32 ACR;                              /* Offset: 0x000  FLASH access control register */
  vu32 KEYR1;                            /* Offset: 0x004  Flash Key Register for bank1 */
  vu32 OPTKEYR;                          /* Offset: 00x08  Flash Option Key Register */
  vu32 CR1;                              /* Offset: 0x00C  Flash Control Register for bank1 */
  vu32 SR1;                              /* Offset: 0x010  Flash Status Register for bank1 */
  vu32 CCR1;                             /* Offset: 0x014  Flash Control Register for bank1 */
  vu32 OPTCR;                            /* Offset: 0x018  Flash Option Control Register */
  vu32 OPTSR_CUR;                        /* Offset: 0x01C  Flash Option Status Current Register */
  vu32 OPTSR_PRG;                        /* Offset: 0x020  Flash Option Status Current Register */
  vu32 OPTCCR;                           /* Offset: 0x024  Flash Option Clear Control Register */
  vu32 PRAR_CUR1;                        /* Offset: 0x028  Flash Current Protection Address Register for bank1 */
  vu32 PRAR_PRG1;                        /* Offset: 0x02C  Flash Protection Address to Program Register for bank1 */
  vu32 SCAR_CUR1;                        /* Offset: 0x030  Flash Current Secure Address Register for bank1 */
  vu32 SCAR_PRG1;                        /* Offset: 0x034  Flash Secure Address Register for bank1 */
  vu32 WPSN_CUR1;                        /* Offset: 0x038  Flash Current Write Protection Register on bank1 */
  vu32 WPSN_PRG1;                        /* Offset: 0x03C  Flash Write Protection to Program Register on bank1 */
  vu32 BOOT_CUR;                         /* Offset: 0x040  Flash Current Boot Address for Pelican Core Register */
  vu32 BOOT_PRG;                         /* Offset: 0x044  Flash Boot Address to Program for Pelican Core Register */
  vu32 RESERVED0[2];
  vu32 CRCCR1;                           /* Offset: 0x050  Flash CRC Control register For Bank1 Register */
  vu32 CRCSADD1;                         /* Offset: 0x054  Flash CRC Start Address Register for Bank1 */
  vu32 CRCEADD1;                         /* Offset: 0x058  Flash CRC End Address Register for Bank1 */
  vu32 CRCDATA;                          /* Offset: 0x05C  Flash CRC Data Register for Bank1 */
  vu32 ECC_FA1;                          /* Offset: 0x060  Flash ECC Fail Address For Bank1 Register */
  vu32 RESERVED1[40];
  vu32 KEYR2;                            /* Offset: 0x104  Flash Key Register for bank2 */
  vu32 RESERVED2;
  vu32 CR2;                              /* Offset: 0x10C  Flash Control Register for bank2 */
  vu32 SR2;                              /* Offset: 0x110  Flash Status Register for bank2 */
  vu32 CCR2;                             /* Offset: 0x114  Flash Status Register for bank2 */
  vu32 RESERVED3[4];
  vu32 PRAR_CUR2;                        /* Offset: 0x128  Flash Current Protection Address Register for bank2 */
  vu32 PRAR_PRG2;                        /* Offset: 0x12C  Flash Protection Address to Program Register for bank2 */
  vu32 SCAR_CUR2;                        /* Offset: 0x130  Flash Current Secure Address Register for bank2 */
  vu32 SCAR_PRG2;                        /* Offset: 0x134  Flash Secure Address Register for bank2 */
  vu32 WPSN_CUR2;                        /* Offset: 0x138  Flash Current Write Protection Register on bank2 */
  vu32 WPSN_PRG2;                        /* Offset: 0x13C  Flash Write Protection to Program Register on bank2 */
  vu32 RESERVED4[4];
  vu32 CRCCR2;                           /* Offset: 0x150  Flash CRC Control register For Bank2 Register */
  vu32 CRCSADD2;                         /* Offset: 0x154  Flash CRC Start Address Register for Bank2 */
  vu32 CRCEADD2;                         /* Offset: 0x158  Flash CRC End Address Register for Bank2 */
  vu32 CRCDATA2;                         /* Offset: 0x15C  Flash CRC Data Register for Bank2 */
  vu32 ECC_FA2;                          /* Offset: 0x160  Flash ECC Fail Address For Bank2 Register */
} FLASH_TypeDef;


/* Flash Keys */
#define FLASH_KEY1           0x45670123  /* Flash key1 */
#define FLASH_KEY2           0xCDEF89AB  /* Flash key2: used with FLASH_KEY1 to unlock the FLASH registers access */
#define FLASH_PDKEY1         0x04152637  /* Flash power down key1 */
#define FLASH_PDKEY2         0xFAFBFCFD  /* Flash power down key2: used with FLASH_PDKEY1 to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_OPTKEY1        0x08192A3B  /* Flash option byte key1 */
#define FLASH_OPTKEY2        0x4C5D6E7F  /* Flash option byte key2: used with FLASH_OPTKEY1*/

/* Flash Control Register definitions */
#define FLASH_CR_LOCK       (1U <<  0)
#define FLASH_CR_PG         (1U <<  1)
#define FLASH_CR_SER        (1U <<  2)
#define FLASH_CR_BER        (1U <<  3)
#define FLASH_CR_PSIZE      (3U <<  4)
#define FLASH_CR_PSIZE_0    (0U <<  4)   /* Flash program/erase by  8 bits */
#define FLASH_CR_PSIZE_1    (1U <<  4)   /* Flash program/erase by 16 bits */
#define FLASH_CR_PSIZE_2    (2U <<  4)   /* Flash program/erase by 32 bits */
#define FLASH_CR_PSIZE_3    (3U <<  4)   /* Flash program/erase by 64 bits */
#define FLASH_CR_PSIZE_VAL  FLASH_CR_PSIZE_3
#define FLASH_CR_START      (1U <<  7)
#define FLASH_CR_SNB        (7U <<  8)

/* Flash Status Register definitions */
#define FLASH_SR_BSY        (1U <<  0)   /* busy flag */
#define FLASH_SR_QW         (1U <<  2)   /* wait queue flag */
#define FLASH_SR_WRPERR     (1U << 17)   /* Write Protection Error on flag */
#define FLASH_SR_PGSERR     (1U << 18)   /* Program Sequence Error on flag */
#define FLASH_SR_STRBERR    (1U << 19)   /* strobe Error on flag */
#define FLASH_SR_INCERR     (1U << 21)   /* Inconsistency Error on flag */
#define FLASH_SR_OPERR      (1U << 22)   /* Operation Error on flag */
#define FLASH_SR_RDPERR     (1U << 23)   /* Read Protection Error on flag */
#define FLASH_SR_RDSERR     (1U << 24)   /* Read Secured Error on flag */
#define FLASH_SR_SNECCERR   (1U << 25)   /* Single ECC Error Correction on flag */
#define FLASH_SR_DBECCERR   (1U << 26)   /* Double Detection ECC Error on flag */
#define FLASH_SR_CRCRDERR   (1U << 28)   /* CRC Read Error on bank flag */
#define FLASH_BSY            FLASH_SR_QW

#define FLASH_PGERR         (FLASH_SR_WRPERR   | FLASH_SR_PGSERR   | \
                             FLASH_SR_STRBERR  | FLASH_SR_INCERR   | \
                             FLASH_SR_OPERR    | FLASH_SR_RDPERR   | \
                             FLASH_SR_RDSERR   | FLASH_SR_SNECCERR | \
                             FLASH_SR_DBECCERR | FLASH_SR_CRCRDERR  )


#if defined FLASH_MEM
  #define FLASH_START             (0x08000000U)
  #if   defined STM32H7x_2048      /* 2 x 1024 KB */
    #define FLASH_BANK_SIZE       (0x00100000U)
  #else
    #error STM32H7xx Flash size not defined!
  #endif
#endif /* FLASH_MEM */


void __NOP(void) {
    __asm("NOP");
}

void __DSB(void) {
    __asm("DSB");
}

void __ISB(void) {
    __asm("ISB");
}


/*
 * Get Bank Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Bank Number
 */

#if defined FLASH_MEM
unsigned long GetBankNum(unsigned long adr) {
  unsigned long bankNum;

  if (adr >= (FLASH_START + FLASH_BANK_SIZE)) {
    bankNum = 1U;
  }
  else {
    bankNum = 0U;
  }

  return (bankNum);
}
#endif /* FLASH_MEM */


/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

#if defined FLASH_MEM
unsigned long GetSecNum (unsigned long adr) {
  unsigned long secNum;

  secNum = ((adr & (FLASH_BANK_SIZE - 1)) >> 17) & 0xF;

  return (secNum);
}
#endif /* FLASH_MEM */


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  FLASH->KEYR1 = FLASH_KEY1;                     /* Unlcock FLASH A Registers access */
  FLASH->KEYR1 = FLASH_KEY2;
  FLASH->CCR1  = FLASH_PGERR;                    /* Clear status register  */

  FLASH->KEYR2 = FLASH_KEY1;                     /* Unlcock FLASH B Registers access */
  FLASH->KEYR2 = FLASH_KEY2;
  FLASH->CCR2  = FLASH_PGERR;                    /* Clear status register  */

  return (0);
}
#endif /* FLASH_MEM */


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int UnInit (unsigned long fnc) {

  FLASH->CR1 |=  FLASH_CR_LOCK;                  /* Lock FLASH A Registers access */

  FLASH->CR2 |=  FLASH_CR_LOCK;                  /* Lock FLASH B Registers access */

  return (0);
}
#endif /* FLASH_MEM */


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {

  FLASH->CCR1 = FLASH_PGERR;                     /* Clear status register  */
  FLASH->CR1  = FLASH_CR_BER | FLASH_CR_PSIZE_VAL;
  FLASH->CR1 |= FLASH_CR_START;

  while (FLASH->SR1 & FLASH_BSY) __NOP();
  FLASH->CR1  =  0;                              /* Reset command register */


  FLASH->CCR2 = FLASH_PGERR;                     /* Clear status register  */
  FLASH->CR2  = FLASH_CR_BER | FLASH_CR_PSIZE_VAL;
  FLASH->CR2 |= FLASH_CR_START;

  while (FLASH->SR2 & FLASH_BSY) __NOP();
  FLASH->CR2  =  0;                              /* Reset command register */

  return (0);
}
#endif /* FLASH_MEM */


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
  unsigned long b, s;

  b = GetBankNum(adr);                           /* get Flash Bank number */
  s = GetSecNum(adr);                            /* get Flash sector number */

  if (b == 0) {
    FLASH->CCR1 = FLASH_PGERR;                   /* Clear status register  */
    FLASH->CR1  = ((s << 8) | FLASH_CR_PSIZE_VAL | FLASH_CR_SER);
    FLASH->CR1 |= FLASH_CR_START;
    __ISB();
    __DSB();

    while (FLASH->SR1 & FLASH_BSY) __NOP();
    FLASH->CR1 = 0;                               /* Reset command register */

    if (FLASH->SR1 & FLASH_PGERR)
      return (1);
  }
  else
  {
    FLASH->CCR2 = FLASH_PGERR;                    /* Clear status register  */
    FLASH->CR2  = ((s << 8) | FLASH_CR_PSIZE_VAL | FLASH_CR_SER);
    FLASH->CR2 |= FLASH_CR_START;
    __ISB();
    __DSB();

    while (FLASH->SR2 & FLASH_BSY) __NOP();
    FLASH->CR2 = 0;                              /* Reset command register */

    if (FLASH->SR2 & FLASH_PGERR)
      return (1);
  }

  return (0);
}
#endif /* FLASH_MEM */


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long b;
  unsigned long *dest_addr = ( u32 *)adr;
  unsigned long *src_addr  = ( u32 *)buf;
  unsigned long row_index  = 8;

  b = GetBankNum(adr);                           /* get Flash Bank number */
  sz = (sz + 31) & ~31;                          /* Adjust size for 8 words (256 Bit) */

  while (sz) {
    if (b == 0) {
      FLASH->CCR1 = FLASH_PGERR;                 /* Clear status register  */
      FLASH->CR1  = FLASH_CR_PSIZE_VAL | FLASH_CR_PG;
    }
    else {
      FLASH->CCR2 = FLASH_PGERR;                 /* Clear status register  */
      FLASH->CR2  = FLASH_CR_PSIZE_VAL | FLASH_CR_PG;
    }
    __ISB();
    __DSB();

    /* Program the 256 bits flash word */
    row_index  = 8;
    do
    {
      *dest_addr = *src_addr;
      dest_addr++;
      src_addr++;
      row_index--;
    } while (row_index != 0U);
    __ISB();
    __DSB();

    if (b == 0) {
      while (FLASH->SR1 & FLASH_BSY) __NOP();
      FLASH->CR1 = 0;                            /* Reset command register */

      if (FLASH->SR1 & FLASH_PGERR)
        return (1);
    }
    else {
      while (FLASH->SR2 & FLASH_BSY) __NOP();
      FLASH->CR2 = 0;                            /* Reset command register */

      if (FLASH->SR2 & FLASH_PGERR)
        return (1);
    }

    sz  -= 32;
  }

  return (0);
}
#endif /* FLASH_MEM */
