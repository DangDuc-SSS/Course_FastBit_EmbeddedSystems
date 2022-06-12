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
	int32_t num1 = 0;
	int32_t num2 = 0;
	uint32_t numOfEvenNum = 0;
	printf("Enter the start and end numbers (give space between 2 nos) :");
	scanf("%d%d", &num1, &num2);
	printf("%d\t%d\n",num1,num2);
	printf("Even numbers are\n");

	if(num2 < num1){
		printf("ending number should be > starting number\n");
		wait_for_user_input();
	}

	while(num1 <= num2){
		if(!(num1 & 1)){
			numOfEvenNum++;
			printf("%4d\t",num1);
			fflush(stdout);
		}
		num1++;
	}
	printf("\nTotal number of even nos = %u\n",numOfEvenNum);

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

