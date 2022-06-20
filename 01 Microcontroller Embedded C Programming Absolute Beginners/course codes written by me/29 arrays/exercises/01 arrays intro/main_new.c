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


void wait_for_user_input(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif




int main(void) {

	uint8_t someData[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	printf("Before : 2nd data item = %x\n",*(someData+1));
	*(someData+1) = 0x9; // equivalent to someData[1] = 0x9;
	printf("after : 2nd data item = %x\n",*(someData+1));

	printf("0th element value = %x\n",someData[0]);
	printf("0th element value = %x\n",*(someData+0));

	someData[2] = 0x33;
	printf("contents of this array\n");
	for(uint8_t i = 0; i < 10 ; i++){
		printf("%x\t",someData[i]);
	}

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

