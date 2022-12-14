/* Driver for routine ellpi */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nr.h"
#include "nrutil.h"

#define MAXSTR 80

#define FAC (3.1415926535/180.0)

main()
{
	char txt[MAXSTR];
	int i,nval;
	float ak,alpha,en,phi,val;
	FILE *fp;

	if ((fp = fopen("fncval.dat","r")) == NULL)
		nrerror("Data file fncval.dat not found\n");
	fgets(txt,MAXSTR,fp);
	while (strncmp(txt,"Legendre Elliptic Integral Third Kind",37)) {
		fgets(txt,MAXSTR,fp);
		if (feof(fp)) nrerror("Data not found in fncval.dat\n");
	}
	fscanf(fp,"%d %*s",&nval);
	printf("\n%s\n",txt);
	printf("%6s %6s %6s %10s %23s\n",
		"phi","-en","sin(alpha)","actual","ellpi(phi,ak)");
	for (i=1;i<=nval;i++) {
		fscanf(fp,"%f %f %f %f",&phi,&en,&alpha,&val);
		alpha=alpha*FAC;
		ak=sin(alpha);
		en = -en;
		phi=phi*FAC;
		printf("%6.2f %6.2f %6.2f %18.6e %18.6e\n",
			phi,en,ak,val,ellpi(phi,en,ak));

	}
	fclose(fp);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
