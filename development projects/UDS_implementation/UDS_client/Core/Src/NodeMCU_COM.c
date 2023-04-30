/*
 * NodeMCU_COM.c
 *
 *  Created on: Apr 24, 2023
 *      Author: Maria, Nada
 */

#include "NodeMCU_COM.h"
#include "usart.h"

/*******************************************************************************
 *                      Global Variables				*
 *******************************************************************************/
uint8_t dataFlag = 0;
uint8_t downloadFinishedFlag = 0;
uint8_t targetToUpdate;
uint32_t downloadSize;

uint8_t data_received[CHUNK_SIZE] = {0};
uint8_t target_update[NUM_OF_TARGETS] = {0};
uint8_t version_number[3] = {1, 1, 1};
uint8_t new_version_number[3];

/*******************************************************************************
 *                      Functions Implementations		*
 *******************************************************************************/
void UART_stateHandler(void){
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

void UART_packageDetection(void){
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

	target_update[0] = data_received[5] && (1 << 0);
	target_update[1] = data_received[5] && (1 << 1);
	target_update[2] = data_received[5] && (1 << 2);

	//signal semaphore to start connection -> tcp_init
	sys_sem_signal(&ethernetSem);

	if(target_update[0]){
		//erase mem
		//erase_inactive_bank();
	}

	uint8_t downloadPackageFrame[4] = {DOWNLOAD_PACKAGE, new_version_number[0], new_version_number[1], new_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 1, HAL_MAX_DELAY);
}

void UART_getTargetUpdate(void){
	static uint8_t fileType = META_DATA;
	static uint8_t counter = -1;

	if(fileType == META_DATA){
		counter = (counter==-1) ? 1 : (counter==1) ? 2 : (counter==2) ? 0 : 4;
		if(counter == 4){
			//seq finished
			downloadFinishedFlag = 1;
			return;
		}
		while(1){
			if(target_update[counter]){
				targetToUpdate = counter;
				break;
			}
			counter = (counter==2) ? 0 : counter+1;
		}
	}

	uint8_t getTargetUpdateFrame[4] = {GET_TARGET_UPDATE, targetToUpdate, fileType, PADDING};
	HAL_UART_Transmit(&huart2, (uint8_t *)getTargetUpdateFrame, sizeof(getTargetUpdateFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 4, HAL_MAX_DELAY);

	fileType = (fileType == META_DATA) ? APP : META_DATA;
}

void UART_downloadFailed(void){
	//Download again or wait!
	uint8_t downloadPackageFrame[4] = {DOWNLOAD_PACKAGE, new_version_number[0], new_version_number[1], new_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 1, HAL_MAX_DELAY);
}

void UART_getDownloadSize(void){
	downloadSize = (((uint32_t)data_received[1])<<2*8 |((uint32_t)data_received[2])<<8 |((uint32_t)data_received[3]));

	//uds request download

	uint8_t startSendingFrame[4] = {START_SENDING, PADDING, PADDING, PADDING};
	HAL_UART_Transmit(&huart2, (uint8_t *)startSendingFrame, sizeof(startSendingFrame), HAL_MAX_DELAY);

	uint16_t chunkSize = (downloadSize > CHUNK_SIZE) ? CHUNK_SIZE : downloadSize;
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, chunkSize, HAL_MAX_DELAY);
	dataFlag = 1;
}

void UART_handleData(void){
	//forward data to target or flash here
	switch(targetToUpdate){
	case 0:
		//flash
		break;

	case 1:
	case 2:
		//uds transfer data
		break;

	default:
		break;
	}

	//ok or not ok!
	uint8_t okFrame[4] = {OK, OK, OK, OK};
	HAL_UART_Transmit(&huart2, (uint8_t *)okFrame, sizeof(okFrame), HAL_MAX_DELAY);

	if(downloadSize > CHUNK_SIZE){
		downloadSize -= CHUNK_SIZE;
		uint16_t chunkSize = (downloadSize > CHUNK_SIZE) ? CHUNK_SIZE : downloadSize;
		HAL_UART_Receive(&huart2, (uint8_t *)data_received, chunkSize, HAL_MAX_DELAY);
	}
	else{
		dataFlag = 0;
		UART_getTargetUpdate();
	}
}