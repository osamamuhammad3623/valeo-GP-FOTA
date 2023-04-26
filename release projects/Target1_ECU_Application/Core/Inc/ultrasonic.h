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
#define ULTRASONIC_TRIGGER_PIN			GPIO_PIN_0
#define ULTRASONIC_ECHO_PIN				GPIO_PIN_0
#define ULTRASONIC_CLOCK_Mhz			180

extern uint32_t g_ultrasonic_last_measured_distance;

/************************************************************
 * Function Prototypes
 * **********************************************************/
uint32_t ultrasonic_get_distance(void);
uint32_t DWT_Delay_Init(void);

/*
 * Description: A function to initialize GPT 5:
 * 		Input-Capture mode, detecting both edges
 * 		Using interrupts
 * 		and start the timer
 * */
void ultrasonic_init(void);

#endif /* INC_ULTRASONIC_H_ */
