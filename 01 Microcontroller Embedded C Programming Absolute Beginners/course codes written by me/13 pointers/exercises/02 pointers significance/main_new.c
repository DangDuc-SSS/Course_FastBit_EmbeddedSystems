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
	char *pAddress1;	//char *ptr; is more used than char* ptr;
	pAddress1 = (char*)&g_data;
	printf("Value at address %p is : %X\n",pAddress1,*pAddress1);

	int *pAddress2;	//int *ptr; is more used than int* ptr;
	pAddress2 = (int*)&g_data;
	printf("Value at address %p is : %X\n",pAddress2,*pAddress2);

	short *pAddress3;	//short *ptr; is more used than short* ptr;
	pAddress3 = (short*)&g_data;
	printf("Value at address %p is : %X\n",pAddress3,*pAddress3);

	long long int *pAddress4;	//long long int *ptr; is more used than long long int* ptr;
	pAddress4 = &g_data;
	printf("Value at address %p is : %I64X\n",pAddress4,*pAddress4);


	return 0;
}

