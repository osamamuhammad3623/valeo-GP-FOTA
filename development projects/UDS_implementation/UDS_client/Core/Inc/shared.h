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
#include "lwip/sys.h"

#define ARRAY_SIZE 					20000U

//semaphores
extern sys_sem_t uartSem;
extern sys_sem_t udsSem1;
extern sys_sem_t udsSem2;

//data related variables
extern uint8_t dataFlag;
extern uint16_t chunkSize;
extern uint32_t downloadSize;
extern uint8_t data_received[ARRAY_SIZE];	//cannot be variable size!

#endif /* INC_SHARED_H_ */
