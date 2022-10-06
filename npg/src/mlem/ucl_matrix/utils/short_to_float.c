#include <stdlib.h>
#include <stdio.h>
main() 
{
	int i,sx,sy;
	short *sdata;
	float *idata;
	fread(&sx, sizeof(int), 1, stdin);
	fread(&sy, sizeof(int), 1, stdin);
	sdata = (short*)calloc(sizeof(short),sx*sy);
	idata = (float*)calloc(sizeof(float),sx*sy);
	fread(sdata, sizeof(short), sx*sy, stdin);
	for (i=0; i<sx*sy; i++) idata[i] = sdata[i];
	fwrite(&sx,sizeof(int),1,stdout);
	fwrite(&sy,sizeof(int),1,stdout);
	fwrite(idata,sizeof(float),sx*sy,stdout);
}
