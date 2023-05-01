/******************************************************************************
 * 
 * Module: UDS_Server 
 * File Name: uds_server.c 
 * Description: Source file for the UDS server driver 
 * Author: Maram Nabil, Maria Osama 
 * 
 *******************************************************************************/
#include "uds_server.h" 
#include "main.h"
#include "flash_memory.h"
#include "bootloader.h"
#include "tcp_server.h"
#include "cmsis_os.h"

//initialize struct… init fn! And deinit if needed 
UDS_Security_Access currentAccessState = ACCESS_DENIED;
UDS_Session currentSession = DEFAULT; 

/******************************************************************************* 
 *                      Global Variables				* 
 *******************************************************************************/
uint8_t generatedKey[32];
uint8_t seedRequestedFlag = 0;
extern CRC_HandleTypeDef hcrc; // (to be declared in main.c)
//uint32_t downloadSize;
uint32_t CRC_result; 
extern RNG_HandleTypeDef hrng;
extern HASH_HandleTypeDef hhash;
extern uint32_t downloadSize;

uint8_t recvd_msg[3000]; ///////////////
int msgSize = 0;
/******************************************************************************* 
 *                      Functions Implementations		*
 *******************************************************************************/
void UDS_execute_request(void *arg) 
{
	if(arg == NULL)
	{
		return;
	}

	uint8_t *requestFrame = (uint8_t *)arg;

	switch(requestFrame[0]){

	case DIAGNOSTICS_SESSION_CONTROL: UDS_change_session(requestFrame);
	break;

	case SECURITY_ACCESS:		 	UDS_security_access(requestFrame);
	break;

	case ROUTINE_CONTROL: 		UDS_call_routine(requestFrame);
	break;

	case REQUEST_DOWNLOAD:		UDS_start_download(requestFrame);
	break;

	case TRANSFER_DATA: 			UDS_process_data(requestFrame);
	break;

	case REQUEST_TRANSFER_EXIT:		 UDS_exit_download(requestFrame);
	break;

	case ECU_RESET:		 	UDS_reboot(requestFrame);
	break;

	default:
		;
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, requestFrame[0], SERVICE_NOT_SUPPORTED};
		tcp_SendResponse(responseFrame, 3);
		break;
	}
} 

void UDS_change_session(uint8_t *requestFrame) 
{
	if(requestFrame[1] < MAX_SESSIONS)
	{
		currentSession = requestFrame[1];
		uint8_t responseFrame[] = {DIAGNOSTICS_SESSION_CONTROL + POSITIVE_RESPONSE_OFFSET, requestFrame[1]};
		tcp_SendResponse(responseFrame, 2);
	}
	else
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, DIAGNOSTICS_SESSION_CONTROL, SUBFUNCTION_NOT_SUPPORTED};
		tcp_SendResponse(responseFrame, 3);
	}
} 

void UDS_security_access(uint8_t *requestFrame) 
{ 
	if(currentSession != EXTENDED)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, SECURITY_ACCESS, CONDITIONS_NOT_CORRECT};
		tcp_SendResponse(responseFrame, 3);
		return;
	}

	switch(requestFrame[1]){

	case SA_REQUEST_SEED: 	UDS_generate_seed(requestFrame);
	break;

	case SA_SEND_KEY:		 UDS_verify_key(requestFrame);
	break;

	default:
		;
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, SECURITY_ACCESS, SUBFUNCTION_NOT_SUPPORTED};
		tcp_SendResponse(responseFrame, 3);
		break;
	}
} 

