/*
 * target_application_tasks.h
 *
 *  Created on: Apr 26, 2023
 *      Author: osama
 */

#ifndef INC_TARGET_APPLICATION_TASKS_H_
#define INC_TARGET_APPLICATION_TASKS_H_

#include "cmsis_os.h"
#include "ultrasonic.h"
#include "buzzer.h"

/************************************************************
 * Definitions
 * **********************************************************/
#define MINIMUM_DISTANCE	20
#define DELAY_TICKS			1000

/************************************************************
 * Function Prototypes
 * **********************************************************/

/*
 * Description: A task that checks the distance
 * 	measured by ultrasonic module every DELAY_TICKS,
 * 	if distance is less than MINIMUM_DISTANCE, the buzzer goes on
 * */
void application_task(void);

#endif /* INC_TARGET_APPLICATION_TASKS_H_ */
