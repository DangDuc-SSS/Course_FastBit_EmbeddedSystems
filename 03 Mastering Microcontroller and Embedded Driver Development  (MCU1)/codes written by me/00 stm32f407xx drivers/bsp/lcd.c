/*
 * lcd.c
 *
 *  Created on: 24 Jul 2022
 *      Author: abdo
 */

#include "lcd.h"

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void udelay(uint32_t cnt);


void lcd_init(void)
{
	// 1. configure the gpio pins which are used for lcd connections
	GPIO_Handle_t lcd_signal;
	lcd_signal.pGPIOx = LCD_GPIO_PORT;
	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
	lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_signal);

	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);


	// 2. do the LCD initialization
	mdelay(40);

	/* RS = 0 , for LCD command*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/* RW = 0 , for Writing tp LCD*/
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(0x3);

	mdelay(5);

	write_4_bits(0x3);

	udelay(150);

	write_4_bits(0x3);

	write_4_bits(0x2);

	// function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	// display on and cursor on
	lcd_send_command(LCD_CMD_DON_CURON);

	lcd_display_clear();

	// entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}


/*writes 4 bit of data/command on to D4,D5,D6,D7 lines*/
static void write_4_bits(uint8_t value)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, ((value >> 0) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, ((value >> 1) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, ((value >> 2) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, ((value >> 3) & 0x1));

	lcd_enable();
}

static void lcd_enable(void)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	udelay(100);
}

void lcd_send_command(uint8_t cmd)
{
	/* RS = 0 for LCD command */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/* R/nW = 0 for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(cmd >> 4);
	write_4_bits(cmd & 0x0F);
}

/*
 * This function sends a character to the LCD
 * Here we used 4 bits parallel data transmission
 * First Higher nibble of the data will be sent on to the data lines D4, D5, D6, D7
 * Then Lower nibble of the data will be set on to the data lines D4, D5, D6, D7
 */
void lcd_print_char(uint8_t data)
{
	/* RS = 1 for user data */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	/* R/nW = 0 for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(data >> 4);		/* Higher nibble*/
	write_4_bits(data & 0x0F);		/* Lower nibble*/
}

void lcd_print_string(char *message)
{
	do
	{
		lcd_print_char((char)*message++);
	}while(*message != '\0');
}

void lcd_display_clear(void)
{
	// Display clear
	lcd_send_command(LCD_CMD_DIS_CLEAR);
	/*
	 * check page number 24 of the datasheet
	 * display clear command execution time is around 2ms
	 */
	mdelay(2);
}

/*Cursor returns to home position */
void lcd_display_return_home(void)
{
	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of the datasheet
	 * return home command execution time is around 2ms
	 */
	mdelay(2);
}

/*
 * Set lcd to a specified location  given by row and column interference
 * Row number (1 to 2)
 * Column number (1 to 16) assuming a 2 x 16 character display
 */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch(row)
	{
		case 1:
			/* Set cursor to 1st row address and add index*/
			lcd_send_command((column |= 0x80));
			break;

		case 2:
			/* Set cursor to 2nd row address and add index*/
			lcd_send_command((column |= 0xC0));
			break;

		default:
			break;
	}
}

void mdelay(uint32_t cnt)
{
	for(uint32_t i = 0; i < (cnt * 1000); i++);
}

static void udelay(uint32_t cnt)
{
	for(uint32_t i = 0; i < (cnt * 1); i++);
}
