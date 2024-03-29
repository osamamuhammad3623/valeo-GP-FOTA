/*
 * dwt.h
 *
 *  Created on: Apr 19, 2023
 *      Author: osama
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "stm32f4xx_hal.h"

/************************************************************
 * Definitions & Global variables
 * **********************************************************/
#define ULTRASONIC_PORT					GPIOA
#define ULTRASONIC_TRIGGER_PIN			GPIO_PIN_4
#define ULTRASONIC_ECHO_PIN				GPIO_PIN_0
#define ULTRASONIC_DISTANCE_1			20
#define ULTRASONIC_DISTANCE_2			10

extern uint32_t g_ultrasonic_last_measured_distance;

/************************************************************
 * Function Prototypes
 * **********************************************************/

/*
 * initialize DWT
 * Trigger & Echo pins are configured using CubeMX GUI
 * */
void ultrasonic_init(void);

void ultrasonic_get_distance(void);

uint32_t DWT_Delay_Init(void);

#endif /* INC_ULTRASONIC_H_ */
