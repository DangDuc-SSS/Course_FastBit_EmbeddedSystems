#ifndef MATH_BTT
#define MATH_BTT

#define SET_BIT(VAR,BTT_NO) \
	VAR = VAR | (1 << BTT_NO)

#define CLR_BIT(VAR,BTT_NO) \
	VAR = VAR & (~(1 << BTT_NO))

#define TOG_BIT(VAR,BTT_NO) \
	VAR = VAR ^ (1 << BTT_NO)

#define GET_BIT(VAR,BTT_NO) \
	(VAR >> BTT_NO) & ~(~0 << 1)

#define ASSIGN_BIT(VAR,BTT_NO,VAL) \
	VAR = (VAR & (~(1<<BTT_NO))) | (VAL << BTT_NO)

#endif