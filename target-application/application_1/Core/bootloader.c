/*
 * bootloader.c
 *
 *  Created on: Dec 2, 2022
 *      Author: osama
 */

#include "stm32f4xx_hal.h"
#include "bootloader.h"

void bootloader_set_boot_bank(uint8_t bank){

	if ((bank != 1) && (bank!=2)){
		return;
	}

	// clear OPTLOCK bit to enable modifying the option bytes
	HAL_FLASH_OB_Unlock();

	// Check that no Flash memory operation is ongoing
	while(FLASH->SR & FLASH_SR_BSY_Msk);

	// if it's required to set bank 1, then clear BFB2 bit
	// and if it's required to set bank 2, set the BFB2 bit
	if (bank==1){
		/* doesn't work! */
		CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk);
	}else{
		SET_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk);
	}

	// start changing user option bytes
	HAL_FLASH_OB_Launch();

	// Wait for the operation to be completed
	while(FLASH->SR & FLASH_SR_BSY_Msk);

	// unlock option-bytes-bit
	HAL_FLASH_OB_Lock();
}


uint8_t bootloader_get_active_bank(void){

}
