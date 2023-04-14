/*
 * flash_memory.h
 *
 *  Created on: Dec 3, 2022
 *      Author: Nada Youssef
 */

#ifndef INC_FLASH_MEMORY_H_
#define INC_FLASH_MEMORY_H_

#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS **************************/
#define SUCCEED              1
#define FAILED               0

#define APP_START_ADDRESS_BANK1        0x08002100
#define APP_START_ADDRESS_BANK2        0x08102100
#define META_DATA_START_ADDRESS_BANK1  0x08002000
#define META_DATA_START_ADDRESS_BANK2  0x08102000

#define MAIN_APP_SECTOR_BANK1          FLASH_SECTOR_7
#define MAIN_APP_SECTOR_BANK2          FLASH_SECTOR_18;


/************************* TYPES DDECLERATIONS ************************/
typedef enum
{
	META_DATA,APP
}FLASH_DataType;

/************************* FUNCTION PROTOTYPING ***********************/
/*
 * Description:
 * Function to erase data from specific sector in the flash memory given the start address
 * and the data size
 * */
uint8_t Flash_Memory_Erase(uint32_t StartSectorAddress , uint32_t dataSizeInBytes);
/*
 * Description:
 * Function to get the inactive bank and erase it from the main Application address
 * */
uint8_t erase_inactive_bank(void);
/*
 * Description:
 * Function to write data in the flash memory from a specific starting address defined based on the type of data
 * and increase the starting address depending on the data written before when the function has been called.
 * */
uint8_t flash_memory_write(uint32_t *data, uint32_t dataSizeInWords, FLASH_DataType dataType);
/*
 * Description:
 * Function to write data in the flash memory
 * writing the corresponding data to the flash starting from a specific address.
 * */
uint8_t Flash_Memory_Write(uint32_t StartSectorAddress ,uint32_t *data, uint32_t dataSizeInBytes);
/*
 * Description:
 * Function to read data from the flash memory
 * starting from a specific address and store the data into array.
 * */
void Flash_Memory_Read (uint32_t StartSectorAddress, uint32_t *buffer, uint16_t numofWords);

#endif /* INC_FLASH_MEMORY_H_ */