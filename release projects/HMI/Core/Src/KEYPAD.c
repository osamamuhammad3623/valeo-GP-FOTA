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


	/*MGPIO_vSetPinMode(KEYPAD_R0_PORT,KEYPAD_R0_PIN,OUTPUT);
	MGPIO_vSetPinMode(KEYPAD_R1_PORT,KEYPAD_R1_PIN,OUTPUT);
	MGPIO_vSetPinMode(KEYPAD_R2_PORT,KEYPAD_R2_PIN,OUTPUT);
	MGPIO_vSetPinMode(KEYPAD_R3_PORT,KEYPAD_R3_PIN,OUTPUT);
    MGPIO_vSetPinOutPutType(KEYPAD_R0_PORT,KEYPAD_R0_PIN,PUSH_PULL);
    MGPIO_vSetPinOutPutType(KEYPAD_R1_PORT,KEYPAD_R1_PIN,PUSH_PULL);
	MGPIO_vSetPinOutPutType(KEYPAD_R2_PORT,KEYPAD_R2_PIN,PUSH_PULL);
    MGPIO_vSetPinOutPutType(KEYPAD_R3_PORT,KEYPAD_R3_PIN,PUSH_PULL);*/

	HAL_GPIO_WritePin(KEYPAD_R0_PORT, KEYPAD_R0_PIN, HIGH);
	HAL_GPIO_WritePin(KEYPAD_R1_PORT,KEYPAD_R1_PIN,HIGH);
	HAL_GPIO_WritePin(KEYPAD_R2_PORT,KEYPAD_R2_PIN,HIGH);
	HAL_GPIO_WritePin(KEYPAD_R3_PORT,KEYPAD_R3_PIN,HIGH);


}
/*
uint8_t  H_KeyPad_void_KeyPadRead(void)
{
	uint8_t Local_uint8_t_Col = 0;
	uint8_t Local_uint8_t_Row = 0;

	uint8_t Local_uint8_t_Arr[4][4] = {{ '1' , '2' , '3' , 'A' },
							 { '4' , '5' , '6' , 'B' },
							 { '7' , '8' , '9' , 'C' },
							 { '*' , '0' , '#' , 'D' }};

	uint8_t Local_uint8_t_Reading = 0;
	for(Local_uint8_t_Row=KEYPAD_R0_PIN;Local_uint8_t_Row<=KEYPAD_R3_PIN;Local_uint8_t_Row++)
	{
		HAL_GPIO_WritePin(KEYPAD_R0_PORT,Local_uint8_t_Row,LOW);
		for(Local_uint8_t_Col=KEYPAD_C0_PIN;Local_uint8_t_Col<=KEYPAD_C3_PIN;Local_uint8_t_Col++)
		{
			if(HAL_GPIO_ReadPin(KEYPAD_C0_PORT,Local_uint8_t_Col) == 0)
			{
         HAL_Delay(100);
				// DWT_Delay_ms(100);
				if(HAL_GPIO_ReadPin(KEYPAD_C0_PORT,Local_uint8_t_Col) == 0)
				{
					Local_uint8_t_Reading = Local_uint8_t_Arr [Local_uint8_t_Row - KEYPAD_R0_PIN][Local_uint8_t_Col - KEYPAD_C0_PIN];
					while(HAL_GPIO_ReadPin(KEYPAD_C0_PORT,Local_uint8_t_Col) == 0);
				}
			}
		}
		HAL_GPIO_WritePin(KEYPAD_R0_PORT,Local_uint8_t_Row,HIGH);
	}
	return Local_uint8_t_Reading;
} */
void Keys_Detect(void)
{
	/* Setting Row 1 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 1");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 2");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 3");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key A");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
	}

	/* Setting Row 2 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 4");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 5");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 6");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key B");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
	}

	/* Setting Row 3 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 7");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 8");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 9");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key C");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
	}

	/* Setting Row 4 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, KEYPAD_R3_PIN, GPIO_PIN_SET);

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C0_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key *");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C0_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C1_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key 0");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C1_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C2_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key #");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C2_PIN));
	}

	if(HAL_GPIO_ReadPin(GPIOD, KEYPAD_C3_PIN) == GPIO_PIN_SET)
	{

	    lcd_write_string_up((uint8_t *)"Key D");
		while (HAL_GPIO_ReadPin (GPIOD, KEYPAD_C3_PIN));
	}
}
