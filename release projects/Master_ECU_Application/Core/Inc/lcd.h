/*
 * lcd.h
 *
 *  Created on: Jun 24, 2023
 *      Author: : Abdelrhman Elsayed, Nada Youssef
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include <stdlib.h>

/*
 * LCD Connections: VCC, GND, PB6->SCL, PB9->SDA
 * */

/******************************************************************************/
/******************************************************************************/
/******************************** Definitions *********************************/
/******************************************************************************/
/******************************************************************************/

#define I2C_ADDR 0x27 // I2C address of the PCF8574
#define RS_BIT 0 // Register select bit
#define EN_BIT 2 // Enable bit
#define BL_BIT 3 // Back-light bit
#define D4_BIT 4 // Data 4 bit
#define D5_BIT 5 // Data 5 bit
#define D6_BIT 6 // Data 6 bit
#define D7_BIT 7 // Data 7 bit

#define LCD_ROWS 2 // Number of rows on the LCD
#define LCD_COLS 16 // Number of columns on the LCD

#define LCD_PRINT_DELAY 2000
#define LCD_WAIT_DELAY  10000

/******************************************************************************/
/******************************************************************************/
/*************************** Functions Prototypes *****************************/
/******************************************************************************/
/******************************************************************************/


void lcd_write_nibble(uint8_t nibble, uint8_t rs);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_write_string(char *str);
void lcd_write_int(int data);
void lcd_set_cursor(uint8_t row, uint8_t column);
void lcd_clear(void);
void lcd_backlight(uint8_t state);
void lcd_write_string_up(char *str);
void lcd_write_string_down(char *str);
void lcd_clear_line_up(void);
void lcd_clear_line_down(void);
void lcd_display_version(uint8_t* buffer);
void lcd_init_percentage_parameters(uint8_t *updateTargets);
uint8_t lcd_status_percentage(uint16_t percentage_slice);








#endif /* INC_LCD_H_ */
