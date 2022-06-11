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
	int32_t num1 = 0;
	printf("Enter a number:");
	scanf("%d",&num1);

	if(num1 & 1){
		printf("%d is odd number\n",num1);
	}else{
		printf("%d us even number\n",num1);
	}

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

