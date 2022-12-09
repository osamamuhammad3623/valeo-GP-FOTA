#include "stm32f4xx_hal.h"
#include "keypad.h"


/***
Global variables
***/

static uint8_t* g_key_states = {0};

const keypad_ConfigType keypad_config = {
	/* ROWs Pins Info */
	{GPIOA, GPIOA, GPIOA, GPIOA},
	{GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3},
	/* COLs Pins */
	{GPIOA, GPIOA, GPIOA, GPIOA},
	{GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7}
};



void keypad_init(uint8_t* key_states)
{
	uint8_t i = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(key_states != NULL){
		g_key_states = key_states;
	}
	else{
		return;
	}

	for(i=0; i<ROWS; i++)
	{
		if((keypad_config.ROW_GPIO[i] == GPIOA) || (keypad_config.COL_GPIO[i] == GPIOA))
	    {
	        __HAL_RCC_GPIOA_CLK_ENABLE();
	    }
		else if((keypad_config.ROW_GPIO[i] == GPIOB) || (keypad_config.COL_GPIO[i] == GPIOB))
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if((keypad_config.ROW_GPIO[i] == GPIOC) || (keypad_config.COL_GPIO[i] == GPIOC))
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if((keypad_config.ROW_GPIO[i] == GPIOD) || (keypad_config.COL_GPIO[i] == GPIOD))
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if((keypad_config.ROW_GPIO[i] == GPIOE) || (keypad_config.COL_GPIO[i] == GPIOE))
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}

		// configure keypad row pins
		GPIO_InitStruct.Pin = keypad_config.ROW_PIN[i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(keypad_config.ROW_GPIO[i], &GPIO_InitStruct);

		// configure keypad col. pins
		GPIO_InitStruct.Pin = keypad_config.COL_PIN[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(keypad_config.COL_GPIO[i], &GPIO_InitStruct);		
	}
}


void keypad_scan(void)
{
	uint8_t au8_ReadState[ROWS] = {0};
	uint8_t au8_ReadSt;
	uint8_t i=0;

	/* Steady State: Output The Pattern [1 1 1 1] To The COL Pins */
	for(i=0; i<COLS; i++)
	{
		HAL_GPIO_WritePin(keypad_config.COL_GPIO[i], keypad_config.COL_PIN[i], 1);
	}
	/* Steady State: Read The ROW Pins To Know If Any Key Is Pressed At All or Not */
	for(i=0; i<ROWS; i++)
	{
		au8_ReadState[i] = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[i], keypad_config.ROW_PIN[i]);
	}
	/* If Any member in array au8_ReadState is 1, we know that a key is pressed */
	/* And we'll keep changing the COL pins output pattern until we find the pressed key or keys */
	/* Else: We'll end the scan procedure at this point */
	if(au8_ReadState[0] | au8_ReadState[1] | au8_ReadState[2] | au8_ReadState[3])
	{
		/*--[ First Row Check ]--*/
		if(au8_ReadState[0])
		{
			/* Output To The COL Pins This Pattern: [1 0 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW0-COL0 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[0], keypad_config.ROW_PIN[0]);
			if(au8_ReadSt)
			{
				g_key_states[0] = 1;
			}
			else
			{
				g_key_states[0] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 1 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW0-COL1 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[0], keypad_config.ROW_PIN[0]);
			if(au8_ReadSt)
			{
				g_key_states[1] = 1;
			}
			else
			{
				g_key_states[1] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 1 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW0-COL2 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[0], keypad_config.ROW_PIN[0]);
			if(au8_ReadSt)
			{
				g_key_states[2] = 1;
			}
			else
			{
				g_key_states[2] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 0 1] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 1);
			/* Read ROW0-COL3 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[0], keypad_config.ROW_PIN[0]);
			if(au8_ReadSt)
			{
				g_key_states[3] = 1;
			}
		}
		else
		{
			g_key_states[0] = 0;
			g_key_states[1] = 0;
			g_key_states[2] = 0;
			g_key_states[3] = 0;
		}
		/*---------------------[END OF 1st ROW Check ]--------------------*/
		/*--[ Second Row Check ]--*/
		if(au8_ReadState[1])
		{
			/* Output To The COL Pins This Pattern: [1 0 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW1-COL0 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[1], keypad_config.ROW_PIN[1]);
			if(au8_ReadSt)
			{
				g_key_states[4] = 1;
			}
			else
			{
				g_key_states[4] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 1 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW1-COL1 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[1], keypad_config.ROW_PIN[1]);
			if(au8_ReadSt)
			{
				g_key_states[5] = 1;
			}
			else
			{
				g_key_states[5] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 1 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW1-COL2 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[1], keypad_config.ROW_PIN[1]);
			if(au8_ReadSt)
			{
				g_key_states[6] = 1;
			}
			else
			{
				g_key_states[6] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 0 1] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 1);
			/* Read ROW1-COL3 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[1], keypad_config.ROW_PIN[1]);
			if(au8_ReadSt)
			{
				g_key_states[7] = 1;
			}
		}
		else
		{
			g_key_states[4] = 0;
			g_key_states[5] = 0;
			g_key_states[6] = 0;
			g_key_states[7] = 0;
		}
		/*---------------------[END OF 2nd ROW Check ]--------------------*/
		/*--[ Third Row Check ]--*/
		if(au8_ReadState[2])
		{
			/* Output To The COL Pins This Pattern: [1 0 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW2-COL0 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[2], keypad_config.ROW_PIN[2]);
			if(au8_ReadSt)
			{
				g_key_states[8] = 1;
			}
			else
			{
				g_key_states[8] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 1 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW2-COL1 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[2], keypad_config.ROW_PIN[2]);
			if(au8_ReadSt)
			{
				g_key_states[9] = 1;
			}
			else
			{
				g_key_states[9] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 1 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW2-COL2 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[2], keypad_config.ROW_PIN[2]);
			if(au8_ReadSt)
			{
				g_key_states[10] = 1;
			}
			else
			{
				g_key_states[10] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 0 1] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 1);
			/* Read ROW2-COL3 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[2], keypad_config.ROW_PIN[2]);
			if(au8_ReadSt)
			{
				g_key_states[11] = 1;
			}
		}
		else
		{
			g_key_states[8] = 0;
			g_key_states[9] = 0;
			g_key_states[10] = 0;
			g_key_states[11] = 0;
		}
		/*---------------------[END OF 3rd ROW Check ]--------------------*/
		/*--[ Fourth Row Check ]--*/
		if(au8_ReadState[3])
		{
			/* Output To The COL Pins This Pattern: [1 0 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW3-COL0 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[3], keypad_config.ROW_PIN[3]);
			if(au8_ReadSt)
			{
				g_key_states[12] = 1;
			}
			else
			{
				g_key_states[12] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 1 0 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW3-COL1 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[3], keypad_config.ROW_PIN[3]);
			if(au8_ReadSt)
			{
				g_key_states[13] = 1;
			}
			else
			{
				g_key_states[13] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 1 0] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 1);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 0);
			/* Read ROW3-COL2 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[3], keypad_config.ROW_PIN[3]);
			if(au8_ReadSt)
			{
				g_key_states[14] = 1;
			}
			else
			{
				g_key_states[14] = 0;
			}
			/* Output To The COL Pins This Pattern: [0 0 0 1] */
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[0], keypad_config.COL_PIN[0], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[1], keypad_config.COL_PIN[1], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[2], keypad_config.COL_PIN[2], 0);
			HAL_GPIO_WritePin(keypad_config.COL_GPIO[3], keypad_config.COL_PIN[3], 1);
			/* Read ROW3-COL3 */
			au8_ReadSt = HAL_GPIO_ReadPin(keypad_config.ROW_GPIO[3], keypad_config.ROW_PIN[3]);
			if(au8_ReadSt)
			{
				g_key_states[15] = 1;
			}
		}
		else
		{
			g_key_states[12] = 0;
			g_key_states[13] = 0;
			g_key_states[14] = 0;
			g_key_states[15] = 0;
		}
		/*---------------------[END OF 4th ROW Check ]--------------------*/
	}
	/* Else If No Key Is Pressed */
	else
	{
		/* Fill The Keys States With 0s */
		for(i=0; i<KEYS; i++)
		{
			g_key_states[i] = 0;
		}
	}
	/* Back To Steady State: Output The Pattern [1 1 1 1] To The COL Pins */
	for(i=0; i<COLS; i++)
	{
		HAL_GPIO_WritePin(keypad_config.COL_GPIO[i], keypad_config.COL_PIN[i], 1);
	}
}
