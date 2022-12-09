#include <stdint.h>

#ifndef KEYPAD_H_
#define KEYPAD_H_


/***************************************************
					Type Definitions 
****************************************************/
#define ROWS          	4
#define COLS          	4
#define KEYS          	16

#define KEY_PRESSED   	1
#define KEY_RELEASED  	0

// accept key is 1 [index 0] on keypad
#define USER_ACCEPT_KEY		0
// reject key is 0 [index 13] on keypad
#define USER_REJECT_KEY		13

typedef struct
{
	uint8_t*  key_states;
}keypad_keys;

typedef struct
{
	GPIO_TypeDef * ROW_GPIO[ROWS];
	uint16_t       ROW_PIN[ROWS];
	GPIO_TypeDef * COL_GPIO[COLS];
	uint16_t       COL_PIN[COLS];
}keypad_ConfigType;


/***************************************************
				Function Prototypes
****************************************************/

void keypad_init(uint8_t* key_states);
void keypad_scan(void);

#endif /* KEYPAD_H_ */
