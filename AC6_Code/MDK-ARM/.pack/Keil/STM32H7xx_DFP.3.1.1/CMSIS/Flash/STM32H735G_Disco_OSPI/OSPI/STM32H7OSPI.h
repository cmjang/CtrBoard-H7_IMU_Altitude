/**
  ******************************************************************************
  * @file    STM32H7OSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32H7OSPI.c
  *           
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


#ifndef __STM32H7OSPI_H
#define __STM32H7OSPI_H
#include "stdint.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h735g_discovery_ospi.h"


#define TIMEOUT 5000U


/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
 int MassErase ( void);


#endif
