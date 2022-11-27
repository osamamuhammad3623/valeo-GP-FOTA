/*
 * shared_flags.h
 *
 *  Created on: Nov 26, 2022
 *      Author: osama
 */

#ifndef INC_SHARED_FLAGS_H_
#define INC_SHARED_FLAGS_H_
#include <stdint.h>


/**************************************
 * 			Definitions
**************************************/

// 0x081E.0000, is the start address of sector 23
/* address of flag: the bank that holds the latest stable image */
#define LATEST_IMAGE_BANK_ADDRESS		(0x081E0000)

/* address of flag: an indicator whether a new image needs to be activated or not */
#define NEW_IMAGE_ADDRESS 				(0x081E0004)

/* address of flag: bank where the new image is stored */
#define NEW_IMAGE_BANK_ADDRESS 			(0x081E0008)


typedef enum {
	BANK1=1, BANK2
}Flash_Memory_Bank;

typedef enum{
	OTA_No_New_Image=0, OTA_New_Image
}OTA_New_Image_Status;


/**************************************
 * 			Functions Prototypes
**************************************/
/* a function to validate address and value of the flags. If they are valid, write them */
uint8_t OTA_set_flag(uint32_t address, uint8_t value);

#endif /* INC_SHARED_FLAGS_H_ */
