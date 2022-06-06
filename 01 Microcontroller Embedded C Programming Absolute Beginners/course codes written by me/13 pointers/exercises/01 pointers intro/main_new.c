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
	//0x00007FFF8E3C3824 -> address
	//char* address1 = (char*)0x00007FFF8E3C3824;

	// create a char type variable and initialize it to value 100
	char var = 100;

	// print the address of the above variable
	printf("address of var = %p\n",&var);

	// create a pointer variable and store the address of the above variable
	char* ptr = &var;

	// perform read operation on the pointer variable to fetch 1 byte of data from the pointer
	char data = *ptr;

	// print the data obtained from the read operation on the pointer
	printf("data value = %d\n",data);

	// perform write operation on the pointer to store the value 65
	*ptr = 65;

	// print the value of the variable defined in step 1
	printf("var = %d\n",var);

	// wait till user press enter
	while(getchar() != '\n');
	return 0;
}

