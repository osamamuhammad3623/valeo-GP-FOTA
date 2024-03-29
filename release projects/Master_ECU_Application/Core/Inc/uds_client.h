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
#include "hash.h"
#include "shared.h"

/******************************************************************************* 
 *                                Definitions                                  * 
 *******************************************************************************/
#define READ_DATA_BY_IDENTIFIER			0x22U
#define DIAGNOSTICS_SESSION_CONTROL		0x10U 
#define SECURITY_ACCESS					0x27U
#define SA_REQUEST_SEED					0x01U
#define SA_SEND_KEY						0x02U
#define TIMEOUT							100U
#define ROUTINE_CONTROL					0x31U
#define RC_START_ROUTINE				0x01U

#define REQUEST_DOWNLOAD				0x34U
#define TRANSFER_DATA					0x36U
#define REQUEST_TRANSFER_EXIT			0x37U
#define ECU_RESET						0x11U
#define ER_SOFT_RESET					0x03U

#define POSITIVE_RESPONSE_OFFSET		0x40U
#define NEGATIVE_RESPONSE				0x7FU
#define CONDITIONS_NOT_CORRECT			0x22U
#define REQUEST_SEQUENCE_ERROR			0x24U
#define SECURITY_ACCESS_DENIED			0x33U
#define INVALID_KEY						0x35U
#define EXCEEDED_NUMBER_OF_ATTEMPTS		0x36U

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
void UDS_init(uint8_t *targetToConnectWith);
void UDS_start_request(TargetECU targetECU);

void UDS_get_version_number(TargetECU targetECU);
void UDS_diagnostics_session_control(TargetECU targetECU, uint8_t session);
void UDS_SA_request_seed(TargetECU targetECU);
void UDS_SA_send_key(TargetECU targetECU, uint8_t *seed);
void UDS_RC_erase_memory(TargetECU targetECU);
void UDS_RC_check_memory(TargetECU targetECU);
void UDS_request_download(TargetECU targetECU, uint32_t fileSize);
void UDS_transfer_data(TargetECU targetECU);
void UDS_request_transfer_exit(TargetECU targetECU);
void UDS_ecu_reset(TargetECU targetECU, uint8_t resetType);

void UDS_receive_response(TargetECU targetECU, uint8_t *responseFrame);

void UDS_RDBI_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_DSC_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_SA_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_RC_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_RD_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_TD_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_RTE_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_ER_handle(TargetECU targetECU, uint8_t *responseFrame);
void UDS_negative_response_handle(TargetECU targetECU, uint8_t *responseFrame);

#endif /* UDS_CLIENT_H_ */ 
