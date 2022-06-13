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

int main(void) {
	uint8_t const data = 10;
	uint8_t *ptr = (uint8_t*)&data;
	*ptr = 50;
	printf("Value = %u\n",data);

	wait_for_user_input();
	return 0;
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n');
	getchar();
}

