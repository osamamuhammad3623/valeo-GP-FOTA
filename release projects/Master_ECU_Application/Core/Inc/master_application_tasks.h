/*
 * application_tasks.h
 *
 *  Created on: Mar 5, 2023
 *      Author: osama
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifndef INC_MASTER_APPLICATION_TASKS_H_
#define INC_MASTER_APPLICATION_TASKS_H_


/***************************************************************
 * Definitions
 ***************************************************************/

/* A queue to store the image chunks */
extern xQueueHandle chunks_queue;

/***************************************************************
 * Function Prototypes
 ***************************************************************/

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
