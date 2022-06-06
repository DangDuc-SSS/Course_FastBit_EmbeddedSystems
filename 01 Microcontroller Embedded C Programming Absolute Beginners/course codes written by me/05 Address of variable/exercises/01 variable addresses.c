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

	// use %p to print a memory address
	printf("Address opf variable a1 = %p\n", &a1);
	printf("Address opf variable a2 = %p\n", &a2);
	printf("Address opf variable a3 = %p\n", &a3);
	printf("Address opf variable a4 = %p\n", &a4);
	printf("Address opf variable a5 = %p\n", &a5);
	printf("Address opf variable a6 = %p\n", &a6);
	return 0;
}
