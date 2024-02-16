#include "stm32h776xx.h"
#include "stm32h7xx_hal_rcc.h"
#define OB_SIZE  0x140
#define OB_START 0x52002018
#define OB_END   0x52002150
#define IS_FLASH_OB_ADDRESS(ADDRESS)       (((ADDRESS) >= OB_START) && ((ADDRESS) <  OB_END))
#define FLASH_KEY1                         ((uint32_t)0x45670123) /*!< Flash key1 */
#define FLASH_KEY2                         ((uint32_t)0xCDEF89AB) /*!< Flash key2: used with FLASH_KEY1 
                                                                       to unlock the FLASH registers access */
#define FLASH_PDKEY1                       ((uint32_t)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2                       ((uint32_t)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                                       to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_OPTKEY1                      ((uint32_t)0x08192A3B) /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                      ((uint32_t)0x4C5D6E7F) /*!< Flash option byte key2: used with FLASH_OPTKEY1*/ 

#define FLASH_FLAG_QW			   ((uint32_t)0x00000004) /*!< Flash BUSY QW*/
#define FLASH_FLAG_BSY			   ((uint32_t)0x00000009) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOP			   ((uint32_t)0x00010000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOPERR	           ((uint32_t)0x00400000) /*!< Flash BUSY FLAG*/

#define VOLTAGE_RANGE			   0//FLASH_FCR_A_PSIZE_A
#define FLASH_BANK1_BASE  FLASH_BASE
#define FLASH_BANK2_BASE  (FLASH_BASE + 0x00100000)
#define IS_FLASH_BANK1_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) <= (FLASH_BANK1_BASE + 0x000FFFFF) ))


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08080000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080C0000) /* Base @ of Sector 7, 128 Kbytes */

void SystemClockHSI_Config(void);
void waitBusy(void);
void resetWDG(void);