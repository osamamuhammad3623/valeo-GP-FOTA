/*
 * bootloader.h
 *
 *  Created on: Dec 2, 2022
 *      Author: osama
 */

#include <stdint.h>

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

void bootloader_set_boot_bank(uint8_t bank);

uint8_t bootloader_get_active_bank(void);

#endif /* BOOTLOADER_H_ */