void UDS_generate_seed(uint8_t *requestFrame) 
{
	//use RNG for seed (truly random while rand lib is pseudo random) and HMAC for key and save it in global var! … adjust configurations of the project correctly first.
	seedRequestedFlag = 1;
	uint32_t seed;
	HAL_RNG_GenerateRandomNumber(&hrng, &seed);	//pooling takes 40 clk cycles
	// interrupt fn: HAL_RNG_GenerateRandomNumber_IT(&hrng);  à RTOS use Queue!
	//needs imp for ISR: HAL_RNG_ReadyDataCallback() to set a flag for example and then read the value by calling HAL_RNG_ReadLastRandomNumber()

	uint8_t seedArr[] = {((uint32_t)seed&(0xFF000000))>>(8*3), ((uint32_t)seed&(0x00FF0000))>>(8*2), ((uint32_t)seed&(0x0000FF00))>>8, seed&(0x000000FF)};
	HAL_StatusTypeDef errorState = HAL_HMACEx_SHA256_Start(&hhash, seedArr, 4, generatedKey, TIMEOUT);
	//HAL_HASH_DeInit(&hhash);
	uint8_t responseFrame[] = {SECURITY_ACCESS + POSITIVE_RESPONSE_OFFSET, requestFrame[1], seedArr[0], seedArr[1], seedArr[2], seedArr[3]};
	tcp_SendResponse(responseFrame, 6);
}

void UDS_verify_key(uint8_t *requestFrame) 
{
	static uint8_t attemptsCount = 0;
	uint8_t receivedKey[32];
	for(int i=0; i<32;i++)
	{
		receivedKey[i] = requestFrame[i+2];
	}

	if(seedRequestedFlag == 0)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, SECURITY_ACCESS, REQUEST_SEQUENCE_ERROR};
		tcp_SendResponse(responseFrame, 3);
	}
	else if(memcmp(generatedKey, receivedKey, 32) == 0)
	{
		attemptsCount = 0;
		seedRequestedFlag = 0;
		uint8_t responseFrame[] = {SECURITY_ACCESS + POSITIVE_RESPONSE_OFFSET, requestFrame[1]};
		tcp_SendResponse(responseFrame, 2);
	}
	else
	{
		attemptsCount ++;
		if(attemptsCount >= MAX_NUMBER_OF_ATTEMPTS)
		{
			uint8_t responseFrame[] = {NEGATIVE_RESPONSE, SECURITY_ACCESS, EXCEEDED_NUMBER_OF_ATTEMPTS};
			tcp_SendResponse(responseFrame, 3);
		}
		else
		{
			uint8_t responseFrame[] = {NEGATIVE_RESPONSE, SECURITY_ACCESS, INVALID_KEY};
			tcp_SendResponse(responseFrame, 3);
		}
	}
}


void UDS_call_routine(uint8_t *requestFrame) 
{ 
	/*if(currentAccessState != ACCESS_GRANTED)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ROUTINE_CONTROL, SECURITY_ACCESS_DENIED};
		tcp_SendResponse(responseFrame, 3);
		return;
	}*/

	//parse payload and jump to the appropriate function
	if(requestFrame[1] == RC_START_ROUTINE)
	{
		if(requestFrame[2] == 0xFF && requestFrame[3] == 0x00)
		{
			UDS_erase_memory_routine(requestFrame);
		}
		else if(requestFrame[2] == 0x02 && requestFrame[3] == 0x02)
		{
			UDS_check_memory_routine(requestFrame);
		}
		else
		{
			uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ROUTINE_CONTROL, REQUEST_OUT_OF_RANGE};
			tcp_SendResponse(responseFrame, 3);
		}
	}
	else
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ROUTINE_CONTROL, SUBFUNCTION_NOT_SUPPORTED};
		tcp_SendResponse(responseFrame, 3);
	}
} 

void UDS_erase_memory_routine(uint8_t *requestFrame) 
{ 
	// erase inactive bank, and check status
	uint8_t errorState = erase_inactive_bank();

	// if error, send neg response
	if (errorState != SUCCEED) {
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ROUTINE_CONTROL, CONDITIONS_NOT_CORRECT};
		tcp_SendResponse(responseFrame, 3);
	}
	//else, send positive response
	else {

		/* Blink LED to indicate memory is erased */
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

		uint8_t responseFrame[] = {ROUTINE_CONTROL + POSITIVE_RESPONSE_OFFSET, requestFrame[1], requestFrame[2], requestFrame[3]};
		tcp_SendResponse(responseFrame, 4);
	}
} 

void UDS_check_memory_routine(uint8_t *requestFrame) 
{ 

} 

