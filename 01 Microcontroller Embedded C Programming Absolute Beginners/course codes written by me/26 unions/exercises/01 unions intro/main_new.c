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





#include <stdint.h>
#include <stdio.h>

// union is used when to configure whether to use one of the 2 vars but not both at the same time
// use the union when the access to the data members is mutually execlusive
typedef union{
	uint16_t shortAddr;
	uint32_t longAddr;
}Address_t;


void wait_for_user_input(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif




int main(void) {
	Address_t addr;
	addr.shortAddr = 0XABCD;
	addr.longAddr = 0XEEEECCCC;

	printf("short addr = %#X\n",addr.shortAddr);
	printf("long addr = %#X\n",addr.longAddr);

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

