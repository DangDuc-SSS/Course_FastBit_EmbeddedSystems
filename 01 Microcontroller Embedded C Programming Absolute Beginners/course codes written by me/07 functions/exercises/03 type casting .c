#include <stdio.h>

int main()
{
	//unsigned char data = (unsigned char)(0xFF00 + 0x87);
	unsigned char data = 0x1FFFFFFFA0B0 + 0X1245;
	float result = (float)80 / 3;
	printf("Data : %u result : %f\n",data,result);
	return 0;
}

