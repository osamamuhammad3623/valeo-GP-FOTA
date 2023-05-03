/*
 * NodeMCU_COM.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Maria, Nada
 */

#ifndef INC_NODEMCU_COM_H_
#define INC_NODEMCU_COM_H_

#include <stdint.h>
#include "shared.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum {
	Non_Urgent, Urgent
}Urgency;


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define VERSION_NUMBER				0x01U
#define NEW_PACKAGE					0x02U
#define DOWNLOAD_PACKAGE			0x03U
#define PACKAGE_DOWNLOADED			0x04U
#define DOWNLOADING_ERROR			0x14U
#define GET_TARGET_UPDATE			0x05U
#define FILE_SIZE					0x06U
#define START_SENDING				0x07U
#define OK							0x00U
#define NOT_OK						0xFFU

#define NUM_OF_TARGETS				 3

/*******************************************************************************
 *                                Externs	                                  *
 *******************************************************************************/
extern uint8_t dataFlag;
extern uint8_t downloadFinishedFlag;
extern uint8_t target_update[NUM_OF_TARGETS];
extern uint8_t current_version_number[3];

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_stateHandler(void);
void UART_packageDetection(void);
void UART_getTargetUpdate(void);
void UART_downloadFailed(void);
void UART_getDownloadSize(void);
void UART_handleData(void);

#endif /* INC_NODEMCU_COM_H_ */
