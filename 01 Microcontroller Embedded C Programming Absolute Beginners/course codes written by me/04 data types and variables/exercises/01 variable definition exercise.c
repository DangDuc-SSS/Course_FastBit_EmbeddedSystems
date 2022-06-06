#include <stdio.h>

int main()
{
	unsigned char distanceA2B = 160;
	unsigned char distanceB2C = 40;
	unsigned char distanceA2C = distanceA2B + distanceB2C;

	/*	 %d to print integer
	*	 %f to print float
	*	 %c to print character
	*	 %s to print string
	*	 %u to print unsigned integer
	*	 %ld to print long int
	*	 %X to print hexa format
	*	 %o to print octal format
	*/
	printf("Totla distance from A2C = %u\n", distanceA2C);

	return 0;
}