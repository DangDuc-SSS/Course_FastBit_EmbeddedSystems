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
#include <math.h>

void wait_for_user_input(void);
void calc_circle_area(void);
void calc_triangle_area(void);
void calc_rectangle_area(void);
void calc_trapezoid_area(void);
void calc_square_area(void);

int main(void)
{
	uint8_t selectedChar = 'a';
	printf("Area Calculation program\n");
	printf("Circle    ---> c\n");
	printf("Triangle  ---> t\n");
	printf("Rectangle ---> r\n");
	printf("Square    ---> s\n");
	printf("Trapezoid ---> z\n");
	rollBack:
	printf("Enter the code here:");
	scanf("%c",&selectedChar);


	switch(selectedChar){
	case 't':
		calc_triangle_area();
		break;

	case 'z':
		calc_trapezoid_area();
		break;

	case 'c':
		calc_circle_area();
		break;

	case 's':
		calc_square_area();
		break;

	case 'r':
		calc_rectangle_area();
		break;
	default:
		printf("please enter a valid char");
		goto rollBack;
	}
	wait_for_user_input();
	return 0;
}

void wait_for_user_input(void)
{
	// wait till the user press enter
	printf("click enter to exit\n");
	while(getchar() != '\n');
	getchar();
}

void calc_circle_area(void)
{
	float area = 0.0;
	float radius = 0;
	printf("Circle area calculation\n");
	printf("Enter the radius value:");
	if(scanf("%f",&radius)){
		if(radius > 0){
			area = M_PI * radius * radius;
			printf("Area = %f\n",area);
		}else{
			printf("radius must be positive number\n");
		}
	}else{
		printf("radius input is not valid\n");
	}
}

void calc_triangle_area(void)
{
	float area = 0.0;
	float base = 0;
	float height = 0;
	printf("triangle area calculation\n");
	printf("Enter the base value:");
	if(scanf("%f",&base)){
		if(base > 0){
			printf("Enter the height value:");
			if(scanf("%f",&height)){
				if(height > 0){
					area = 0.5 * base * height;
					printf("Area = %f\n",area);

				}else{
					printf("height must be positive number\n");
				}

			}else{
				printf("height input is not valid\n");
			}

		}else{
			printf("base must be positive number\n");
		}
	}else{
		printf("base input is not valid\n");
	}
}

void calc_rectangle_area(void)
{
	float area = 0.0;
	float length = 0;
	float width = 0;
	printf("rectangle area calculation\n");
	printf("Enter the length value:");
	if(scanf("%f",&length)){
		if(length > 0){
			printf("Enter the width value:");
			if(scanf("%f",&width)){
				if(width > 0){
					area =  width * length;
					printf("Area = %f\n",area);

				}else{
					printf("width must be positive number\n");
				}

			}else{
				printf("width input is not valid\n");
			}

		}else{
			printf("length must be positive number\n");
		}
	}else{
		printf("length input is not valid\n");
	}
}

void calc_square_area(void)
{
	float area = 0.0;
	float length = 0;
	printf("square area calculation\n");
	printf("Enter the length value:");
	if(scanf("%f",&length)){
		if(length > 0){
			area = length * length;
			printf("Area = %f\n",area);
		}else{
			printf("length must be positive number\n");
		}
	}else{
		printf("length input is not valid\n");
	}
}

void calc_trapezoid_area(void)
{
	float area = 0.0;
	float baseB = 0;
	float baseA = 0;
	float height = 0;
	printf("trapezoid area calculation\n");
	printf("Enter the baseA value:");
	if(scanf("%f",&baseA)){
		if(baseA > 0){
			printf("Enter the baseB value:");
			if(scanf("%f",&baseB)){
				if(baseB > 0){
					printf("Enter the height value:");
					if(scanf("%f",&height)){
						if(height > 0){
							area = 0.5 * (baseA + baseB)* height;
							printf("Area = %f\n",area);

						}else{
							printf("height must be positive number\n");
						}

					}else{
						printf("height input is not valid\n");
					}
				}else{
					printf("baseB must be positive number\n");
				}
			}else{
				printf("baseB input is not valid\n");
			}



		}else{
			printf("baseA must be positive number\n");
		}
	}else{
		printf("baseA input is not valid\n");
	}
}
