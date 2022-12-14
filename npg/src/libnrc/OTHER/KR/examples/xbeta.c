/* Driver for routine beta */

#include <stdio.h>
#include <string.h>
#include "nr.h"
#include "nrutil.h"

#define MAXSTR 80

main()
{
	char txt[MAXSTR];
	int i,nval;
	float val,w,z;
	FILE *fp;

	if ((fp = fopen("fncval.dat","r")) == NULL)
		nrerror("Data file fncval.dat not found\n");
	fgets(txt,MAXSTR,fp);
	while (strncmp(txt,"Beta Function",13)) {
		fgets(txt,MAXSTR,fp);
		if (feof(fp)) nrerror("Data not found in fncval.dat\n");
	}
	fscanf(fp,"%d %*s",&nval);
	printf("\n%s\n",txt);
	printf("%5s %6s %16s %20s\n","w","z","actual","beta(w,z)");
	for (i=1;i<=nval;i++) {
		fscanf(fp,"%f %f %f",&w,&z,&val);
		printf("%6.2f %6.2f %18.6e %18.6e\n",w,z,val,beta(w,z));
	}
	fclose(fp);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
