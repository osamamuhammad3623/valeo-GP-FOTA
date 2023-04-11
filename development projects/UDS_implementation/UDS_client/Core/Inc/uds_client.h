/****************************************************************************** 
 * 
 * Module: UDS_Client 
 * File Name: uds_client.h 
 * Description: Header file for the UDS client driver 
 * Author: Maria Osama, Maram Nabil 
 * 
 *******************************************************************************/

#ifndef UDS_CLIENT_H_ 
#define UDS_CLIENT_H_  

#include <stdint.h>
#include "tcp_client.h"

/******************************************************************************* 
 *                                Definitions                                  * 
 *******************************************************************************/
#define DIAGNOSTICS_SESSION_CONTROL		0x10U 
#define SECURITY_ACCESS				0x27U
#define SA_REQUEST_SEED				0X01U 
#define SA_SEND_KEY					0X02U
#define TIMEOUT						100U
#define ROUTINE_CONTROL				0X31U 
#define RC_START_ROUTINE			0X01U  

#define REQUEST_DOWNLOAD			0X34U
#define TRANSFER_DATA				0X36U 
#define REQUEST_TRANSFER_EXIT		0X37U
#define ECU_RESET					0X11U
#define ER_SOFT_RESET				0X03U  

#define POSITIVE_RESPONSE_OFFSET	0X40U
#define PACKET_SIZE					1024U
#define NEGATIVE_RESPONSE			0X7FU 
#define CONDITIONS_NOT_CORRECT		0X22U
#define REQUEST_SEQUENCE_ERROR		0X24U
#define SECURITY_ACCESS_DENIED		0X33U
#define INVALID_KEY					0X35U
#define EXTENDED_NUMBER_OF_ATTEMPTS		0X36U
//#define PENDING_RESPONSE			0X78U  

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
void UDS_diagnostics_session_control(TargetECU targetECU, void *arg);	// UDS_Session session
void UDS_SA_request_seed(TargetECU targetECU, void *arg);
void UDS_SA_send_key(TargetECU targetECU, void *arg);			// uint32_t seed
void UDS_RC_erase_memory(TargetECU targetECU, void *arg);
void UDS_RC_check_memory(TargetECU targetECU, void *arg);
void UDS_request_download(TargetECU targetECU, void *arg);		// uint32_t downloadSize
void UDS_transfer_data(TargetECU targetECU, void *arg);			//uint16_t packetSize, uint8_t *data
void UDS_request_transfer_exit(TargetECU targetECU, void *arg);
void UDS_ecu_reset(TargetECU targetECU, void *arg);

void UDS_receive_response(TargetECU targetECU, void *arg);

void UDS_DSC_handle(TargetECU targetECU, uint8_t *responseFrame);	//check session EXT and send next frame SA
void UDS_SA_handle(TargetECU targetECU, uint8_t *responseFrame);		//check subfn.. 1 use seed, 2 erase mem req
void UDS_RC_handle(TargetECU targetECU, uint8_t *responseFrame);		//check subfn & routineID
void UDS_RD_handle(TargetECU targetECU, uint8_t *responseFrame);		//adjust var and start sending data
void UDS_TD_handle(TargetECU targetECU, uint8_t *responseFrame);		//dec counter and send data
void UDS_RTE_handle(TargetECU targetECU, uint8_t *responseFrame);	//check crc and start down or finish
void UDS_ER_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_negative_response_handle(TargetECU targetECU, uint8_t *responseFrame);

void UDS_start_request(TargetECU targetECU);
//if there is an update for gateway, imp. fns for it in uart part 
//erase, flash (+ CRC!) and reboot  

#endif /* UDS_CLIENT_H_ */ 