void UDS_start_download(uint8_t *requestFrame) 
{
	// check access
//	if (currentAccessState != ACCESS_GRANTED)
//	{
//		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, REQUEST_DOWNLOAD, SECURITY_ACCESS_DENIED};
//		tcp_SendResponse(responseFrame, 3);
//		return;
//	}
	// save image size from frame
	downloadSize = (requestFrame[2]<<(8*2)) | (requestFrame[1]<<8) | requestFrame[0];

	// send response
	uint16_t packetSize = CHUNK_SIZE;
	uint8_t packetSizeBytes[] = {((uint16_t)packetSize&(0xFF00))>>8, packetSize&(0x00FF)};

	uint8_t responseFrame[] = {REQUEST_DOWNLOAD + POSITIVE_RESPONSE_OFFSET, packetSizeBytes[0], packetSizeBytes[1]};
	tcp_SendResponse(responseFrame, 3);
} 

void UDS_process_data(uint8_t *requestFrame) 
{
	/*
	if (currentAccessState != ACCESS_GRANTED) {
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, TRANSFER_DATA, SECURITY_ACCESS_DENIED};
		tcp_SendResponse(responseFrame, 3);
		return;
	}
	uint8_t errorState;
	int dataSizeInWords;
	uint8_t *dataBytes = &requestFrame[1];
	uint32_t *dataWords;
	// decrement frame size from image size, and converting data array from bytes to words
	if (downloadSize >= CHUNK_SIZE) {
		downloadSize -= CHUNK_SIZE;
		dataSizeInWords = bytesToWords(dataBytes, CHUNK_SIZE, dataWords);
	} else {
		dataSizeInWords = bytesToWords(dataBytes, downloadSize, dataWords);
	}

	// flash
	errorState = flash_memory_write(dataWords, dataSizeInWords, APP); // always APP just for testing
	if (errorState == FAILED)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, TRANSFER_DATA, CONDITIONS_NOT_CORRECT};
		tcp_SendResponse(responseFrame, 3);
		return;
	}
	// calculate CRC, to check: reset at the start of each file ?
	CRC_result = HAL_CRC_Accumulate(&hcrc, dataWords, (uint32_t) dataSizeInWords);
*/
	/*--------------------------------- FOR TESTING ------------------------------------------*/
	uint8_t errorState;
	int dataSizeInWords;
	uint8_t *dataBytes = &requestFrame[1];
	uint32_t *dataWords;
	// decrement frame size from image size, and converting data array from bytes to words
//	if (downloadSize >= CHUNK_SIZE) {
//		downloadSize -= CHUNK_SIZE;
//		//dataSizeInWords = bytesToWords(dataBytes, CHUNK_SIZE, dataWords);
//		errorState = flash_memory_write(dataBytes, CHUNK_SIZE/4, APP); // always APP just for testing
//
//	} else {
//		dataSizeInWords = bytesToWords(dataBytes, downloadSize, dataWords);
//		errorState = flash_memory_write(dataWords, dataSizeInWords, APP); // always APP just for testing
//
//		//To DO
//		//last chunk flag
//	}
	/*--------------- FOR TESTING -------------------*/
	if (downloadSize >= CHUNK_SIZE) {
		errorState = flash_memory_write(dataBytes, CHUNK_SIZE/4, APP); // always APP just for testing
	} else {
//		dataSizeInWords = bytesToWords(dataBytes, downloadSize, dataWords);
//		errorState = flash_memory_write(dataWords, dataSizeInWords, APP); // always APP just for testing
		errorState = flash_memory_write(dataBytes, downloadSize/4, APP); // always APP just for testing
	}
	/*-----------------------------------------------*/


	// flash
	//errorState = flash_memory_write(dataWords, dataSizeInWords, APP); // always APP just for testing
	if (errorState == FAILED)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, TRANSFER_DATA, CONDITIONS_NOT_CORRECT};
		tcp_SendResponse(responseFrame, 3);
		return;
	}
	// calculate CRC, to check: reset at the start of each file ?
