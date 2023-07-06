/*
 * shared.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Maria
 */

#ifndef INC_SHARED_H_
#define INC_SHARED_H_

#include "flash_memory.h"
#include "lwip/sys.h"
#include "lcd.h"
#include "KEYPAD.h"

#define ARRAY_SIZE 					5000U

// os handles
extern osThreadId_t UartTaskHandle;
extern osThreadId_t UdsTaskHandle;
extern osThreadId_t target1ThreadID;
extern osThreadId_t target2ThreadID;
extern osThreadId_t InstallTaskHandle;
extern osMutexId_t targetsMutexHandle;

//data related variables
extern uint8_t dataFlag;
extern uint8_t installationReadyFlag;
extern uint8_t target1InstalledFlag;
extern uint8_t target2InstalledFlag;
extern uint8_t target1_version_received;
extern uint8_t target2_version_received;
extern uint8_t target1_version_number[3];
extern uint8_t target2_version_number[3];
extern uint8_t data_received[ARRAY_SIZE];
extern uint8_t new_version_number[3];
extern uint8_t target_update[3];
extern uint16_t chunkSize;
extern uint32_t downloadSize;
extern uint32_t target_calculated_CRC;

#endif /* INC_SHARED_H_ */
