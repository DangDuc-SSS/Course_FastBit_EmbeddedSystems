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
int main(void)
{
	double number1, number2, number3;
	double average;

	printf("Enter 3 numbers: ");
	fflush(stdout);
	scanf("%lf %lf %lf",&number1, &number2, &number3);

	average = (number1 + number2 + number3) / 3;
	printf("\nAverage is : %0.3lf \n",average);

	printf("\npress any keyword to exit\n");
	// i used getchar() twice because the input buffer stores \n in it which is read by first getchar() so it doesnot wait
	getchar();
	getchar();

	return 0;
}

