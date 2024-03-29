/*
 * lcd.c
 *
 *  Created on: Jun 24, 2023
 *      Author: Abdelrhman Elsayed, Nada Youssef
 */
#include "lcd.h"
#include "i2c.h"
#include "string.h"


/******************************************************************************/
/******************************************************************************/
/************************** Private Global Variables **************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
// Define global variable for back-light state
uint8_t backlight_state = 1;

// Define global variables for download percentage calculation
uint32_t quotient;
uint32_t divisor;

/******************************************************************************/
/******************************************************************************/
/*************************** Functions Definitions ****************************/
/******************************************************************************/
/******************************************************************************/
void lcd_write_nibble(uint8_t nibble, uint8_t rs) {
	uint8_t data = nibble << D4_BIT;
	data |= rs << RS_BIT;
	data |= backlight_state << BL_BIT; // Include backlight state in data
	data |= 1 << EN_BIT;
	HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR << 1, &data, 1, 100);
	HAL_Delay(1);
	data &= ~(1 << EN_BIT);
	HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR << 1, &data, 1, 100);
}

void lcd_send_cmd(uint8_t cmd) {
	uint8_t upper_nibble = cmd >> 4;
	uint8_t lower_nibble = cmd & 0x0F;
	lcd_write_nibble(upper_nibble, 0);
	lcd_write_nibble(lower_nibble, 0);
	if (cmd == 0x01 || cmd == 0x02) {
		HAL_Delay(2);
	}
}

void lcd_send_data(uint8_t data) {
	uint8_t upper_nibble = data >> 4;
	uint8_t lower_nibble = data & 0x0F;
	lcd_write_nibble(upper_nibble, 1);
	lcd_write_nibble(lower_nibble, 1);
}

void lcd_init(void) {
	HAL_Delay(50);
	lcd_write_nibble(0x03, 0);
	HAL_Delay(5);
	lcd_write_nibble(0x03, 0);
	HAL_Delay(1);
	lcd_write_nibble(0x03, 0);
	HAL_Delay(1);
	lcd_write_nibble(0x02, 0);
	lcd_send_cmd(0x28);
	lcd_send_cmd(0x0C);
	lcd_send_cmd(0x06);
	lcd_send_cmd(0x01);
	HAL_Delay(2);
}

void lcd_write_string(char *str) {
	while (*str) {
		lcd_send_data(*str++);
	}
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void lcd_write_int(int data) {
	char buff[16]; /* String to hold the ascii result */
	itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	lcd_write_string(buff); /* Display the string */
}

void lcd_set_cursor(uint8_t row, uint8_t column) {
	uint8_t address;
	switch (row) {
	case 0:
		address = 0x00;
		break;
	case 1:
		address = 0x40;
		break;
	default:
		address = 0x00;
	}
	address += column;
	lcd_send_cmd(0x80 | address);
}

void lcd_clear(void) {
	lcd_send_cmd(0x01);
	HAL_Delay(2);
}

void lcd_backlight(uint8_t state) {
	if (state) {
		backlight_state = 1;
	} else {
		backlight_state = 0;
	}
}

void lcd_write_string_up(char *str) {
	lcd_set_cursor(0, 0);
	lcd_write_string(str);
}

void lcd_write_string_down(char *str) {
	lcd_set_cursor(1, 0);
	lcd_write_string(str);
}

void lcd_clear_line_up(void){
	char* spaces ="                ";
	lcd_set_cursor(0, 0);
	lcd_write_string(spaces);
}

void lcd_clear_line_down(void){
	char* spaces ="                ";
	lcd_set_cursor(1, 0);
	lcd_write_string(spaces);
}

void lcd_display_version(uint8_t* buffer){

	lcd_set_cursor(1, 8);
	char buff[16]; /* String to hold the ascii result */
	itoa(buffer[0],buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	lcd_write_string(buff); /* Display the string */
	lcd_set_cursor(1, 9);
	lcd_write_string("."); /* Display the string */

	lcd_set_cursor(1, 10);
	itoa(buffer[1],buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	lcd_write_string(buff); /* Display the string */
	lcd_set_cursor(1, 11);
	lcd_write_string("."); /* Display the string */

	lcd_set_cursor(1, 12);
	itoa(buffer[2],buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	lcd_write_string(buff); /* Display the string */
}

void lcd_init_percentage_parameters(uint8_t *updateTargets){
	divisor = 0;
	quotient = 1;

	if(updateTargets[0]){
		quotient += 140000;
	}
	if(updateTargets[1]){
		quotient += 85000;
	}
	if(updateTargets[2]){
		quotient += 85000;
	}
}

uint8_t lcd_status_percentage(uint16_t percentage_slice){
	divisor += percentage_slice;
	uint8_t result = ((float)divisor / quotient) * 80 + 20;
	return (result < 100? result : 100);
}
