/*
 * OTA_Update.c
 *
 *  Created on: Nov 17, 2022
 *      Author: osama
 */

#include "OTA_Update.h"

//static uint8_t LATEST_IMAGE_BANK __attribute__((section(".ARM.__at_0x08004000"))) = Bank1;

static uint8_t IS_NEW_IMAGE __attribute__((section(".ARM.__at_0x08004004"))) = OTA_No_Update;

static uint8_t NEW_IMAGE_BANK __attribute__((section(".ARM.__at_0x08004008"))) = Bank2;


void OTA_set_latest_image_bank(Memory_Bank bank){
	if ((bank == Bank1) || (bank==Bank2)){
		*(uint8_t*)(LATEST_IMAGE_BANK_ADDRESS)=bank;
		//LATEST_IMAGE_BANK = bank;
	}
}

Memory_Bank OTA_latest_image_bank(void){
	return (*(uint8_t*)(LATEST_IMAGE_BANK_ADDRESS));
}

void OTA_set_is_new_image(OTA_Update_Status flag){
	if((flag==OTA_No_Update) || (flag==OTA_Update)){
		IS_NEW_IMAGE = flag;
	}
}

OTA_Update_Status OTA_is_new_image(void){
	return IS_NEW_IMAGE;
}

void OTA_set_new_image_bank(Memory_Bank bank){
	if ((bank == Bank1) || (bank==Bank2)){
		NEW_IMAGE_BANK = bank;
	}
}

Memory_Bank OTA_new_image_bank(void){
	return NEW_IMAGE_BANK;
}
