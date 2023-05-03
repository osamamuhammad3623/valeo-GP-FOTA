/*
 * buzzer.h
 *
 *  Created on: Mar 10, 2023
 *      Author: osama
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f4xx_hal.h"

/************************************************************
 * Definitions
 * **********************************************************/
#define BUZZER_PIN		GPIO_PIN_5
#define BUZZER_PORT		GPIOA

/************************************************************
 * Function Prototypes
 * **********************************************************/

// buzzer_init function only configures the pin as output using CubeMX GUI.

/*
 * Description: A function to enable the buzzer
 * */
void buzzer_on(void);

/*
 * Description: A function to disable the buzzer
 * */
void buzzer_off(void);

#endif /* INC_BUZZER_H_ */
