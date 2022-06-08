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
	float totalIncome = 0;
	float taxPayable = 0;

	printf("please enter income:");
	scanf("%f",&totalIncome);

	if(totalIncome <= 9525)
		taxPayable = 0;
	else if(totalIncome > 9525 && totalIncome <= 38700)
		taxPayable = totalIncome*(12.0/100);
	else if(totalIncome > 38700 && totalIncome < 82500)
		taxPayable = totalIncome*(22.0/100);
	else if(totalIncome > 82500)
		taxPayable = totalIncome*(33.0/100) + 1000;

	printf("the tax you should pay is %f\n",taxPayable);
	// wait till the user press enter
	printf("click enter to exit\n");
	while(getchar() != '\n');
	getchar();

	return 0;
}
