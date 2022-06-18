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

struct carModel {
	unsigned int carNumber;
	uint32_t carPrice;
	uint16_t carMaxSpeed;
	float carWeight;
};
void wait_for_user_input(void);

int main(void) {
	struct carModel carBMW = { 2021, 15000, 220, 1330 };

	printf("Sizeof struct carModel is %d\n",sizeof(carBMW));

	wait_for_user_input();
	return 0;
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

