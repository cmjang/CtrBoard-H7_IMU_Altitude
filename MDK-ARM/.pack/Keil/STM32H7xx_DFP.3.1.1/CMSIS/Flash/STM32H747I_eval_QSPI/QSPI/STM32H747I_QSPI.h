/**
  ******************************************************************************
  * @file    STM32H747I_QSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32H747I_QSPI.c
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H747I_EVAL_QSPI
#define __STM32H747I_EVAL_QSPI

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval_qspi.h"
#include <string.h>


#define TIMEOUT 5000U




/* Private function prototypes -----------------------------------------------*/
int Init_QSPI(void);
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);

 HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
 int MassErase(void);
void ResetMemory(QSPI_HandleTypeDef *hqspi);
int SystemClock_Config(void);
void HAL_MspInit(void);


static uint8_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_DummyCyclesCfg       (QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_WriteEnable          (QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);
static uint8_t QSPI_EnterQPI(QSPI_HandleTypeDef *hqspi);
void ResetMemory(QSPI_HandleTypeDef *hqspi);

#endif /* __STM32H747I_EVAL_QSPI */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
