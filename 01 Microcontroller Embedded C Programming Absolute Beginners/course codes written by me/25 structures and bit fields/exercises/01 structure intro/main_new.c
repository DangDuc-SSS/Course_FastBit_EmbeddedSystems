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
	struct carModel carFord = { 4031, 35000, 160, 1900.96};
	struct carModel carHonda = {.carWeight = 90.90,.carPrice = 150000};


	printf("Details of car BMW is as follows\n");
	printf("carNumber = %u\n",carBMW.carNumber);
	printf("carPrice = %u\n",carBMW.carPrice);
	printf("carMaxSpeed = %u\n",carBMW.carMaxSpeed);
	printf("carWeight = %f\n",carBMW.carWeight);


	carFord.carNumber = 1000;
	printf("Details of car ford is as follows\n");
	printf("carNumber = %u\n",carFord.carNumber);
	printf("carPrice = %u\n",carFord.carPrice);
	printf("carMaxSpeed = %u\n",carFord.carMaxSpeed);
	printf("carWeight = %f\n",carFord.carWeight);

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

