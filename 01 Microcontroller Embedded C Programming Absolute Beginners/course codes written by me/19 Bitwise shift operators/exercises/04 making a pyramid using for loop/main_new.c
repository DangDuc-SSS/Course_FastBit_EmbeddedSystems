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
#include <stdint.h>
void wait_for_user_input(void);

int main(void) {
	uint32_t height = 0;
	printf("Enter the the height of the pyramid :");
	scanf("%d", &height);

	for (uint32_t i = 0; i < height; i++){
		for (uint32_t j = 0; j < i + 1; j++){
			printf("%d\t",(i-j+1));
		}
		printf("\n");
	}
	wait_for_user_input();
	return 0;
}

void wait_for_user_input(void) {
	// wait till the user press enter
	printf("click enter to exit\n");
	while (getchar() != '\n')
		;
	getchar();
}

