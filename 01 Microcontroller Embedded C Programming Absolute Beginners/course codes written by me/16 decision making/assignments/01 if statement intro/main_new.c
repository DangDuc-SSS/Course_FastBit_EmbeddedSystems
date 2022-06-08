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

int main(void)
{
	uint32_t age = 0;
	printf("please enter your age\n");
	scanf("%d",&age);

	if(age >= 18)
		printf("you can cast a vote\n");
	else
		printf("you cannot cast a vote\n");

	// wait till the user press enter
	printf("click enter to exit\n");
	getchar();
	while(getchar() != '\n');
	return 0;
}

