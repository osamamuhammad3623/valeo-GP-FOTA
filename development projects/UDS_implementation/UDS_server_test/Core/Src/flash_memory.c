/*
 * flash_memory_v2.c
 *
 *  Created on: May 11, 2023
 *      Author: Nada Youssef
 */

#include "flash_memory.h"
#include "stm32f4xx_hal.h"
#include "rtc.h"

/*
 * Description:
 * function to get the sector number of a specific address.
 * */
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	/* BANK 1 */
	if((Address >= 0x08000000 ) && (Address < 0x08003FFF))
	{
		sector = FLASH_SECTOR_0;
	}

	else if((Address >= 0x08004000 ) && (Address <  0x08007FFF))
	{
		sector = FLASH_SECTOR_1;
	}

	else if((Address >= 0x08008000) && (Address <  0x0800BFFF))
	{
		sector = FLASH_SECTOR_2;
	}

	else if((Address >= 0x0800C000) && (Address < 0x0800FFFF))
	{
		sector = FLASH_SECTOR_3;
	}

	else if((Address >=  0x08010000) && (Address <  0x0801FFFF))
	{
		sector = FLASH_SECTOR_4;
	}

	else if((Address >=  0x08020000) && (Address < 0x0803FFFF))
	{
		sector = FLASH_SECTOR_5;
	}

	else if((Address >= 0x08040000) && (Address < 0x0805FFFF))
	{
		sector = FLASH_SECTOR_6;
	}

	else if((Address >= 0x08060000) && (Address < 0x0807FFFF))
	{
		sector = FLASH_SECTOR_7;
	}

	else if((Address >= 0x08080000) && (Address < 0x0809FFFF))
	{
		sector = FLASH_SECTOR_8;
	}

	else if((Address >= 0x080A0000) && (Address < 0x080BFFFF))
	{
		sector = FLASH_SECTOR_9;
	}

	else if((Address >= 0x080C0000) && (Address < 0x080DFFFF))
	{
		sector = FLASH_SECTOR_10;
	}

	else if((Address >= 0x080E0000) && (Address < 0x080FFFFF))
	{
		sector = FLASH_SECTOR_11;
	}

	/* BANK 2 */
	if((Address >= 0x08100000 ) && (Address < 0x08103FFF))
	{
		sector = FLASH_SECTOR_12;
	}

	else if((Address >= 0x08104000 ) && (Address <  0x08107FFF))
	{
		sector = FLASH_SECTOR_13;
	}

	else if((Address >= 0x08108000) && (Address <  0x0810BFFF))
	{
		sector = FLASH_SECTOR_14;
	}

	else if((Address >= 0x0810C000) && (Address < 0x0810FFFF))
	{
		sector = FLASH_SECTOR_15;
	}

	else if((Address >=  0x08110000) && (Address <  0x0811FFFF))
	{
		sector = FLASH_SECTOR_16;
	}

	else if((Address >=  0x08120000) && (Address < 0x0813FFFF))
	{
		sector = FLASH_SECTOR_17;
	}

	else if((Address >= 0x08140000) && (Address < 0x0815FFFF))
	{
		sector = FLASH_SECTOR_18;
	}

	else if((Address >= 0x08160000) && (Address < 0x0817FFFF))
	{
		sector = FLASH_SECTOR_19;
	}

	else if((Address >= 0x08180000) && (Address < 0x0819FFFF))
	{
		sector = FLASH_SECTOR_20;
	}

	else if((Address >= 0x081A0000) && (Address < 0x081BFFFF))
	{
		sector = FLASH_SECTOR_21;
	}

	else if((Address >= 0x081C0000) && (Address < 0x081DFFFF))
	{
		sector = FLASH_SECTOR_22;
	}

	else if((Address >= 0x081E0000) && (Address < 0x081FFFFF))
	{
		sector = FLASH_SECTOR_23;
	}
	return sector;
}


uint8_t Flash_Memory_Erase(uint32_t StartSectorAddress , uint32_t dataSizeInBytes){
	static FLASH_EraseInitTypeDef EraseInitStruct;   /* Structure to erase the flash area */
	uint32_t SECTORError;

	/* Getting the number of sector to erase from the first sector */
	uint32_t StartSector = GetSector(StartSectorAddress);                /*getting the start sector number*/
	uint32_t EndSectorAddress = StartSectorAddress + dataSizeInBytes;    /*getting the end sector address*/
	uint32_t EndSector = GetSector(EndSectorAddress);                    /*getting the end sector number*/

	/* Filling the erasing structure */
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = StartSector;
	EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

	/* Unlocking the Flash control register */
	HAL_FLASH_Unlock();

	/* check if the erasing process is done correctly */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*Error occurred while page erase*/
		return FAILED;
	}

	/* Locking the Flash control register */
	HAL_FLASH_Lock();

	return SUCCEED;
}


uint8_t Flash_Memory_Write(uint32_t StartSectorAddress ,uint32_t *data, uint32_t dataSizeInBytes){
	uint32_t numofWords=dataSizeInBytes/4;     /*getting number of words to write*/
	uint32_t numofWordsWritten=0;

	/* Unlocking the Flash control register */
	HAL_FLASH_Unlock();

	/* looping on the data word by word to write it in the flash */
	while(numofWordsWritten < numofWords){

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, data[numofWordsWritten]) == HAL_OK)
		{
			StartSectorAddress += 4;
			numofWordsWritten++;
		}
		else
		{
			/* Error occurred while writing data in Flash memory*/
			return FAILED;
		}

	}

	/* Locking the Flash control register */
	HAL_FLASH_Lock();

	return SUCCEED;

}



