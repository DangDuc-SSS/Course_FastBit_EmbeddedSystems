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

void array_display(int32_t const *const pArray, uint8_t nItems);
void swapArray(int32_t *const pArray1, int32_t *const pArray2, uint8_t nItems1, uint8_t nItems2);
void wait_for_user_input(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
	uint32_t noItemsArray1 = 0;
	uint32_t noItemsArray2 = 0;


	printf("Array swapping progrma\n");
	printf("Enter no of elements of Array-1 and Array-2:");
	scanf("%d %d",&noItemsArray1,&noItemsArray2);

	int32_t array1[noItemsArray1];
	int32_t array2[noItemsArray2];

	for(uint8_t i = 0; i < noItemsArray1; i++){
		printf("enter element %d of Array-1:",i);
		scanf("%d",&array1[i]);
	}
	for(uint8_t i = 0; i < noItemsArray2; i++){
		printf("enter element %d of Array-1:",i);
		scanf("%d",&array2[i]);
	}

	printf("Contents of the array before swap\n");
	array_display(array1,noItemsArray1);
	printf("\n");
	array_display(array2,noItemsArray2);
	printf("\n");

	swapArray(array1,array2,noItemsArray1,noItemsArray2);

	printf("Contents of array after swap\n");
	array_display(array1,noItemsArray1);
	printf("\n");
	array_display(array2,noItemsArray2);
	printf("\n");

	wait_for_user_input();
	return 0;
}

void array_display(int32_t const *const pArray, uint8_t nItems)
{
	// lets display the contents of the recieved array
	for(uint32_t i = 0; i < nItems; i++){
		printf("%d\t",pArray[i]);
	}
}

void swapArray(int32_t *const pArray1, int32_t *const pArray2, uint8_t nItems1, uint8_t nItems2)
{
	uint8_t size = nItems1;
	if(size > nItems2)
		size = nItems2;

	int32_t temp = 0;
	for(uint8_t i = 0; i < size; i++){
		temp = pArray1[i];
		pArray1[i] = pArray2[i];
		pArray2[i] = temp;
	}
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

