/*
 * bootloader.h
 *
 *  Created on: Dec 2, 2022
 *      Author: osama
 */

#include <stdint.h>

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

/*
 * A function to set the boot-bank for the next reset by changing BFB2 bit in Option Bytes
 * - to set bank 1, reset the bit
 * - to set bank 2, set the bit
 * */
void static bootloader_set_boot_bank(uint8_t bank);


/*
 * A function to set the inactive bank to be the next boot-bank
 * if bank 1 is the active one, set bank 2, and vice versa
 * */
void bootloader_switch_to_inactive_bank(void);


/*
 * A function to make a software reset
 * */
void bootloader_reboot(void);


/*
 * A function to jump to an application
 * */
void bootloader_jump_to_application(uint32 start_addr);

#endif /* BOOTLOADER_H_ */