//	CRC_result = HAL_CRC_Accumulate(&hcrc, dataWords, (uint32_t) dataSizeInWords);

	//	tcp_receiveChunk(CHUNK_SIZE);
	// flash
	//erase_inactive_bank();
	//uint8_t errorState = flash_memory_write((uint32_t *)((uint8_t *)requestFrame+1), CHUNK_SIZE/4, APP); //1604 // always APP just for testing
	//recvd_msg[msgSize] =

	/* Blink LED to indicate memory is flashed */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	// send response
	uint8_t responseFrame[] = {TRANSFER_DATA + POSITIVE_RESPONSE_OFFSET};
	tcp_SendResponse(responseFrame, 1);
	/*-----------------------------------------------------------------------------------------*/

////	tcp_receiveChunk(CHUNK_SIZE);
//	// flash
//	//erase_inactive_bank();
//	uint8_t errorState = flash_memory_write((uint32_t *)((uint8_t *)requestFrame+1), CHUNK_SIZE/4, APP); //1604 // always APP just for testing
//	//recvd_msg[msgSize] =
//
//	/* Blink LED to indicate memory is flashed */
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	// send response
//	uint8_t responseFrame[] = {TRANSFER_DATA + POSITIVE_RESPONSE_OFFSET};
//	tcp_SendResponse(responseFrame, 1);
} 


void UDS_exit_download(uint8_t *requestFrame) 
{ 
	// check access
	if (currentAccessState != ACCESS_GRANTED) {
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, REQUEST_TRANSFER_EXIT, SECURITY_ACCESS_DENIED};
		tcp_SendResponse(responseFrame, 3);
		return;
	}

	// send response with final CRC
	uint8_t crcResult[] = {((uint32_t)CRC_result&(0xFF000000))>>(8*3), ((uint32_t)CRC_result&(0x00FF0000))>>(8*2), ((uint32_t)CRC_result&(0x0000FF00))>>8, CRC_result&(0x000000FF)};
	uint8_t responseFrame[] = {REQUEST_TRANSFER_EXIT + POSITIVE_RESPONSE_OFFSET, crcResult[0], crcResult[1], crcResult[2], crcResult[3]};
	tcp_SendResponse(responseFrame, 5);
}

void UDS_reboot(uint8_t *requestFrame) 
{ 
	// check access
	if (currentAccessState != ACCESS_GRANTED)
	{
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ECU_RESET, SECURITY_ACCESS_DENIED};
		tcp_SendResponse(responseFrame, 3);
		return;
	}

	// check type of reset, if not soft reset, send neg resp. (sub function not supported)
	if (requestFrame[1] != ER_SOFT_RESET) {
		uint8_t responseFrame[] = {NEGATIVE_RESPONSE, ECU_RESET, SUBFUNCTION_NOT_SUPPORTED};
		tcp_SendResponse(responseFrame, 3);
		return;
	}

	uint8_t responseFrame[] = {ECU_RESET + POSITIVE_RESPONSE_OFFSET, ER_SOFT_RESET};
	tcp_SendResponse(responseFrame, 2);

	// reboot
	bootloader_switch_to_inactive_bank();
	bootloader_reboot();
}

int bytesToWords(uint8_t* dataBytes, uint32_t dataSizeInBytes, uint32_t * dataWords) {
	int i;
	int j;
	int dataSizeInWords;
	if (dataSizeInBytes%4 != 0) {
		dataSizeInWords = (int)((double)dataSizeInBytes/4 + 1);
	} else {
		dataSizeInWords = dataSizeInBytes/4;
	}

	int byteNum = 0;
	for (i = 0; i < dataSizeInWords; i++) {
		for (j = 0; j < 4 && byteNum < dataSizeInBytes; j++) {
			dataWords[i] |= dataBytes[byteNum] << 8*(3-j);
			byteNum++;
		}
	}
	// check if size in bytes is not divisible by 4, then pad with ones
	if (dataSizeInBytes%4 != 0) {
		padWithOnes(dataSizeInBytes, dataWords, dataSizeInWords);
	}
	return dataSizeInWords;
}

void padWithOnes(uint32_t dataSizeInBytes, uint32_t * dataWords, uint32_t dataSizeInWords) {
	int paddedBytesNum = 4 - (dataSizeInBytes%4);
	int i;
	for (i = 0; i < paddedBytesNum; i++) {
		dataWords[dataSizeInWords-1] |= 0xFF<<(8*i);
	}
}


