/* Driver for routine sobseq */

#include <stdio.h>
#include "nr.h"
#include "nrutil.h"

main()
{
	int i,n1=(-1),n2=3;
	float *x;

	x=vector(1,n2);
	sobseq(&n1,x);
	for (i=1;i<=32;i++) {
		sobseq(&n2,x);
		printf(" %10.5f %10.5f %10.5f %5d\n",x[1],x[2],x[3],i);
	}
	free_vector(x,1,n2);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
