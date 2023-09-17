/****************************************************************************** 
 * 
 * Module: UDS_Client 
 * File Name: uds_client.c 
 * Description: Source file for the UDS client driver 
 * Author: Maria Osama, Maram Nabil 
 * 
 *******************************************************************************/ 
#include "uds_client.h"

uint8_t target1InstalledFlag = 0;
uint8_t target2InstalledFlag = 0;
uint8_t target1_version_received = 0;
uint8_t target2_version_received = 0;
uint8_t target1_version_number[3];
uint8_t target2_version_number[3];
uint8_t transferDataFrame[ARRAY_SIZE + 1];
uint32_t target_calculated_CRC;

/*******************************************************************************
 *                      Functions Implementations		* 
 *******************************************************************************/ 
void UDS_init(uint8_t *targetToConnectWith){
	init_uds_request_callback(UDS_start_request);
	init_uds_recv_resp_clbk(UDS_receive_response);
	tcpclient_init(targetToConnectWith);
}

void UDS_start_request(TargetECU targetECU) {
	if (!(target1InstalledFlag || target2InstalledFlag)) {
		uint8_t sessionType = (uint8_t) EXTENDED;
		UDS_diagnostics_session_control(targetECU, sessionType);
	} else {
		UDS_get_version_number(targetECU);
	}
}

