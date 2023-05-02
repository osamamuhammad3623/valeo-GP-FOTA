/******************************************************************************
 * 
 * Module: UDS_Server 
 * File Name: uds_server.h 
 * Description: Header file for the UDS server driver 
 * Author: Maram Nabil, Maria Osama 
 *
 *******************************************************************************/
#ifndef UDS_SERVER_H_ 
#define UDS_SERVER_H_ 

#include <stdint.h> 

/******************************************************************************* 
 *                                Definitions                                  * 
 *******************************************************************************/
#define DIAGNOSTICS_SESSION_CONTROL		0x10U 
#define SECURITY_ACCESS					0x27U
#define SA_REQUEST_SEED					0X01U
#define SA_SEND_KEY						0X02U
#define TIMEOUT							100U

#define ROUTINE_CONTROL					0X31U
#define RC_START_ROUTINE				0X01U 

#define REQUEST_DOWNLOAD				0X34U 
//#define CHUNK_SIZE						20000U
#define TRANSFER_DATA					0X36U
#define REQUEST_TRANSFER_EXIT			0X37U 
#define ECU_RESET						0X11U
#define ER_SOFT_RESET					0X03U

#define POSITIVE_RESPONSE_OFFSET		0X40U
#define NEGATIVE_RESPONSE				0X7FU 
#define SERVICE_NOT_SUPPORTED			0X11U 
#define SUBFUNCTION_NOT_SUPPORTED		0X12U 
#define CONDITIONS_NOT_CORRECT			0X22U 
#define REQUEST_SEQUENCE_ERROR			0X24U 
#define REQUEST_OUT_OF_RANGE			0X31U 
#define SECURITY_ACCESS_DENIED			0X33U 
#define INVALID_KEY						0X35U
#define EXCEEDED_NUMBER_OF_ATTEMPTS		0X36U
#define MAX_NUMBER_OF_ATTEMPTS			0X03U 
//#define PENDING_RESPONSE				0X78U 

/******************************************************************************* 
 *                         Types Declaration                                   * 
*******************************************************************************/
typedef enum 
{ 
	ACCESS_DENIED, ACCESS_GRANTED
}UDS_Security_Access; 

typedef enum 
{ 
	DEFAULT = 1, PROGRAMMING, EXTENDED, SAFETY, MAX_SESSIONS
}UDS_Session; 

/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/
void UDS_init(void);
void UDS_init_conn_state(void);
void UDS_execute_request(void *arg); 

void UDS_change_session(uint8_t *requestFrame); 
void UDS_security_access(uint8_t *requestFrame); 
void UDS_generate_seed(uint8_t *requestFrame); 
void UDS_verify_key(uint8_t *requestFrame);	 
void UDS_call_routine(uint8_t *requestFrame); 
void UDS_erase_memory_routine(uint8_t *requestFrame); 
void UDS_check_memory_routine(uint8_t *requestFrame); 
void UDS_start_download(uint8_t *requestFrame); 
void UDS_process_data(uint8_t *requestFrame); 
void UDS_exit_download(uint8_t *requestFrame); 
void UDS_reboot(uint8_t *requestFrame); 

//int bytesToWords(uint8_t* dataBytes, uint32_t dataSizeInBytes, uint32_t * dataWords);
//void padWithOnes(uint32_t dataSizeInBytes, uint32_t * dataWords, uint32_t dataSizeInWords);

#endif /* UDS_SERVER_H_ */
