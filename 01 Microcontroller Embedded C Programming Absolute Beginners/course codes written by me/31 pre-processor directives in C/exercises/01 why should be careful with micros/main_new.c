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

#define PI_VALUE 3.1415f
#define AREA_OF_CIRCLE(x) PI_VALUE * x * x	// better to be like this ((PI_VALUE) * (x) * (x))

int main()
{
	// this program shows why you should be careful with multiplications in function like a macro
    float area_circle;
    // right answer
    area_circle = AREA_OF_CIRCLE(2);
    printf("ARea = %f\n",area_circle);	// =>  1415f * 2 * 2
    // wrong answer for the same input
    area_circle = AREA_OF_CIRCLE(1+1);
    printf("ARea = %f\n",area_circle);	// =>  1415f * 1+1 * 1+1


    return 0;
}


