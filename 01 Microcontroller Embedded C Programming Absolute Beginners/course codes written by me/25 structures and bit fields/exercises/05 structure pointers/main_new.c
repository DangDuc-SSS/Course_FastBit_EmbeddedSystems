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

typedef struct {
	char data1;
	int data2;
	char data3;
	short data4;
}DataSet_t;


void wait_for_user_input(void);
void displayMemberElements(DataSet_t *pData);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif




int main(void) {
	DataSet_t data;

	data.data1 = 0xAA;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x55;
	data.data4 = 0xA5A5;



	DataSet_t *ptr = &data;

	// data before
	printf("data.data1 = %X\n",data.data1);

	//modify the first element
	ptr->data1 = 0x55;

	// data after
	printf("data.data1 = %X\n",data.data1);

	displayMemberElements(&data);
	wait_for_user_input();
	return 0;
}

void displayMemberElements(DataSet_t *pData)
{
	printf("data1 = %X\n",pData->data1);
	printf("data2 = %X\n",pData->data2);
	printf("data3 = %X\n",pData->data3);
	printf("data4 = %X\n",pData->data4);

}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

