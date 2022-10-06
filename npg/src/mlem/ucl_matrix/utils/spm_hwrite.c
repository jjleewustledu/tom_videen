/*	===========================================================================
 *	Module:			spm_hwrite.c
 *	Date:			15-Apr-97
 *	Author:			Merence Sibomana <Sibomana@topo.ucl.ac.be>
 *	Description:	writes the spm header from command line in the form
 *      [DIM VOX SCALE TYPE ORIGIN DESCRIP]
 *	==========================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analyze.h"			 /* dsr */

static void usage() {
	fprintf(stderr,
	"usage : spm_hwrite filename dim vox scale type origin descrip\n");
	exit(1);
}

main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* header for ANALYZE */
	FILE           *fd;			 /* file for ANALYZE hdr */
	char            tmp[80];
	short spm_origin[3];
	int xdim, ydim, zdim;
	float dx,dy,dz;
	float scale_factor;
	int data_type;


	if (argc < 7) usage();

/*
 * Create spm hdr file
 */
    if (sscanf(argv[2],"%d,%d,%d",&xdim,&ydim,&zdim) != 3) usage();
	memset(&hdr,0,sizeof(hdr));
	hdr.hk.sizeof_hdr = sizeof (struct dsr); /* required by developers */
	strcpy(hdr.hk.data_type,"dsr");
	hdr.hk.extents = 16384;          /* recommended by developers  */
	hdr.hk.regular = 'r';            /* required by developers */
	hdr.dime.dim[0] = 4;             /* typically only 4 dimensions  */
	hdr.dime.dim[4] = 1;             /* only one volume typically  */
	strcpy(hdr.dime.vox_units,"mm");
	hdr.dime.dim[1] = xdim; hdr.dime.dim[2] = ydim;
	hdr.dime.dim[3] = zdim;
	if (sscanf(argv[3],"%g,%g,%g",&dx,&dy,&dz) != 3) usage();
	hdr.dime.pixdim[1] = dx;
	hdr.dime.pixdim[2] = dy;
	hdr.dime.pixdim[3] = dz;
	if (sscanf(argv[4],"%g",&scale_factor) != 1) usage();
	hdr.dime.funused1 = scale_factor;
	if (sscanf(argv[5],"%d",&data_type) != 1) usage();
	hdr.dime.datatype = data_type;
    if (sscanf(argv[6],"%d,%d,%d",&xdim,&ydim,&zdim) != 3) usage();
	spm_origin[0] = xdim; spm_origin[1] = ydim; spm_origin[2] = zdim;
	if (argc > 7) strncpy (hdr.hist.descrip, argv[7], 79);
	if (data_type == 2) hdr.dime.bitpix = 8;
	else hdr.dime.bitpix = 16;				/* assume short */

/*
 * swap bytes if necessary
 */
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
		swab((char*)spm_origin,hdr.hist.originator,3*sizeof(short));
	} else memcpy(hdr.hist.originator,spm_origin,3*sizeof(short));

/*
 * write Analyze hdr file
 */
	if ((fd=fopen(argv[1],"w")) == NULL ||
		fwrite(&hdr,sizeof(struct dsr),1,fd) < 0) {
		perror(argv[1]);
		exit (1);
	}
	fclose (fd);
	exit (0);
}
