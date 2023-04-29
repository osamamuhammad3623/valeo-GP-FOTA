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
int count = 0;
uint8_t requestFrame[app_size + 1]; // uint8_t *requestFrame; 6416+1  1024+1


/*******************************************************************************
 *                      Functions Implementations		* 
 *******************************************************************************/ 
void UDS_diagnostics_session_control(TargetECU targetECU, void *arg)
{ 
	uint8_t session = *(uint8_t *)arg;
	uint8_t requestFrame[] = {DIAGNOSTICS_SESSION_CONTROL, session};
	tcp_SendMessage(targetECU, requestFrame, 2);
}

void UDS_SA_request_seed(TargetECU targetECU, void *arg)
{
	uint8_t requestFrame[] = {SECURITY_ACCESS, SA_REQUEST_SEED};
	tcp_SendMessage(targetECU, requestFrame, 2);
}

void UDS_SA_send_key(TargetECU targetECU, void *arg)
{ 
	uint8_t *generatedKey;
	HAL_HMACEx_SHA256_Start(&hhash, (uint8_t *)arg, 4, generatedKey, TIMEOUT);
	uint8_t requestFrame[34] = {SECURITY_ACCESS, SA_SEND_KEY};
	for(int i=0; i<32; i++)
	{
		requestFrame[i+2] = generatedKey[i];
	}
	tcp_SendMessage(targetECU, requestFrame, 34);
}

void UDS_RC_erase_memory(TargetECU targetECU, void *arg)
{
	uint8_t requestFrame[] = {ROUTINE_CONTROL, RC_START_ROUTINE, 0xFF, 0x00};
	tcp_SendMessage(targetECU, requestFrame, 4);
}

void UDS_RC_check_memory(TargetECU targetECU, void *arg)
{
	uint8_t requestFrame[] = {ROUTINE_CONTROL, RC_START_ROUTINE, 0x02, 0x02};
	tcp_SendMessage(targetECU, requestFrame, 4);
}

void UDS_request_download(TargetECU targetECU, void *arg)
{
	uint32_t size = *(uint32_t *)arg;
	uint8_t downloadSize[] = {((uint32_t)size&0xFF000000)>>(24), ((uint32_t)size&0x00FF0000)>>(16),((uint32_t)size&0x0000FF00)>>(8), size&0x000000FF};
	uint8_t requestFrame[] = {REQUEST_DOWNLOAD, downloadSize[0], downloadSize[1], downloadSize[2], downloadSize[3]};
	tcp_SendMessage(targetECU, requestFrame, 5);
}

void UDS_transfer_data(TargetECU targetECU, void *arg)
{
	//use the array used by uart to save data and forward it
	//frame: (TRANSFER_DATA, data)
	uint8_t *data = (uint8_t *)arg;
	//uint8_t requestFrame[app_size + 1]; // uint8_t *requestFrame; 6416+1  1024+1
	requestFrame[0] = TRANSFER_DATA;
	uint16_t i;
//	for (i = 0; i < app_size; i++) {  // PACKET_SIZE 6416
//		requestFrame[i+1] = data[i];
//	}
	for (i = 0; i < app_size; i++) {  // PACKET_SIZE 6416
		requestFrame[i+1] = data_received[i];
	}
//	tcp_SendMessage(targetECU, data_received, app_size);
	tcp_SendMessage(targetECU, requestFrame, app_size+1); // PACKET_SIZE+1 6416+1

	/* Blink LED to indicate chunk is sent */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

}

void UDS_request_transfer_exit(TargetECU targetECU, void *arg)
{
	uint8_t requestFrame[] = {REQUEST_TRANSFER_EXIT};
	tcp_SendMessage(targetECU, requestFrame, 1);
}

void UDS_ecu_reset(TargetECU targetECU, void *arg)
{
	uint8_t requestFrame[] = {ECU_RESET, ER_SOFT_RESET};
	tcp_SendMessage(targetECU, requestFrame, 2);
}

void UDS_receive_response(TargetECU targetECU, void *arg)
{
	if(arg == NULL)
	{
		//delete task!
	}
	uint8_t *responseFrame = (uint8_t *)arg;
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
	//delete task!
}

void UDS_DSC_handle(TargetECU targetECU, uint8_t *responseFrame)
{ 
//	char msg[20];
//	int messageLength = sprintf(msg , "session success\n");
//	tcp_SendMessage(targetECU, msg, messageLength);

} 


void UDS_SA_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_RC_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	UDS_transfer_data(targetECU, data_received);
	//count++;
//		for(int count = 0 ;count <= 3;count++ ) {
//			UDS_transfer_data(targetECU, data_received);
//		}

}

void UDS_RD_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_TD_handle(TargetECU targetECU, uint8_t *responseFrame)
{
	count++;
	if(count <= 5) {
		UDS_transfer_data(targetECU, data_received);
	}

}

void UDS_RTE_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_ER_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_negative_response_handle(TargetECU targetECU, uint8_t *responseFrame)
{

}

void UDS_start_request(TargetECU targetECU) {
//	UDS_transfer_data(targetECU, data_received);
	void *arg;
	UDS_RC_erase_memory(targetECU, arg);
	/*uint8_t sessionType = (uint8_t)EXTENDED;
	UDS_diagnostics_session_control(targetECU, (void*)&sessionType);*/
}





