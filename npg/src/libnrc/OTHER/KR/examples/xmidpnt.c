/* Driver for routine midpnt */

#include <stdio.h>
#include <math.h>
#include "nr.h"

#define NMAX 10

/* Test function */
float func(x)
float x;
{
	return 1.0/sqrt(x);
}

/* Integral of test function */
float fint(x)
float x;
{
	return 2.0*sqrt(x);
}

main()
{
	float a=0.0,b=1.0,s;
	int i;

	printf("\nIntegral of func computed with MIDPNT\n");
	printf("Actual value of integral is %7.4f\n",(fint(b)-fint(a)));
	printf("%6s %29s \n","n","Approx. integral");
	for (i=1;i<=NMAX;i++) {
		s=midpnt(func,a,b,i);
		printf("%6d %24.6f\n",i,s);
	}
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
