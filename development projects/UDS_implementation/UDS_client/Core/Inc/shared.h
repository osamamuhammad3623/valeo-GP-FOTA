/*
 * shared.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Maria
 */

#ifndef INC_SHARED_H_
#define INC_SHARED_H_

#include "flash_memory.h" //needed for FLASH_DataType enum
#include "sys_arch.h"

#define CHUNK_SIZE 					20000

//semaphores
extern sys_sem_t ethernetSem;
extern sys_sem_t uartSem;

//data received
extern uint8_t data_received[CHUNK_SIZE];

#endif /* INC_SHARED_H_ */