void UDS_get_version_number(TargetECU targetECU) {
	uint8_t requestFrame[] = {READ_DATA_BY_IDENTIFIER, 0xF1, 0x95};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_diagnostics_session_control(TargetECU targetECU, uint8_t session)
{
	uint8_t requestFrame[] = {DIAGNOSTICS_SESSION_CONTROL, session};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_SA_request_seed(TargetECU targetECU)
{
	uint8_t requestFrame[] = {SECURITY_ACCESS, SA_REQUEST_SEED};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_SA_send_key(TargetECU targetECU, uint8_t *seed)
{ 
	uint8_t generatedKey[32];
	HAL_HMACEx_SHA256_Start(&hhash, seed, 4, generatedKey, TIMEOUT);
	uint8_t requestFrame[34] = {SECURITY_ACCESS, SA_SEND_KEY};
	for(int i=0; i<32; i++)
	{
		requestFrame[i+2] = generatedKey[i];
	}
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_RC_erase_memory(TargetECU targetECU)
{
	uint8_t requestFrame[] = {ROUTINE_CONTROL, RC_START_ROUTINE, 0xFF, 0x00};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_RC_check_memory(TargetECU targetECU)
{
	uint8_t requestFrame[] = {ROUTINE_CONTROL, RC_START_ROUTINE, 0x02, 0x02};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_request_download(TargetECU targetECU, uint32_t fileSize)
{
	uint8_t downloadSize[] = {((uint32_t)fileSize&0x00FF0000)>>(16),((uint32_t)fileSize&0x0000FF00)>>(8), fileSize&0x000000FF};
	uint8_t requestFrame[] = {REQUEST_DOWNLOAD, downloadSize[0], downloadSize[1], downloadSize[2]};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_transfer_data(TargetECU targetECU)
{
	//use the array used by uart to save data and forward it
	transferDataFrame[0] = TRANSFER_DATA;
	uint16_t size = (downloadSize > chunkSize) ? chunkSize : downloadSize;
	for (uint16_t i = 0; i < size; i++) {
		transferDataFrame[i+1] = data_received[i];
	}
	tcp_SendMessage(targetECU, transferDataFrame, size+1);
}

void UDS_request_transfer_exit(TargetECU targetECU)
{
	uint8_t requestFrame[] = {REQUEST_TRANSFER_EXIT};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_ecu_reset(TargetECU targetECU, uint8_t resetType)
{
	uint8_t requestFrame[] = {ECU_RESET, resetType};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_receive_response(TargetECU targetECU, uint8_t *responseFrame)
{
	if(responseFrame == NULL)
	{
		return;
	}
	switch(responseFrame[0]){
	case READ_DATA_BY_IDENTIFIER + POSITIVE_RESPONSE_OFFSET:
		UDS_RDBI_handle(targetECU, responseFrame);
		break;
	case DIAGNOSTICS_SESSION_CONTROL + POSITIVE_RESPONSE_OFFSET:
		UDS_DSC_handle(targetECU, responseFrame);
		break;
	case SECURITY_ACCESS + POSITIVE_RESPONSE_OFFSET:
		UDS_SA_handle(targetECU, responseFrame);
		break;
	case ROUTINE_CONTROL + POSITIVE_RESPONSE_OFFSET:
		UDS_RC_handle(targetECU, responseFrame);
		break;
	case REQUEST_DOWNLOAD + POSITIVE_RESPONSE_OFFSET:
		UDS_RD_handle(targetECU, responseFrame);
		break;
	case TRANSFER_DATA + POSITIVE_RESPONSE_OFFSET:
		UDS_TD_handle(targetECU, responseFrame);
		break;
	case REQUEST_TRANSFER_EXIT + POSITIVE_RESPONSE_OFFSET:
		UDS_RTE_handle(targetECU, responseFrame);
		break;
	case ECU_RESET + POSITIVE_RESPONSE_OFFSET:
		UDS_ER_handle(targetECU, responseFrame);
		break;
	case NEGATIVE_RESPONSE:
		UDS_negative_response_handle(targetECU, responseFrame);
		break;
	default:		break;
	}
}

void UDS_RDBI_handle(TargetECU targetECU, uint8_t *responseFrame) {
	if (responseFrame[1] == 0xF1 && responseFrame[2] == 0x95) {
		if(targetECU == PS_TARGET){
			target1_version_number[0] = responseFrame[3];
			target1_version_number[1] = responseFrame[4];
			target1_version_number[2] = responseFrame[5];
			target1_version_received = 1;
		}else if(targetECU == WIPERS_TARGET){
			target2_version_number[0] = responseFrame[3];
			target2_version_number[1] = responseFrame[4];
			target2_version_number[2] = responseFrame[5];
			target2_version_received = 1;
		}
	}
	else{
		// Do nothing
	}


}

void UDS_DSC_handle(TargetECU targetECU, uint8_t *responseFrame)
{ 
	//check session EXT and send next frame SA_request_seed
	if(responseFrame[1] == EXTENDED){
		UDS_SA_request_seed(targetECU);
	}
	else{
		// Do nothing
	}

	lcd_set_cursor(1, 8);
	if(targetECU == PS_TARGET){
		lcd_write_string("5");
	}else if(targetECU == WIPERS_TARGET){
		lcd_write_string("9");
	}
}

void UDS_SA_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	if(responseFrame[1] == SA_REQUEST_SEED){
		UDS_SA_send_key(targetECU, &responseFrame[2]);
	}
	else if(responseFrame[1] == SA_SEND_KEY){
		//access granted .. send erase mem or ecu reset request!
		UDS_RC_erase_memory(targetECU);
	}
	else{
		// Do nothing
	}
}

void UDS_RC_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	if(responseFrame[1] == RC_START_ROUTINE){
		if(responseFrame[2] == 0xFF && responseFrame[3] == 0x00){

			osMutexRelease(targetsMutexHandle);

			//tell uart task that target is ready to receive data
			osThreadResume(UartTaskHandle);

			if(targetECU == PS_TARGET){
				lcd_set_cursor(1, 8);
				lcd_write_string("8");
				osThreadSetPriority(target1ThreadID, osPriorityNormal1);
				osThreadSuspend(target1ThreadID);
			}else if(targetECU == WIPERS_TARGET){
				lcd_set_cursor(1, 7);
				lcd_write_string("12");
				osThreadSetPriority(target2ThreadID, osPriorityNormal1);
				osThreadSuspend(target2ThreadID);
			}
			UDS_request_download(targetECU, downloadSize);
		}
		else{
			// Do nothing
		}
	}
	else{
		// Do nothing
	}
}

void UDS_RD_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//adjust global var (chunkSize) and start sending data after receiving it from uart
	chunkSize = (((uint32_t)responseFrame[1])<<8 |((uint32_t)responseFrame[2]));

	//resume uart task to get file with appropriate chunk size
	osThreadResume(UartTaskHandle);
	if(targetECU == PS_TARGET){
		osThreadSuspend(target1ThreadID);
	}else if(targetECU == WIPERS_TARGET){
		osThreadSuspend(target2ThreadID);
	}

	UDS_transfer_data(targetECU);
}

void UDS_TD_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//resume uart task to ack that data is sent correctly and ready for next chunk
	osThreadResume(UartTaskHandle);
	if(targetECU == PS_TARGET){
		osThreadSuspend(target1ThreadID);
	}else if(targetECU == WIPERS_TARGET){
		osThreadSuspend(target2ThreadID);
	}

	if(dataFlag){
		UDS_transfer_data(targetECU);
	}else{
		UDS_request_transfer_exit(targetECU);
	}
}

void UDS_RTE_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//check crc
	target_calculated_CRC = (((uint32_t)responseFrame[1])<<3*8 |((uint32_t)responseFrame[2])<<2*8 |((uint32_t)responseFrame[3])<<8 |((uint32_t)responseFrame[4]));

	osThreadResume(UartTaskHandle);
	if(targetECU == PS_TARGET){
		osThreadSuspend(target1ThreadID);
	}else if(targetECU == WIPERS_TARGET){
		osThreadSuspend(target2ThreadID);
	}

	if (!installationReadyFlag) {
		UDS_request_download(targetECU, downloadSize);
	} else {
		uint8_t resetType = (uint8_t) ER_SOFT_RESET;
		UDS_ecu_reset(targetECU, resetType);
	}
}

void UDS_ER_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	if(targetECU ==PS_TARGET){
		target1InstalledFlag = 1;
	}else if(targetECU == WIPERS_TARGET){
		target2InstalledFlag = 1;
	}
}

void UDS_negative_response_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//to be handled
}