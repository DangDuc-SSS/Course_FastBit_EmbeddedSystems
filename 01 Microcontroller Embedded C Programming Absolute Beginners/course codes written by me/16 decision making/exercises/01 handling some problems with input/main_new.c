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
int main(void)
{
	float num1 = 0;
	float num2 = 0;
	int32_t n1, n2;
	// scanf returns 0 -> failed input
	// scanf returns 1 -> successful input
	printf("please enter First number(integer):");
	if(!scanf("%f",&num1)){
		printf("invalid\n");
		wait_for_user_input();
		return 0;
	}
	printf("please enter Second number(integer):");
	if(!scanf("%f",&num2)){
			printf("invalid");
			wait_for_user_input();
			return 0;
		}

	/*we are storing only the integer part of the real numbers*/
	// copy contents from float to integer to see if there is information loss or not
	n1 = num1;
	n2 = num2;

	if((n1 != num1) || (n2 != num2))
		printf("Warning | comparing only integer part\n");

	if(n1 == n2)
		printf("both numbers are equal\n");
	else if(num1 > num2)
		printf("biggest num is: %d\n",n1);
	else
		printf("biggest num is: %d\n",n2);


	return 0;
}
void wait_for_user_input(void)
{
	// wait till the user press enter
	printf("click enter to exit\n");
	while(getchar() != '\n');
	getchar();
}

