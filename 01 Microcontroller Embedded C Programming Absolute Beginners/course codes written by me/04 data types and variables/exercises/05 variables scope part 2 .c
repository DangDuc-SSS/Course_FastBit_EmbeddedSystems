#include <stdio.h>

int valueA = 4;

int main()
{
	{
		int valueA = 67;
	}
	
	printf("Value A = %d\n", valueA);

	return 0;
}