uint8_t erase_inactive_bank(void){

	static FLASH_EraseInitTypeDef EraseInitStruct;   /* Structure to erase the flash area */
	uint32_t SECTORError;


	/*check the inactive bank to erase*/
	uint32_t InactiveBank = !read_backup_reg(APP_SELECTOR_REG);


	/* Filling the erasing structure */
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;

    /* Determine the start sector based on which Bank that will be erased */
	switch (InactiveBank){
	/*erase from sector 7 in case of bank 1*/
	case BANK1:
		EraseInitStruct.Sector        = START_SECTOR_BANK1;
		EraseInitStruct.NbSectors       = 11;
		break;
	/*erase from sector 18 in case of bank 2*/
	case BANK2:
		EraseInitStruct.Sector        = START_SECTOR_BANK2;
		EraseInitStruct.NbSectors       = 7;
		break;

	default:
		return FAILED;

	}


	/* Unlocking the Flash control register */
	HAL_FLASH_Unlock();

	/* check if the erasing process is done correctly */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*Error occurred while erasing*/
		return FAILED;
	}

	/* Locking the Flash control register */
	HAL_FLASH_Lock();

	return SUCCEED;

}



void Flash_Memory_Read (uint32_t StartSectorAddress, uint32_t *buffer, uint16_t numofWords){

	while (1)
	{
		/* store the data in the 32 bit array */
		*buffer = *(uint32_t *)StartSectorAddress;
		StartSectorAddress += 4;
		buffer++;
		if (!(numofWords--)) break;
	}

}



uint8_t flash_memory_write(uint32_t *data, uint32_t dataSizeInWords, FLASH_DataType dataType){
	/* begin from the start
	 * initialized only once, then increased at each call
	 * depending on the data size that will be written */
	static uint32_t StartAddress=0;
	uint32_t numofWords=dataSizeInWords;     /*getting number of words to write*/
	uint32_t numofWordsWritten=0;
	uint8_t result;
	static uint8_t ComFlag=0;
	static uint8_t AppFlag=0;


	/*depending on the type of data to be written, the start address will be determined*/

	switch (dataType){
	case COM_DATA:
		/*check the inactive bank to write in*/
		if (ComFlag==0){
		if(read_backup_reg(APP_SELECTOR_REG)){
			/* active bank -> bank2
			 * inactive bank -> bank1
			 */
			StartAddress=COM_DATA_START_ADDRESS_BANK1;
		}
		else{
			/* active bank -> bank1
			 * inactive bank -> bank2
			 */
			StartAddress=COM_DATA_START_ADDRESS_BANK2;
		}
		ComFlag=1;
		}
		else{

		}

		break;

	case APP:
		if (AppFlag==0){
		/*check the inactive bank to write in*/
		if(read_backup_reg(APP_SELECTOR_REG)){
			/* active bank -> bank2
			 * inactive bank -> bank1
			 */
			StartAddress=APP_START_ADDRESS_BANK1;
		}
		else{
			/* active bank -> bank1
			 * inactive bank -> bank2
			 */
			StartAddress=APP_START_ADDRESS_BANK2;
		}
		AppFlag=1;
		}
		else{

		}

		break;
	default:
		result= FAILED;

	}



	/* Unlocking the Flash control register */
	HAL_FLASH_Unlock();

	/* looping on the data word by word to write it in the flash */
	while(numofWordsWritten < numofWords){

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartAddress, data[numofWordsWritten]) == HAL_OK)
		{
			StartAddress += 4;
			numofWordsWritten++;
		}
		else
		{
			/* Error occurred while writing data in Flash memory*/
			result= FAILED;
		}

	}

	/* Locking the Flash control register */
	HAL_FLASH_Lock();

	result= SUCCEED;
//	if (dataType == COM_DATA){
//		StartAddress=0;
//	}
//	else{
//
//	}

	return result;
}

uint8_t read_backup_reg(uint8_t reg) {
    return (uint8_t)HAL_RTCEx_BKUPRead(&hrtc, reg);
}

void write_backup_reg(uint8_t reg, uint32_t data) {
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&hrtc, reg, data);
    HAL_PWR_DisableBkUpAccess();

}

//
//uint8_t erase_inactive_bank(void){
//	static FLASH_EraseInitTypeDef EraseInitStruct;   /* Structure to erase the flash area */
//	uint32_t SECTORError;
//	uint32_t InactiveBank;
//
//	/*check the inactive bank to erase*/
//	if(READ_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk)){
//		/* active bank -> bank2
//		 * inactive bank -> bank1
//		 */
//		InactiveBank=FLASH_BANK_1;
//	}
//	else{
//		/* active bank -> bank1
//		 * inactive bank -> bank2
//		 */
//		InactiveBank=FLASH_BANK_2;
//	}
//
//	/* Filling the erasing structure */
//	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_MASSERASE;
//	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
//	EraseInitStruct.Banks=InactiveBank;
//
//
//	/* Unlocking the Flash control register */
//	HAL_FLASH_Unlock();
//
//	/* check if the erasing process is done correctly */
//	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
//	{
//		/*Error occurred while page erase*/
//		return FAILED;
//	}
//
//	/* Locking the Flash control register */
//	HAL_FLASH_Lock();
//
//	return SUCCEED;
//
//
//}
