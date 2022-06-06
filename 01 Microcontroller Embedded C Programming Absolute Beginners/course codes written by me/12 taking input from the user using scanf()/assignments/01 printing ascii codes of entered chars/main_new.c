/*
 * main_new.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abdo
 */

/*
 * main.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abdo
 */


#include <stdio.h>
int main(void)
{
	char ascii_1, ascii_2, ascii_3, ascii_4, ascii_5, ascii_6;

	printf("Enter 6 characters of your choice : ");
	fflush(stdout);
	scanf("%c %c %c %c %c %c",&ascii_1, &ascii_2, &ascii_3, &ascii_4, &ascii_5, &ascii_6);

	printf("\nASCII codes: %u %u %u %u %u %u \n",ascii_1, ascii_2, ascii_3, ascii_4, ascii_5, ascii_6);

	printf("\npress any keyword to exit\n");
	// i used getchar() twice because the input buffer stores \n in it which is read by first getchar() so it doesnot wait
	getchar();
	getchar();

	return 0;
}

