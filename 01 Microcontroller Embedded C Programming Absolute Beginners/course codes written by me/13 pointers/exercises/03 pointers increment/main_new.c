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

long long int g_data = 0xFFFEABCD11112345;
int main(void)
{

	short *pAddress = (short*)&g_data;
	printf("Value of pAddress is %p\n",pAddress);
	printf("Value at address %p is %x\n",pAddress, *pAddress);

	pAddress = pAddress + 1;
	printf("Value of pAddress is %p\n",pAddress);
	printf("Value at address %p is %x\n",pAddress, *pAddress);


	return 0;
}

