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
#define SUCCEED  1
#define FAILED   0

/************************* FUNCTION PROTOTYPING ***********************/
/*
 * Description:
 * Function to erase data from specific sector in the flash memory given the start address
 * and the data size
 * */
uint8_t Flash_Memory_Erase(uint32_t StartSectorAddress , uint32_t dataSizeInBytes);
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

