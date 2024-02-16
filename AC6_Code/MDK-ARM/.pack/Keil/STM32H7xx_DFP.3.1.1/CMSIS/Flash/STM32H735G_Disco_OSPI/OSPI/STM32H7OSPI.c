/**
  ******************************************************************************
  * @file    STM32H7OSPI.c
  * @author  MCD Application Team
  * @brief   This file defines the operations of the external loader for
  *          MX25LM51245G OSPI memory of STM32H735-Disco.
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

#include "STM32H7OSPI.h"
#include "stm32h735g_discovery_ospi.h"
#include <string.h>

 #include "./mx25lm51245g/mx25lm51245g.h"


 

BSP_OSPI_NOR_Init_t Flash;

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32H735_Disco_OSPI_Private_Functions Private Functions
  * @{
  */

void HAL_Delay(uint32_t Delay)
{
  int i=0;
  for (i=0; i<0x1000; i++);
}
 uint32_t HAL_GetTick(void)
{
  int i;
  for (i=0;i<1024;i++);
  uwTick++;
  return uwTick;
}
/** @defgroup STM32H735_Disco_OSPI_Exported_Functions Exported Functions
  * @{
  */
 static int SystemClock_Config(void);
/**
  * @brief  Initializes the OSPI interface.
  * @param  Instance   OSPI Instance
  * @param  Init       OSPI Init structure
  * @retval BSP status
  */
 HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{ 
  return HAL_OK;
}
/**
  * @brief  System initialization.
  * @param  None
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */
int Init_OSPI()
{   int32_t result=0;

 
  /* Zero Init structs */
  memset(&Flash,0,sizeof(Flash));
  SystemInit(); 
	 
   Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
   Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;

  /* Configure the system clock  */
   SystemClock_Config();

	 HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); 

/*Initialaize OSPI*/
   if(BSP_OSPI_NOR_Init(0,&Flash) !=0)
    return 0;
/*Configure the OSPI in memory-mapped mode*/
   result=BSP_OSPI_NOR_EnableMemoryMappedMode(0);  

	 if(result==0)
    return 1; 

   return 0;
}

/**
  * @brief   erase memory.
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */

 int MassErase (void){
 /*Initialaize OSPI*/
	 BSP_OSPI_NOR_DeInit(0);
   BSP_OSPI_NOR_Init(0,&Flash);
	
/*Erases the entire OSPI memory*/
	 BSP_OSPI_NOR_Erase_Chip(0);

  /*Reads current status of the OSPI memory*/
	while (BSP_OSPI_NOR_GetStatus(0)!=BSP_ERROR_NONE){};

	 return 0;
 
 }
 
 /**
  * @brief   Program memory.
  * @param   Address: page address
  * @param   Size   : size of data
  * @param   buffer : pointer to data buffer
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    /*Initialaize OSPI*/
    BSP_OSPI_NOR_DeInit(0);
    Address = Address & 0x0fffffff;
    BSP_OSPI_NOR_Init(0,&Flash);
   
	/*Writes data to the OSPI memory*/
    BSP_OSPI_NOR_Write(0,buffer,Address, Size);
  
   return 1;
}


/**
  * @brief   Sector erase.
  * @param   EraseStartAddress :  erase start address
  * @param   EraseEndAddress   :  erase end address
  * @retval  None
  */
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
	
  uint32_t BlockAddr;
  EraseStartAddress &= 0x0FFFFFFF;  
  EraseEndAddress &= 0x0FFFFFFF;
  EraseStartAddress = EraseStartAddress -  EraseStartAddress % 0x10000;
 /*Initialaize OSPI*/
	BSP_OSPI_NOR_DeInit(0);
	BSP_OSPI_NOR_Init(0,&Flash);	
	
	  while (EraseEndAddress>=EraseStartAddress)
  {
    BlockAddr = EraseStartAddress;
  /*Erases the specified block of the OSPI memory*/
   BSP_OSPI_NOR_Erase_Block(0,BlockAddr, MX25LM51245G_ERASE_64K );
 
  /*Reads current status of the OSPI memory*/
		while (BSP_OSPI_NOR_GetStatus(0)!=0);

		EraseStartAddress+=0x10000;
		/*Configure the OSPI in memory-mapped mode*/
    BSP_OSPI_NOR_EnableMemoryMappedMode(0);
  }

 
  return 1;	
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */

static int SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  //HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) {};
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    while(1) {};
  }

 
return 1;

}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
