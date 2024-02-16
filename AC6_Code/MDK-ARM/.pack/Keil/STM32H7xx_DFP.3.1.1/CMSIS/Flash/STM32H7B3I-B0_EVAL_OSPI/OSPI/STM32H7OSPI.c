/**
  ******************************************************************************
  * @file    STM32H7OSPI.c
  * @author  MCD Application Team
  * @brief   This file defines the operations of the external loader for
  *          MX25LM51245G OSPI memory of STM32H7B3I/B0-EVAL.
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
#include "stm32h7b3i_eval_ospi.h"
#include <string.h>

 #include "./mx25lm51245g/mx25lm51245g.h"


 

BSP_OSPI_NOR_Init_t Flash;

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32H7B3I/B0_EVAL_OSPI_Private_Functions Private Functions
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
/** @defgroup STM32H7B3I/B0_EVAL_OSPI_Exported_Functions Exported Functions
  * @{
  */

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







/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
