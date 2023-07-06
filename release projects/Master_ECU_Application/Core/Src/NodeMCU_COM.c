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
int8_t counter = -1;
uint8_t fileType = COM_DATA;
uint8_t dataFlag = 0;
uint8_t downloadFinishedFlag = 0;
uint8_t targetToUpdate;
uint8_t percentage;
uint16_t chunkSize;
uint32_t downloadSize;
uint32_t file_CRC;

uint8_t data_received[ARRAY_SIZE] = {0};
uint8_t target_update[3] = {0};
uint8_t current_version_number[3];
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
			percentage = lcd_status_percentage(0);
			lcd_set_cursor(1, 7);
			lcd_write_int(percentage);
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
	new_version_number[0] = data_received[2];
	new_version_number[1] = data_received[3];
	new_version_number[2] = data_received[4];

	if(data_received[1] == Non_Urgent){
		//notify user about detection of new package
		lcd_clear();
		lcd_write_string_up("New Update");
		lcd_write_string_down("Version: ");
		lcd_display_version(new_version_number);
		HAL_Delay(LCD_PRINT_DELAY);

		//wait for response
		lcd_clear();
		lcd_write_string_up("Download Now?");
		lcd_write_string_down("  A:Yes   B:No  ");

		//if neg reponse{wait}
		//else {continue flow}

		while(1){
			if(Keys_Detect() == 'A'){
				break;
			}else if(Keys_Detect() == 'B'){
				lcd_clear();
				HAL_Delay(LCD_WAIT_DELAY);
				lcd_write_string_up("What about Now!");
				lcd_write_string_down("  A:Yes   B:No  ");
			}else{
				// Do nothing
			}
		}
	}
	else{
		//notify user about starting download
		lcd_clear();
		lcd_write_string_up("Urgent Update!!");
		lcd_write_string_down("Version: ");
		lcd_display_version(new_version_number);
		HAL_Delay(LCD_PRINT_DELAY);
	}

	target_update[0] = (data_received[5] & (1 << 0))>>0;
	target_update[1] = (data_received[5] & (1 << 1))>>1;
	target_update[2] = (data_received[5] & (1 << 2))>>2;

	lcd_clear();
	lcd_write_string_up("Downloading ...");
	lcd_set_cursor(1, 8);
	lcd_write_string("0 %");
	lcd_init_percentage_parameters(target_update);

	uint8_t downloadPackageFrame[] = {DOWNLOAD_PACKAGE, new_version_number[0], new_version_number[1], new_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);

	counter = -1;

	if(target_update[0]){
		// create task for erasing master inactive bank
		masterEraseMemory = sys_thread_new("masterEraseMemory_thread", masterEraseMemory_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal1);
	}

	lcd_set_cursor(1, 8);
	lcd_write_string("3");

	if (target_update[1] || target_update[2]) {
		// Increase priority of uds task to start connection
		osThreadSetPriority(UdsTaskHandle, osPriorityNormal2);
	}

	lcd_set_cursor(1, 7);
	lcd_write_string("15");

	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 1, HAL_MAX_DELAY);
}

void UART_getTargetUpdate(void)
{
	if(fileType == COM_DATA){
		counter = (counter==-1) ? 1 : (counter==1) ? 2 : (counter==2) ? 0 : 4;
		if(counter == 4){
			//seq finished
			lcd_set_cursor(1, 7);
			lcd_write_string("100");
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
			if(counter == 4){
				//seq finished
				lcd_set_cursor(1, 7);
				lcd_write_string("100");
				downloadFinishedFlag = 1;
				return;
			}
		}
	}

	uint8_t getTargetUpdateFrame[] = {GET_TARGET_UPDATE, targetToUpdate, fileType};
	HAL_UART_Transmit(&huart2, (uint8_t *)getTargetUpdateFrame, sizeof(getTargetUpdateFrame), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t *)data_received, 8, HAL_MAX_DELAY);

	if(targetToUpdate != 0){
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
		targetToUpdate == 1 ? osThreadResume(target1ThreadID) : osThreadResume(target2ThreadID);
		// Suspend UART task to get chunk size
		osThreadSuspend(UartTaskHandle);
	}
	else{
		chunkSize = ARRAY_SIZE;
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
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		break;

	case 1:
	case 2:
		// Resume target task to send uds transfer data
		targetToUpdate == 1 ? osThreadResume(target1ThreadID) : osThreadResume(target2ThreadID);
		osThreadSuspend(UartTaskHandle);
		break;

	default:
		break;
	}

	percentage = lcd_status_percentage(size);
	lcd_set_cursor(1, 7);
	lcd_write_int(percentage);

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
		if(targetToUpdate == 0){
			fileType = (fileType == COM_DATA) ? APP : COM_DATA;
		}

		if (targetToUpdate != 0) {
		// Resume target task to send uds request transfer exit
			targetToUpdate == 1 ? osThreadResume(target1ThreadID) : osThreadResume(target2ThreadID);
			osThreadSuspend(UartTaskHandle);
		}
		else{
			//calculate crc
		}

		//compare crc
		//in case of neg scenario we need to erase memory first!
//		if (file_CRC != target_calculated_CRC) {
//			if (fileType == APP) {
//				counter = (counter==1) ? -1 : (counter==2) ? 1 : 2;
//			}
//			fileType = (fileType == META_DATA) ? APP : META_DATA;	//not always correct.. in case of updating master!
//		}else{
//
//		}

		UART_getTargetUpdate();
	}
}

static void masterEraseMemory_thread(void *arg) {
	erase_inactive_bank();
	osThreadTerminate(masterEraseMemory);
}
