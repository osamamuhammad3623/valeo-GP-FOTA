/*
 * shared_flags.c
 *
 *  Created on: Nov 27, 2022
 *      Author: osama
 */
#include "shared_flags.h"
//#include "stm32f4xx_hal.h"

uint8_t OTA_set_flag(uint32_t address, uint8_t value){

	char is_valid = 0;
	if (address == LATEST_IMAGE_BANK_ADDRESS || NEW_IMAGE_BANK_ADDRESS){
		is_valid = ((value == BANK1) || (value == BANK2));

	}else if (address == NEW_IMAGE_ADDRESS){
		is_valid = ((value == OTA_No_New_Image) || (value == OTA_New_Image));
	}

	if (is_valid){
		*(uint8_t*)address=value;
		// 	HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, value);
	}

	/* read the value again to check it's been written */
	return (((*(uint8_t*)address) == value));
}
