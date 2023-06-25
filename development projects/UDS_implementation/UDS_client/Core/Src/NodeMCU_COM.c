/*
 * NodeMCU_COM.c
 *
 *  Created on: Apr 24, 2023
 *      Author: Maria, Nada, Maram
 */

#include "NodeMCU_COM.h"
#include "usart.h"

/*******************************************************************************
 *                      Global Variables				*
 *******************************************************************************/
uint8_t fileType = COM_DATA;
int8_t counter = -1;
uint8_t dataFlag = 0;
uint8_t downloadFinishedFlag = 0;
uint8_t targetToUpdate;
uint16_t chunkSize;
uint32_t downloadSize;
uint32_t file_CRC;

uint8_t data_received[ARRAY_SIZE] = {0};
uint8_t target_update[NUM_OF_TARGETS] = {0};
uint8_t current_version_number[3] = {1, 1, 1};
uint8_t new_version_number[3];

osThreadId_t masterEraseMemory;
/*******************************************************************************
 *                      Functions Implementations		*
 *******************************************************************************/
void UART_stateHandler(void)
{
	if(!dataFlag){
		switch(data_received[0]){
		case NEW_PACKAGE:
			UART_packageDetection();
			break;

		case PACKAGE_DOWNLOADED:
			UART_getTargetUpdate();
			break;

		case DOWNLOADING_ERROR:
			UART_downloadFailed();
			break;

		case FILE_SIZE:
			UART_getDownloadSize();
			break;

		default:
			break;
		}
	}
	else{
		UART_handleData();
	}
}

void UART_packageDetection(void)
{
	if(data_received[1] == Non_Urgent){
		//notify user about detection of new package
		//wait for response
		//if neg reponse{wait}
		//else {continue flow}
	}
	else{
		//notify user about starting download
	}

	new_version_number[0] = data_received[2];
	new_version_number[1] = data_received[3];
	new_version_number[2] = data_received[4];

	target_update[0] = (data_received[5] & (1 << 0))>>0;
	target_update[1] = (data_received[5] & (1 << 1))>>1;
	target_update[2] = (data_received[5] & (1 << 2))>>2;

	uint8_t downloadPackageFrame[] = {DOWNLOAD_PACKAGE, new_version_number[0], new_version_number[1], new_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);

	counter = -1;

	if(target_update[0]){
		// create task for erasing master inactive bank
		masterEraseMemory = sys_thread_new("masterEraseMemory_thread", masterEraseMemory_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal1);
	}

	if (target_update[1] || target_update[2]) {
		// Increase priority of uds task to start connection
		osThreadSetPriority(UdsTaskHandle, osPriorityNormal2);
	}

	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 1, HAL_MAX_DELAY); //HAL_MAX_DELAY
}

void UART_getTargetUpdate(void)
{
	if(fileType == COM_DATA){
		counter = (counter==-1) ? 1 : (counter==1) ? 2 : (counter==2) ? 0 : 4;
		if(counter == 4){
			//seq finished
			downloadFinishedFlag = 1;
			return;
		}
		while(1){
			if(!target_update[counter]){
				counter = (counter==1) ? 2 : (counter==2) ? 0 : 4;
			} else {
				targetToUpdate = counter;
				break;
			}
//			if(target_update[counter]){
//				targetToUpdate = counter;
//				break;
//			}
//			counter = (counter==1) ? 2 : (counter==2) ? 0 : 4;
			if(counter == 4){
				//seq finished
				downloadFinishedFlag = 1;
				return;
			}
		}
	}

	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

	uint8_t getTargetUpdateFrame[] = {GET_TARGET_UPDATE, targetToUpdate, fileType};
	HAL_UART_Transmit(&huart2, (uint8_t *)getTargetUpdateFrame, sizeof(getTargetUpdateFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 8, HAL_MAX_DELAY);


	// flash in fota master
	if (targetToUpdate != 0) { //////////
		fileType = (fileType == COM_DATA) ? APP : COM_DATA;
	}
}

void UART_downloadFailed(void)
{
	//Download again or wait!
	uint8_t downloadPackageFrame[] = {DOWNLOAD_PACKAGE, new_version_number[0], new_version_number[1], new_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 1, HAL_MAX_DELAY);
}

void UART_getDownloadSize(void)
{
	downloadSize = (((uint32_t)data_received[1])<<2*8 |((uint32_t)data_received[2])<<8 |((uint32_t)data_received[3]));
	file_CRC = (((uint32_t)data_received[4])<<3*8 |((uint32_t)data_received[5])<<2*8 |((uint32_t)data_received[6])<<8 |((uint32_t)data_received[7]));

	if(targetToUpdate != 0){
		// Resume target task to send uds request download
		osThreadResume(target1ThreadID);
		// Suspend UART task to get chunk size
		osThreadSuspend(UartTaskHandle);
	}
	else{
		chunkSize = ARRAY_SIZE;	//can be changed
	}

	uint8_t chunk[] = {((uint32_t)chunkSize&0x0000FF00)>>(8), chunkSize&0x000000FF};
	uint8_t startSendingFrame[] = {START_SENDING, chunk[0], chunk[1]};
	HAL_UART_Transmit(&huart2, (uint8_t *)startSendingFrame, sizeof(startSendingFrame), HAL_MAX_DELAY);

	uint16_t size = (downloadSize > chunkSize) ? chunkSize : downloadSize;
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, size, HAL_MAX_DELAY);
	dataFlag = 1;
}

void UART_handleData(void)
{
	uint8_t errorState;
	uint16_t size = (downloadSize > chunkSize) ? chunkSize : downloadSize;
	//forward data to target or flash here
	switch(targetToUpdate){
	case 0:
		//flash
		errorState = flash_memory_write((uint32_t *)data_received, (uint32_t)size/4, fileType); // APP
		break;

	case 1:
	case 2:
		// Resume target task to send uds transfer data
		osThreadResume(target1ThreadID);
		osThreadSuspend(UartTaskHandle);
		break;

	default:
		break;
	}

	if(downloadSize > chunkSize){
		//ok or not ok!
		uint8_t okFrame[] = {OK};
		downloadSize -= chunkSize;
		uint16_t size = (downloadSize > chunkSize) ? chunkSize : downloadSize;
		HAL_UART_Transmit(&huart2, (uint8_t *)okFrame, sizeof(okFrame), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t *)data_received, size, HAL_MAX_DELAY);
	}
	else{
		dataFlag = 0;
		if (targetToUpdate == 0) {
			fileType = (fileType == COM_DATA) ? APP : COM_DATA; ////////
		}

		osDelay(1);
		if (targetToUpdate != 0) {
		// Resume target task to send uds request transfer exit
			osThreadResume(target1ThreadID);
			osThreadSuspend(UartTaskHandle);
		}

//		if (file_CRC != target_calculated_CRC) {
//			if (fileType == APP) {
//				counter = (counter==1) ? -1 : (counter==2) ? 1 : 2;
//			}
//			fileType = (fileType == META_DATA) ? APP : META_DATA;
//		}
		UART_getTargetUpdate();
	}
}

static void masterEraseMemory_thread(void *arg) {
	erase_inactive_bank();
	osThreadTerminate(masterEraseMemory);
}
