#include <stdio.h>

// global scope variables
int myScore;

// this is a function prototype of function myFunc1
void myFunc1(void);

int main()
{
	

	myScore = 900;

	printf("Valie of the variable myScore = %d\n",myScore);
	// this is a function call
	myFunc1();

	return 0;
}


// this is a function definition
void myFunc1(void)
{
	// myScore isnot seen here if it's declared in local scope of main
	//printf("Valie of the variable myScore = %d\n",myScore);
}