/* Driver for routine f1dim */

#include <stdio.h>
#include "nr.h"
#include "nrutil.h"

float func(x)
float x[];
{
	int i;
	float f=0.0;

	for (i=1;i<=3;i++) f += (x[i]-1.0)*(x[i]-1.0);
	return f;
}

#define NDIM 3

int ncom;   /* defining declarations */
float *pcom,*xicom,(*nrfunc)();

main()
{
	ncom=NDIM;
	pcom=vector(1,ncom);
	xicom=vector(1,ncom);
	nrfunc=func;
	pcom[1]=pcom[2]=pcom[3]=0.0;
	printf("\nEnter vector direction along which to\n");
	printf("plot the function. Minimum is in the\n");
	printf("direction 1.0 1.0 1.0 - enter x y z:\n");
	scanf(" %f %f %f",&xicom[1],&xicom[2],&xicom[3]);
	scrsho(f1dim);
	free_vector(xicom,1,ncom);
	free_vector(pcom,1,ncom);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
