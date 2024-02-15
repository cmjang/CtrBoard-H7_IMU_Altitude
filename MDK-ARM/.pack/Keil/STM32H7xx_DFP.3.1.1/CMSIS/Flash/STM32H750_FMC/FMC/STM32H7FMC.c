
#include "stm32h7xxfmc.h"
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	return HAL_OK;
}




int Init_fmc (void)
{
  int result = 0;
  SystemInit(); 
  //HAL_Init();
  result = SystemClock_Config();
  if (result == 1)
    return result;
  result = NOR_Init();
  if (result == 1)
    return result;
  return 0;  
}

int SectorErase_fmc (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{ 
int result = 0;
  result =Sector_Erase ( EraseStartAddress , EraseEndAddress);
  return result;
	
}

int Write_fmc (uint32_t Address, uint32_t Size, uint16_t* Buffer)
{ 
  int result = 0;
  result =Write_FMC ( Address,  Size, Buffer);
  return result;

}

