/*	===========================================================================
 *	Module:			spm_hread.c
 *	Date:			15-Apr-97
 *	Author:			Merence Sibomana <Sibomana@topo.ucl.ac.be>
 *	Description:	prints the spm header in the form
 *      [DIM VOX SCALE TYPE ORIGIN DESCRIP] on the standard output
 *	==========================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analyze.h"			 /* dsr */

static void swaw( from, to, length)
  short int from[], to[];
  int length;
{
    short int temp;
    int i;

    for (i=0;i<length; i+=2)
    {  temp = from[i+1];
       to[i+1]=from[i];
       to[i] = temp;
    }
}


main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* header for ANALYZE */
	FILE           *fd;			 /* file for ANALYZE hdr */
	char            tmp[80];
	short spm_origin[5];
	int xdim, ydim, zdim;


	if (argc < 2) {
		fprintf (stderr, "Usage: spm_hread ANALYZE_hdr\n");
		exit (1);
	}
/*
 * Read Analyze hdr file
 */

	if ((fd=fopen(argv[1],"r")) == NULL ||
		fread(&hdr,sizeof(struct dsr),1,fd) < 0) {
		perror(argv[1]);
		exit (1);
	}
	if (ntohs(1) != 1) {
		hdr.hk.sizeof_hdr = ntohl(hdr.hk.sizeof_hdr);
		hdr.hk.extents = ntohl(hdr.hk.extents);
		swab((char*)hdr.dime.dim,tmp,8*sizeof(short));
		memcpy(hdr.dime.dim,tmp,8*sizeof(short));
		hdr.dime.datatype = ntohs(hdr.dime.datatype);
		hdr.dime.bitpix = ntohs(hdr.dime.bitpix);
		swab((char*)hdr.dime.pixdim,tmp,8*sizeof(float));
		swaw(tmp,hdr.dime.pixdim,8*sizeof(float)/2);
		swab((char*)&hdr.dime.funused1,tmp,sizeof(float));
        swaw(tmp,&hdr.dime.funused1,sizeof(float)/2);
		hdr.dime.glmax = ntohl(hdr.dime.glmax);
		hdr.dime.glmin = ntohl(hdr.dime.glmin);
	}

	xdim = hdr.dime.dim[1]; ydim = hdr.dime.dim[2]; zdim = hdr.dime.dim[3];
    printf ("%d,%d,%d",hdr.dime.dim[1],hdr.dime.dim[2],
		hdr.dime.dim[3]);						/* image size */
    printf (" %g,%g,%g",hdr.dime.pixdim[1],hdr.dime.pixdim[2],
		hdr.dime.pixdim[3]);					/* voxel size */
	printf(" %g", hdr.dime.funused1);			/* scale factor */
	printf (" %d", hdr.dime.datatype); 			/* data type */
												/* check spm origin */
	if (ntohs(1) != 1) swab(hdr.hist.originator,(char*)spm_origin,10);
	else memcpy(spm_origin,hdr.hist.originator,10);
	if (spm_origin[0]>1 && spm_origin[1]>1 && spm_origin[2]>1 &&
		spm_origin[0]<xdim && spm_origin[1]<ydim && spm_origin[2]<zdim) {
	} else spm_origin[0] = spm_origin[1] = spm_origin[2] = 1; 	/* unknown origin */
	printf (" %d,%d,%d", spm_origin[0],spm_origin[1],spm_origin[2]);
	printf (" %s\n", hdr.hist.descrip);
	fclose (fd);
	exit (0);
}
