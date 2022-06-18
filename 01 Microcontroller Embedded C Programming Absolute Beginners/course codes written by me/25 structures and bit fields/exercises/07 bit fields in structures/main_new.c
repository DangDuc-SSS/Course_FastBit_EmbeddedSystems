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

// the value are arranged from left to right bits
/*
 * addrMode		|	1
 * shortAddr	|	2
 * longAddr		|	8
 * sensor		|	3
 * bat			|	3
 * payLoad		|	12
 * status		|	1
 * crc			|	2
 */

typedef struct {
	uint32_t crc		:2;
	uint32_t status		:1;
	uint32_t payload	:12;
	uint32_t bat		:3;
	uint32_t sensor		:3;
	uint32_t longAddr	:8;
	uint32_t shortAddr	:2;
	uint32_t addrMode	:1;
}Packet_t;

#define GetBitsValue(value,start,end) \
	((value >> (start)) & ((1 << (end - start))-1) )


void wait_for_user_input(void);
void displayMemberElements(Packet_t *pData);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif




int main(void) {
	uint32_t packetValue;
	Packet_t data;

	printf("Enter the 32bit packet value: ");
	scanf("%X",&packetValue);
	data.addrMode = GetBitsValue(packetValue,31,32);
	data.shortAddr = GetBitsValue(packetValue,29,31);
	data.longAddr = GetBitsValue(packetValue,21,29);
	data.sensor = GetBitsValue(packetValue,18,21);
	data.bat = GetBitsValue(packetValue,15,18);
	data.payload = GetBitsValue(packetValue,3,15);
	data.status = GetBitsValue(packetValue,2,3);
	data.crc = GetBitsValue(packetValue,0,2);
	displayMemberElements(&data);
	wait_for_user_input();
	return 0;
}

void displayMemberElements(Packet_t *pData)
{
	printf("crc = %X\n",pData->crc);
	printf("status = %X\n",pData->status);
	printf("payload = %X\n",pData->payload);
	printf("bat = %X\n",pData->bat);
	printf("sensor = %X\n",pData->sensor);
	printf("status = %X\n",pData->status);
	printf("longAddr = %X\n",pData->longAddr);
	printf("shortAddr = %X\n",pData->shortAddr);
	printf("addrMode = %X\n",pData->addrMode);
	printf("Size of data = %d\n",sizeof(pData));
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

