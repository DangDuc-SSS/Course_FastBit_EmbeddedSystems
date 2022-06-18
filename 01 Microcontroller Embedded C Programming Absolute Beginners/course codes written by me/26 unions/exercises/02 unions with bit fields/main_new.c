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

typedef union {
	uint32_t packetValue;
	struct {
		uint32_t crc		:2;	// LSB
		uint32_t status		:1;
		uint32_t payload	:12;
		uint32_t bat		:3;
		uint32_t sensor		:3;
		uint32_t longAddr	:8;
		uint32_t shortAddr	:2;
		uint32_t addrMode	:1;	// MSB
	}packetFields;

}Packet_t;

#define GetBitsValue(value,start,end) \
	((value >> (start)) & ((1 << (end - start))-1) )


void wait_for_user_input(void);
void displayMemberElements(Packet_t *pData);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif




int main(void) {
	Packet_t data;

	printf("Enter the 32bit packet value: ");
	scanf("%X",&data.packetValue);

	displayMemberElements(&data);
	wait_for_user_input();
	return 0;
}

void displayMemberElements(Packet_t *pData)
{
	printf("crc = %X\n",pData->packetFields.crc);
	printf("status = %X\n",pData->packetFields.status);
	printf("payload = %X\n",pData->packetFields.payload);
	printf("bat = %X\n",pData->packetFields.bat);
	printf("sensor = %X\n",pData->packetFields.sensor);
	printf("status = %X\n",pData->packetFields.status);
	printf("longAddr = %X\n",pData->packetFields.longAddr);
	printf("shortAddr = %X\n",pData->packetFields.shortAddr);
	printf("addrMode = %X\n",pData->packetFields.addrMode);
	printf("Size of data = %d\n",sizeof(pData));
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

