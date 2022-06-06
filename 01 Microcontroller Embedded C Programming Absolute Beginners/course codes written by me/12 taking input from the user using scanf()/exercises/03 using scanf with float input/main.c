/*
 * main.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abdo
 */


#include <stdio.h>
int main(void)
{
	float number1, number2, number3;
	float average;

	printf("Enter the first number:");
	fflush(stdout);
	scanf("%f",&number1);
	printf("\nEnter the second number:");
	fflush(stdout);
	scanf("%f",&number2);
	printf("\nEnter the third number:");
	fflush(stdout);
	scanf("%f",&number3);

	average = (number1 + number2 + number3) / 3;
	printf("\nAverage is : %f \n",average);

	printf("\npress any keyword to exit\n");
	// i used getchar() twice because the input buffer stores \n in it which is read by first getchar() so it doesnot wait
	getchar();
	getchar();

	return 0;
}
