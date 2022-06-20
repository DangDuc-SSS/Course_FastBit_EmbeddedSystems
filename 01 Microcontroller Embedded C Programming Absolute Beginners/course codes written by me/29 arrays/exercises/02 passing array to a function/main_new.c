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

void array_display(uint8_t const *const pArray, uint32_t nItems);
void wait_for_user_input(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {

	uint8_t someData[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF };

	for (uint32_t i = 0; i < 10; i++) {
		someData[i] = 0x33;
	}

	uint32_t nItems = sizeof(someData) / sizeof(uint8_t);

	// display from the 0th element
	array_display(someData,nItems);

	// display from the 2nd item
	array_display((someData+2),(nItems-2));
	// of
	array_display(&someData[2],(nItems-2));

	wait_for_user_input();
	return 0;
}

void array_display(uint8_t const *const pArray, uint32_t nItems)
{
	// lets display the contents of the recieved array
	for(uint32_t i = 0; i < nItems; i++){
		//printf("%x\t",*(pArray + i));
		printf("%x\t",pArray[i]);
	}
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

