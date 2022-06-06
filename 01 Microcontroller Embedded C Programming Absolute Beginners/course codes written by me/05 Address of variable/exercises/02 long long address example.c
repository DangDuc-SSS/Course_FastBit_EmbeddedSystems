#include <stdio.h>


int main()
{
	char a1 = 'A';
	char a2 = 'P';
	char a3 = 'P';
	char a4 = 'I';
	char a5 = 'e';
	char a6 = ':';
	char a7 = ')';

	unsigned long long int addressOfa1 = (unsigned long long int)&a1;

	// use %I64u or %IIU to print an unsigned long long int number
	printf("Address opf variable a1 = %I64u\n", addressOfa1);
	return 0;
}
