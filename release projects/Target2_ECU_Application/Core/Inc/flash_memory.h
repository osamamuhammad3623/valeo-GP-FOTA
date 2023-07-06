/*
 * flash_memory.h
 *
 *  Created on: May 15, 2023
 *      Author: hp
 */

#ifndef INC_FLASH_MEMORY_H_
#define INC_FLASH_MEMORY_H_

#include "stm32f4xx_hal.h"

/******************************************************************************/
/******************************************************************************/
/******************************** Definitions *********************************/
/******************************************************************************/
/******************************************************************************/
#define SUCCEED              1
#define FAILED               0

#define APP_START_ADDRESS        0x08160000
#define COM_DATA_START_ADDRESS   0x08140000

#define START_SECTOR_BANK1          FLASH_SECTOR_6
#define START_SECTOR_BANK2          FLASH_SECTOR_18;

#define META_DATA_ADDRESS               0x08041000
#define MAJOR_V_METADATA_OFFSET           8
#define MINOR_V_METADATA_OFFSET           10
#define PATCH_V_METADATA_OFFSET           12
/******************************************************************************/
/******************************************************************************/
/**************************** TYPES DDECLERATIONS *****************************/
/******************************************************************************/
/******************************************************************************/
typedef enum
{
	COM_DATA,APP
}FLASH_DataType;

/******************************************************************************/
/******************************************************************************/
/*************************** Functions Prototypes *****************************/
/******************************************************************************/
/******************************************************************************/
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
/*
 * Description:
 * Function to erase the whole inactive bank completly
 * */
uint8_t erase_whole_inactive_bank(void);
/*
 * Description:
 * Function to read the current version from memory and return it.
 * inputs: pointer to buffer to fill it with the current version
 * */
void get_version_number(uint8_t* buffer);
#endif /* INC_FLASH_MEMORY_H_ */
