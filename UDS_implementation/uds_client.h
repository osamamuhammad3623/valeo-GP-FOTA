/******************************************************************************
 *
 * Module: UDS_Client
 * File Name: uds_client.h
 * Description: Header file for the UDS client driver
 * Author: Maram Nabil, Maria Osama
 *
 *******************************************************************************/

#ifndef UDS_CLIENT_H_ 
#define UDS_CLIENT_H_  

#include <stdint.h>

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	DEFAULT = 1, PROGRAMMING, EXTENDED, SAFETY, MAX_SESSIONS
}UDS_Session;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UDS_diagnostics_session_control(UDS_Session session);
void UDS_SA_request_seed(void);
void UDS_SA_send_key(uint32_t seed);
void UDS_RC_erase_memory(void);
void UDS_RC_check_memory(void); // to be checked
void UDS_request_download(uint32_t chunkSize);
void UDS_transfer_data(uint8_t* data, uint16_t packetSize);
void UDS_request_transfer_exit(uint32_t crc);
void UDS_ecu_reset(void);

//callback in app 
void UDS_receive_response(struct netbuf* buf);

#endif /* UDS_CLIENT_H_ */ 
