/*
 * KeyPad_program.c
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#include "KEYPAD.h"

#define HIGH 1
#define LOW 0
void H_KeyPad_void_KeyPadInit(void)
{
	HAL_GPIO_WritePin(KEYPAD_R0_PORT, KEYPAD_R0_PIN, HIGH);
	HAL_GPIO_WritePin(KEYPAD_R1_PORT,KEYPAD_R1_PIN,HIGH);
	HAL_GPIO_WritePin(KEYPAD_R2_PORT,KEYPAD_R2_PIN,HIGH);
	HAL_GPIO_WritePin(KEYPAD_R3_PORT,KEYPAD_R3_PIN,HIGH);
}

char Keys_Detect(void)
{
	/* Setting Row 1 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
	    return '1';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
	    return '2';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
	    return '3';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
		return 'A';
	}

	/* Setting Row 2 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
		return '4';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
		return '5';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
		return '6';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
		return 'B';
	}

	/* Setting Row 3 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
		return '7';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
		return '8';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
		return '9';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
		return 'C';
	}

	/* Setting Row 4 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_SET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
		return '*';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
		return '0';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
		return '#';
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
		return 'D';
	}

	return ' ';
}
