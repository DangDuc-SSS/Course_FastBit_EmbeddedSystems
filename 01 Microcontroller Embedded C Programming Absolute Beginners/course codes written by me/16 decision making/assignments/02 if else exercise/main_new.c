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
	uint32_t num1 = 0;
	uint32_t num2 = 0;

	printf("please enter First number(integer):\n");
	scanf("%d",&num1);
	printf("please enter Second number(integer):\n");
	scanf("%d",&num2);

	if(num1 == num2)
		printf("both numbers are equal\n");
	else if(num1 > num2)
		printf("biggest num is: %d\n",num1);
	else
		printf("biggest num is: %d\n",num2);

	// wait till the user press enter
	printf("click enter to exit\n");
	getchar();
	while(getchar() != '\n');
	return 0;
}

