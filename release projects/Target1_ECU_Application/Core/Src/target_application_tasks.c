/*
 * target_application_tasks.c
 *
 *  Created on: Apr 26, 2023
 *      Author: osama
 */

#include "target_application_tasks.h"


void application_task(void){
	while(1){
		if (g_ultrasonic_last_measured_distance < MINIMUM_DISTANCE){
			buzzer_on();
		}else{
			buzzer_off();
		}

		osDelay(DELAY_TICKS);
	}
}
