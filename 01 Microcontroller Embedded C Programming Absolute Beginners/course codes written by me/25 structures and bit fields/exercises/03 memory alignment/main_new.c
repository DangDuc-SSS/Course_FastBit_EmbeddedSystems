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
#include <stdint.h>

struct DataSet{
	char data1;
	int data2;
	char data3;
	short data4;
};

void wait_for_user_input(void);

int main(void) {

	struct DataSet data;

	data.data1 = 0x11;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x22;
	data.data4 = 0xABCD;

	uint8_t *ptr;
	ptr = (uint8_t*)&data;

	uint32_t totalSize = sizeof(struct DataSet);

	printf("Memory address 			Content \n");
	printf("================================\n");

	for(uint32_t i = 0; i < totalSize; i++){
		printf("%p 		%X\n",ptr,*ptr);
		ptr++;
	}

	printf("Total memory consumed by this struct variable = %d\n",sizeof(struct DataSet));

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

