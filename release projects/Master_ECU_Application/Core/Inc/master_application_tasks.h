/*
 * application_tasks.h
 *
 *  Created on: Mar 5, 2023
 *      Author: osama
 */

#include "FreeRTOS.h"

#ifndef INC_MASTER_APPLICATION_TASKS_H_
#define INC_MASTER_APPLICATION_TASKS_H_

/***************************************************************
 * Definitions & Global variables
 ***************************************************************/
typedef enum {
	No_Pending_Update,
	Pending_Update, // a notification is received and waiting for user response
	Downloading_From_Server,
	Updating_Targets, // sending updates on Ethernet network
	Installing // all targets received and ready to activate
}Update_State;

/* A global variable that holds the current update state */
extern Update_State current_update_state;


/***************************************************************
 * Function Prototypes
 ***************************************************************/

/*
 * Description: A task to update LCD message, display the current update status every 1 second
 * */
void application_update_LCD_task(void);

/*
 * Description: A task to receive update chunks from UART
 * */
void application_receive_chunks_task(void);

/*
 * Description: A task to send update chunks via Ethernet
 * */
void application_send_chunks_task(void);

/*
 * Description: A task to ensure all targets successfully received their updates & ready to activate
 * */
void application_track_targets_reception_task(void);

#endif /* INC_MASTER_APPLICATION_TASKS_H_ */
