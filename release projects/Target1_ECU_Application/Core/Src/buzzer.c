/*
 * buzzer.c
 *
 *  Created on: Mar 10, 2023
 *      Author: osama
 */

#include "buzzer.h"

void buzzer_on(void){
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);

}

void buzzer_off(void){
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

