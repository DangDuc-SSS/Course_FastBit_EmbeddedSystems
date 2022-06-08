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
	int32_t num2 = 0;

	printf("enter the first num: ");
	scanf("%d",&num1);

	printf("enter the second num: ");
	scanf("%d",&num2);

	printf("result of & : %d\n",num1&num2);
	printf("result of | : %d\n",num1|num2);
	printf("result of ^ : %d\n",num1^num2);
	printf("result of ~ for 1st num : %d\n",~num1);
	printf("result of ~ for 2st num : %d\n",~num2);

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

