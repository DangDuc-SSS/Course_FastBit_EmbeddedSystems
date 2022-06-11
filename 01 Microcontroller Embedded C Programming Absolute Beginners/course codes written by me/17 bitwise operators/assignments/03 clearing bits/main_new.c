/*
 * main_new.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abdo
 */

/*
 * main.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abdo
 */


#include <stdio.h>
#include <stdint.h>
void wait_for_user_input(void);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)	\
	(byte & 0x80 ? '1' : '0'), \
	(byte & 0x40 ? '1' : '0'), \
	(byte & 0x20 ? '1' : '0'), \
	(byte & 0x10 ? '1' : '0'), \
	(byte & 0x08 ? '1' : '0'), \
	(byte & 0x04 ? '1' : '0'), \
	(byte & 0x02 ? '1' : '0'), \
	(byte & 0x01 ? '1' : '0')



int main(void)
{
	int32_t num1 = 0;
	printf("Enter a number:");
	scanf("%d",&num1);

	printf("number before : " BYTE_TO_BINARY_PATTERN " "BYTE_TO_BINARY_PATTERN "\n",BYTE_TO_BINARY(num1 >> 8),BYTE_TO_BINARY(num1));
	//int32_t temp_clr = 0b01110000;
	//temp_clr = ~temp_clr;
	//num1 = num1 & temp_clr;
	num1 = num1 & ~0b01110000;
	printf("number after : " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n",BYTE_TO_BINARY(num1 >> 8),BYTE_TO_BINARY(num1));

	wait_for_user_input();
	return 0;
}

void wait_for_user_input(void)
{
	// wait till the user press enter
	printf("click enter to exit\n");
	while(getchar() != '\n');
	getchar();
}

