/* Driver for routine plgndr */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nr.h"
#include "nrutil.h"

#define MAXSTR 80

main()
{
	char txt[MAXSTR];
	int i,j,m,n,nval;
	float fac,val,x;
	FILE *fp;

	if ((fp = fopen("fncval.dat","r")) == NULL)
		nrerror("Data file fncval.dat not found\n");
	fgets(txt,MAXSTR,fp);
	while (strncmp(txt,"Legendre Polynomials",20)) {
		fgets(txt,MAXSTR,fp);
		if (feof(fp)) nrerror("Data not found in fncval.dat\n");
	}
	fscanf(fp,"%d %*s",&nval);
	printf("\n%s\n",txt);
	printf("%4s %4s %10s %17s %24s\n","n",
		"m","x","actual","plgndr(n,m,x)");
	for (i=1;i<=nval;i++) {
		fscanf(fp,"%d %d %f %f",&n,&m,&x,&val);
		fac=1.0;
		if (m > 0)
			for (j=n-m+1;j<=n+m;j++) fac *= j;
		fac *= 2.0/(2.0*n+1.0);
		val *= sqrt(fac);
		printf("%4d %4d %13.6f %19.6e %19.6e\n",
			n,m,x,val,plgndr(n,m,x));
	}
	fclose(fp);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
