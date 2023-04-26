/*
 * dwt.c
 *
 *  Created on: Apr 19, 2023
 *      Author: osama
 */

#include "ultrasonic.h"

uint32_t g_ultrasonic_last_measured_distance=0;
TIM_HandleTypeDef htim5;


__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
	uint32_t clk_cycle_start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

uint32_t ultrasonic_get_distance(void){
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
	return (local_time/58); // time is divided by 58 to get distance by cm [number is from the HC-SR04 datasheet]
}

uint32_t DWT_Delay_Init(void) {
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

void ultrasonic_init(void){

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 0;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 4294967295;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	// start the timer
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim){

	// if the source is timer channel 1
	if (htim->Channel == TIM_CHANNEL_1){

		static uint32_t u32_rising_value=0; // timer value at the rising edge
		static uint32_t u32_falling_value=0; // timer value at the falling edge
		uint32_t u32_echo_clocks=0;

		// if Echo pin is high, this it's the rising edge, otherwise it's the falling edge
		if (HAL_GPIO_ReadPin(ULTRASONIC_PORT, ULTRASONIC_ECHO_PIN) == GPIO_PIN_SET){
			u32_rising_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		}else{
			u32_falling_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

			if (u32_falling_value > u32_rising_value){
				u32_echo_clocks= u32_falling_value - u32_rising_value;
			}else{
				u32_echo_clocks = (0xFFFFFFFF - u32_rising_value) + u32_falling_value;
			}

			// to get the echo time = clock cycles / Frequency
			uint32_t u32_echo_time = u32_echo_clocks / (ULTRASONIC_CLOCK_Mhz*10e6);

			// to get distance in cm, divide by 58 (from data sheet)
			g_ultrasonic_last_measured_distance = (u32_echo_time/58);
		}
	}
}
