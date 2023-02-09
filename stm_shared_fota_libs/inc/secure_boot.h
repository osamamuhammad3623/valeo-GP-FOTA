/*
 * secure_boot.h
 *
 *  Created on: Feb 9, 2023
 *      Author: osama
 */

#include <stdint.h>

#ifndef SECURE_BOOT_H_
#define SECURE_BOOT_H_


/************** DEFINITIONS ********************/
#define 	VALID		1
#define		NOT_VALID	0


/************** Function Prototypes ********************/
/*
 * A function to ...
 * */
uint8 secure_boot_verify_application(uint32 start_addr);

#endif /* SECURE_BOOT_H_ */
