/*$Id: d_uvlc.c,v 1.1 1995/10/16 17:33:51 ty7777 Exp $*/
/*$Log: d_uvlc.c,v $
 * Revision 1.1  1995/10/16  17:33:51  ty7777
 * Initial revision
 **/

/*
 *  Copyright (C) 1993 University of Louvain, Louvain-la-Neuve, Belgium
 *  Telecommunication Laboratory.
 *
 *  This program may be used free of charge by the members
 *  of all academic and/or scientific institutions.
 *       ANY OTHER USE IS PROHIBITED.
 *  It may also be included in any package
 *      -  provided that this text is not removed from
 *      the source code and that reference to this original
 *      work is done ;
 *      - provided that this package is itself available
 *      for use, free of charge, to the members of all
 *      academic and/or scientific institutions.
 *  Nor the author, nor the Universite Catholique de Louvain, in any
 *  way guarantee that this program will fullfill any particular
 *  requirement, nor even that its use will always be harmless.
 *
 *
*/
/* 
		pdpet.c

	pdpet (-in param)	in	out

*/

#include <stdio.h>
#include "uvlc.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/d_uvlc.c,v 1.1 1995/10/16 17:33:51 ty7777 Exp $";

/* main routine */
pdpet(skipdpcm, Long, input_filename, output_filename, ncol,nline)
int skipdpcm, Long;
char *input_filename, *output_filename;
int ncol,nline;
{
	int *p1,*p2,*pn,i,j,i1,i2,j1,pp2[1024],d1,d2,d3,d4,dmin;
	int pred,l,pout,rout,lo,hi,indd,length,pp[1024], nind,flag;
	int pin,rin,pind[8],ntot,nx,pp3[1024];
	short *sp1;
	FILE *input, *output, *filein;
	int *f, size;
	double prob,entro;

	nind=0;
	input = fopen(input_filename,"r");
	output = fopen(output_filename,"w");

						/* get input header from input*/
/*
	rewind(input);
	fread(&ncol,4,1,input);	
	fread(&nline,4,1,input);	
*/
	size = ncol*nline;

	p1=(int*)calloc(size,sizeof(int));	/* allocate and init to 0 */
	sp1=(short*)calloc(size,sizeof(short));

/***************************************/
/*	DECODEUR */
/***************************************/


	ntot = (size+Long-1)/Long;
	pn = (int *) calloc(ntot*Long,sizeof(int));
	nind = 0;
	pin = 32;

	while( nind<ntot )
	{
		uvld(pp,Long,input,&pin,&rin);
		for(i=0; i<Long; i++) pn[nind*Long+i]=pp[i];
		nind+=1;
	}
	fclose(input);

	length=0;
	for(i=0; i<nline; i++)
		for(j=0; j<ncol; j++)
		{
			dmin=1000;d1=1000;d2=1000;d3=1000;d4=1000;
			if(j>1)d1=abs(p1[i*ncol+j-1]-p1[i*ncol+j-2]);
			if(d1<dmin){dmin=d1;pred=1;}
			if(i>1)d2=abs(p1[(i-1)*ncol+j]-p1[(i-2)*ncol+j]);
			if(d2<dmin){dmin=d2;pred=2;}
			if(i>1&&j>1)d3=abs(p1[(i-1)*ncol+j-1]-p1[(i-2)*ncol+j-2]);
			if(d3<dmin){dmin=d3;pred=3;}
			if(i>1&&j<(ncol-2))d4=abs(p1[(i-1)*ncol+j+1]-p1[(i-2)*ncol+j+2]);
			if(d4<dmin){dmin=d4;pred=4;}
			if(dmin>=1000) pred=0;
			if(pred==1){p1[i*ncol+j]=pn[length]+p1[i*ncol+j-1];}
			if(pred==2){p1[i*ncol+j]=pn[length]+p1[(i-1)*ncol+j];}
			if(pred==3){p1[i*ncol+j]=pn[length]+p1[(i-1)*ncol+j-1];}
			if(pred==4){p1[i*ncol+j]=pn[length]+p1[(i-1)*ncol+j+1];}
			if(pred==0){if(j>0) p1[i*ncol+j]=pn[length]+p1[i*ncol+j-1];
			else p1[i*ncol+j]=pn[length];}
			if(skipdpcm==1) p1[i*ncol+j]=pn[length];
			length+=1;
		}

	rewind(output);
/*
	fwrite(&ncol,4,1,output);	
	fwrite(&nline,4,1,output);	
*/
	for(i=0; i<size; i++) sp1[i] = (short)p1[i];
	fwrite(sp1,sizeof(short),size,output);
	fclose(output);
}
