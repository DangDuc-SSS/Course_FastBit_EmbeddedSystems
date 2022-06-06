#include <stdio.h>

// function decleration (function prototyping)
int function_add_numbers(int , int , int);

int main()
{
	int returnValue = function_add_numbers(12, 13, 14);
	printf("Sum = %d\n", returnValue);
	return 0;
}

// function definition
int function_add_numbers(int a, int b, int c)
{
	int sum = a + b + c;
	return sum;
}
