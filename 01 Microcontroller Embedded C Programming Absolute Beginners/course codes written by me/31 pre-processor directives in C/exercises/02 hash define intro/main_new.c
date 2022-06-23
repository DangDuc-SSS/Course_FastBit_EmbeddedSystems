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

//#define AREA_TRI	-> to enable #ifdef
#define AREA_CIR

#undef AREA_CIR		// to undefine a macro

int main()
{

#ifndef AREA_TRI
	float radius = 0;
	printf("This is circle are calculation program\n");
	fflush(stdout);
	printf("Enter the radius : ");
	fflush(stdout);
	scanf("%f",&radius);
	printf("Area of circle = %f\n",(3.1415 * radius * radius));
	fflush(stdout);
#else
	float base,height;
	printf("This is Triangle are calculation program\n");
	fflush(stdout);
	printf("Enter the base and height : ");
	fflush(stdout);
	scanf("%f%f",&base,&height);
	printf("Area of triangle = %f\n",(0.5 * base *height));
#endif

// example 2
/*
#ifdef AREA_CIR
	float radius = 0;
	printf("This is circle are calculation program\n");
	fflush(stdout);
	printf("Enter the radius : ");
	fflush(stdout);
	scanf("%f",&radius);
	printf("Area of circle = %f\n",(3.1415 * radius * radius));
	fflush(stdout);
#endif

#ifdef AREA_TRI
	float base,height;
	printf("This is Triangle are calculation program\n");
	fflush(stdout);
	printf("Enter the base and height : ");
	fflush(stdout);
	scanf("%f%f",&base,&height);
	printf("Area of triangle = %f\n",(0.5 * base *height));
#endif
*/


// example 1
/*
#if 1
	float radius = 0;
	printf("This is circle are calculation program\n");
	fflush(stdout);
	printf("Enter the radius : ");
	fflush(stdout);
	scanf("%f",&radius);
	printf("Area of circle = %f\n",(3.1415 * radius * radius));
	fflush(stdout);
#else
	float base,height;
	printf("This is Triangle are calculation program\n");
	fflush(stdout);
	printf("Enter the base and height : ");
	fflush(stdout);
	scanf("%f%f",&base,&height);
	printf("Area of triangle = %f\n",(0.5 * base *height));
#endif
*/

    return 0;
}


