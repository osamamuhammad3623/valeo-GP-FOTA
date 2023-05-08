/******************************************************************************
  * @file    boot_manager.h
  * @author  BOOT MANAGER Team
  * @brief   Header file for boot manager Application Program.
 *******************************************************************************/

#ifndef BOOT_MANAGER_H_
#define BOOT_MANAGER_H_

/********************************** Includes **********************************/
#include "stdint.h"
#include "boot_manager_confg.h"

/******************************************************************************/
/******************************************************************************/
/*************************** Functions Prototypes *****************************/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* \Description:
 * function to implement secure booting
 * steps:
 * 1- Fetch the meta data and certificates from memory
 * 2- checking for root index revocation
 * 3- verify the two certificates, root certificate and secure boot certificates
 * 4- extract the root certificate public key
 * 4- HW Anchor verification
 * 5- extract the secure boot public key in RSA key format
 * 6- verify signature
 * 7- Anti roll back verification
 * 8- calculate the hash of the image and compare it with the image digest
 *                                                                            */
/******************************************************************************/
uint8_t secure_boot_verify(uint8_t Application_selector);

/******************************************************************************/
/* \Description:
 * function to jump to application at specific address                        */
/******************************************************************************/
void jump_to_application(uint32_t start_addr);


uint8_t read_backup_reg(uint8_t reg);

void write_backup_reg(uint8_t reg, uint32_t data) ;
void swap_to_inactive_bank(void);

void bootloader_switch_to_inactive_bank(void);

void bootloader_reboot(void);

#endif /* BOOT_MANAGER_H_ */

