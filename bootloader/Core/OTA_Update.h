/*
 * OTA_Update.h
 *
 *  Created on: Nov 17, 2022
 *      Author: osama
 */

#ifndef OTA_UPDATE_H_
#define OTA_UPDATE_H_

#include <stdint.h>

/**************************************
 * 			Definitions
 **************************************/
#define LATEST_IMAGE_BANK_ADDRESS		(0x08004000)
#define NEW_IMAGE_ADDRESS 				(0x08004004)
#define NEW_IMAGE_BANK_ADDRESS 			(0x08004008)

typedef enum {
	Bank1=1, Bank2
}Memory_Bank;

typedef enum{
	OTA_No_Update=0, OTA_Update
}OTA_Update_Status;

/**************************************
 * 			Functions Prototypes
 **************************************/

void OTA_set_latest_image_bank(Memory_Bank bank);
Memory_Bank OTA_latest_image_bank(void);

void OTA_set_is_new_image(OTA_Update_Status flag);
OTA_Update_Status OTA_is_new_image(void);

void OTA_set_new_image_bank(Memory_Bank bank);
Memory_Bank OTA_new_image_bank(void);

#endif /* OTA_UPDATE_H_ */
