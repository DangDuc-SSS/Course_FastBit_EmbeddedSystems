#include <stdio.h>

void file1_myFun1(void);

// use static int mainPrivateData; -> to make the variable private 
int mainPrivateData; // if you used static before int , it will throw error because the other file is trying to modify it

int main()
{
	mainPrivateData = 1000;

	printf("mainPrivateData = %d\n", mainPrivateData);
	
	file1_myFun1();

	printf("mainPrivateData = %d\n", mainPrivateData);

	return 0;
}

// use static keyword with the function to make it private
static void change_system_clock(int system_clock)
{
	printf("system clock changed to = %d\n", system_clock);
}