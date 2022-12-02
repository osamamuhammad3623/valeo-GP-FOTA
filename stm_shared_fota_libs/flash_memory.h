/*
 * flash_memory.h
 *
 *  Created on: Dec 2, 2022
 *      Author: osama
 */
#include <stdint.h>

#ifndef FLASH_MEMORY_H_
#define FLASH_MEMORY_H_

/*
 * A function to erase a bank
 * - Input: bank number: 1 or 2
 * - Return: 1 if bank is erased successfully, 0 otherwise
 * */
uint8_t flash_erase_bank(uint8_t);

#endif /* FLASH_MEMORY_H_ */
