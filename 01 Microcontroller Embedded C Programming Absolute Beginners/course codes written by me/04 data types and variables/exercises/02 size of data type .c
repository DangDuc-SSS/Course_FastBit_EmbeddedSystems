#include <stdio.h>

int main()
{
	// another way of knowing size of each variable is to read compiler reference manual
	long long myLongHistory = 900;
	char size = sizeof(myLongHistory);

	printf("size of char data type		= %d\n", sizeof(char));
	printf("size of short data type		= %d\n", sizeof(short));
	printf("size of int data type		= %d\n", sizeof(int));
	printf("size of long data type		= %d\n", sizeof(long));
	printf("size of long long data type = %d\n", sizeof(long long));
	printf("size of long long data type = %d\n", sizeof(myLongHistory));
	printf("size of long long data type = %d\n", size);
	return 0;
}