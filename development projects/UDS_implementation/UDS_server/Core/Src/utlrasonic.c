/*
 * dwt.c
 *
 *  Created on: Apr 19, 2023
 *      Author: osama
 */

#include "ultrasonic.h"

uint32_t g_ultrasonic_last_measured_distance=0;


void DWT_Delay_us(volatile uint32_t microseconds)
{
	uint32_t clk_cycle_start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void ultrasonic_get_distance(void){
	uint32_t local_time=0;
	// set the trigger pin for 10 uS and then clear it
	HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_PIN_SET);
	DWT_Delay_us(10);
	HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_PIN_RESET);

	// measure time for Echo pin
	while(!HAL_GPIO_ReadPin(ULTRASONIC_PORT, ULTRASONIC_ECHO_PIN));
	while(HAL_GPIO_ReadPin(ULTRASONIC_PORT, ULTRASONIC_ECHO_PIN)){
		local_time++;
		DWT_Delay_us(1);
	}

	g_ultrasonic_last_measured_distance =  (local_time/58); // time is divided by 58 to get distance by cm [number is from the HC-SR04 datasheet]
}


void ultrasonic_init(void){

	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

	/* Disable clock cycle counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
	/* Enable  clock cycle counter */
	DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

	/* Reset the clock cycle counter value */
	DWT->CYCCNT = 0;

	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

	/* Check if clock cycle counter has started */
	if(DWT->CYCCNT)
	{
		return 0; /*clock cycle counter started*/
	}
	else
	{
		return 1; /*clock cycle counter not started*/
	}
}
