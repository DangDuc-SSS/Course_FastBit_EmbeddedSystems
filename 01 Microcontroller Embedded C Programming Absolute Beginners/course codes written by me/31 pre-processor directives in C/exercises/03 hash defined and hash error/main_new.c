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

#define AREA_CIR
#define AREA_TRI


#if !defined(AREA_CIR) && !defined(AREA_TRI)
#error "No macros defined."
// or you can use
#warning "No macros defined."
#endif


int main()
{

	// defined is used wilth multiple #ifdef
#if defined(AREA_CIR) && defined(AREA_TRI)
	printf("This is the area calculation program for both circle annd triangle\n");
#endif

	float radius = 0;
	printf("This is circle are calculation program\n");
	fflush(stdout);
	printf("Enter the radius : ");
	fflush(stdout);
	scanf("%f",&radius);
	printf("Area of circle = %f\n",(3.1415 * radius * radius));
	fflush(stdout);

	float base,height;
	printf("This is Triangle are calculation program\n");
	fflush(stdout);
	printf("Enter the base and height : ");
	fflush(stdout);
	scanf("%f%f",&base,&height);
	printf("Area of triangle = %f\n",(0.5 * base *height));



    return 0;
}


