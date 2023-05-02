/****************************************************************************** 
 * 
 * Module: UDS_Client 
 * File Name: uds_client.c 
 * Description: Source file for the UDS client driver 
 * Author: Maria Osama, Maram Nabil 
 * 
 *******************************************************************************/ 
#include "uds_client.h"
#include "tcp_client.h"

/******************************************************************************* 
 *                      Global Variables				*
 *******************************************************************************/
extern HASH_HandleTypeDef hhash;
int count = 0;	//should be removed .. just used for testing
uint8_t transferDataFrame[app_size + 1];

/*******************************************************************************
 *                      Functions Implementations		* 
 *******************************************************************************/ 
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
	uint8_t downloadSize[] = {((uint32_t)fileSize&0xFF000000)>>(24), ((uint32_t)fileSize&0x00FF0000)>>(16),((uint32_t)fileSize&0x0000FF00)>>(8), fileSize&0x000000FF};
	uint8_t requestFrame[] = {REQUEST_DOWNLOAD, downloadSize[0], downloadSize[1], downloadSize[2], downloadSize[3]};
	tcp_SendMessage(targetECU, requestFrame, sizeof(requestFrame));
}

void UDS_transfer_data(TargetECU targetECU)
{
	//use the array used by uart to save data and forward it
	transferDataFrame[0] = TRANSFER_DATA;
	uint16_t i;
	for (i = 0; i < app_size; i++) {  //CHUNK_SIZE
		transferDataFrame[i+1] = data_received[i];
	}
	tcp_SendMessage(targetECU, transferDataFrame, sizeof(transferDataFrame)); //check this in case of receiving last chunk

	/* Blink LED to indicate chunk is sent */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

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

void UDS_DSC_handle(TargetECU targetECU, uint8_t *responseFrame)
{ 
	//check session EXT and send next frame SA
	if(responseFrame[1] == EXTENDED){
		UDS_SA_request_seed(targetECU);
	}
	else{
		// Do nothing
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
			//tell uart task that target is ready! ... wait for semaphore
			UDS_request_download(targetECU, app_size);
		}
		else{
			//can be used in case of check mem!
		}
	}
	else{
		// Do nothing
	}
}

void UDS_RD_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//adjust global var (chunk size) and start sending data after receiving it from uart
	//uint16_t chunk_size = (((uint32_t)responseFrame[1])<<8 |((uint32_t)responseFrame[2]));
	//wait for semaphore
	UDS_transfer_data(targetECU);
}

void UDS_TD_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//tell uart task that data is sent correctly and ready for next chunk!
	//flag for last chunk to check whether to transfer data or exit

//	count++;
//	if(count <= 5) {
//		UDS_transfer_data(targetECU);
//	}
	UDS_request_transfer_exit(targetECU);
}

void UDS_RTE_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	//check crc
}

void UDS_ER_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_negative_response_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_start_request(TargetECU targetECU) {
//	UDS_transfer_data(targetECU, data_received);
//	UDS_RC_erase_memory(targetECU);
	uint8_t sessionType = (uint8_t)EXTENDED;
	UDS_diagnostics_session_control(targetECU, sessionType);
}
