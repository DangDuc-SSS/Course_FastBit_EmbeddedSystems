#include <stdio.h>

// this is a function prototype
void myFunc1(void);

int main()
{
	myFunc1();

	myFunc1();

	myFunc1();

	myFunc1();

	return 0;
}
void myFunc1(void)
{
	// static is used to make variable private to all other files
	// so it's like global variable but only accessed from this function
	static int count = 0;
	count = count + 1;
	printf("This function executed %d time(s)\n",count);
}