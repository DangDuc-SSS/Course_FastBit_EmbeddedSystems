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
	double charge, chargeE;
	double electrons;

	printf("Enter the charge: ");
	scanf("%lf",&charge);

	printf("Enter the charge of an electron");
	scanf("%le",&chargeE);

	electrons = (charge / chargeE) * -1;
	printf("total number of electrons = %le\n",electrons);
	printf("total number of electrons = %lf\n",electrons);

	printf("press 'enter' key to exit the application\n");
	getchar();
	while(getchar() != '\n')
	{

	}
	return 0;
}

