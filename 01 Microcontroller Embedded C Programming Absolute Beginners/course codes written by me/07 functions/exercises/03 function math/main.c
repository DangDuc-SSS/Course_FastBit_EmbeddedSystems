#include <stdio.h>
#include "./math/math.h"

int main()
{
	printf("Add : %X\n", math_add(0X0FFF1111, 0X0FFF1111));
	printf("Mul : %I64X\n", math_mul(0X0FFF1111, 0X0FFF1111));
	printf("Mul : %X\n", math_sub(0X0FFF1111, 0X0FFF1111));
	printf("Div : %f\n", math_div(100, 8));
	return 0;
}

