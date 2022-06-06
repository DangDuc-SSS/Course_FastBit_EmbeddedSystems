int math_add(int n1, int n2)
{

	return n1 + n2;
}

int math_sub(int n1, int n2)
{
	return n1 - n2;
}

long long int math_mul(int n1, int n2)
{
	// casting is to solve issue of information lost due to integer multiplication
	return (long long int)n1 * n2;
}

float math_div(int n1, int n2)
{
	return (float)n1 / n2;
}