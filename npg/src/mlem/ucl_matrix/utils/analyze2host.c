/*	===============================================================================
 *	Module:			analyze2host.c
 *	Date:			11-Apr-97
 *	Author:			Sibomana@topo.ucl.ac.be
 *	Description:	Converts Analyze header to host dependent data
 *	===========================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analyze.h"			 /* dsr */

#define MAXSTR 256
#define TRANSVERSE '\000'
#define CORONAL    '\001'
#define SAGITTAL   '\002'
#define FAIL 1


main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* header for ANALYZE */
	FILE           *fd;			 /* file for ANALYZE hdr */
	char           *ANALYZE_hdr=NULL;	/* output Analyze header filename  */
	char           *ANALYZE_img=NULL;	/* output Analyze image filename */
	char			*p, *ANALYZE_base;
	short           xdim, ydim,zdim;		 /* pixel dimensions */
	char            tmp[80];
	char patient_id[12];
	short spm_origin[5];


/*
 *	Get command line arguments and initialize filenames:
 *	---------------------------------------------------
 */

	if (argc < 2) {
		fprintf (stderr,
			"Usage: analyze2host ANALYZE_header Host_Analyze_header\n");
		exit (FAIL);
	}

/*
 * Read Analyze hdr file
 */
	if ((fd = fopen (argv[1], "r")) == NULL) {
		perror(argv[1]);
		exit (FAIL);
	}

	if (fread(&hdr,sizeof(struct dsr),1,fd) < 0) {
		perror(argv[1]);
		exit (FAIL);
	}
	fclose(fd);
	if (ntohs(1) != 1) {
		hdr.hk.sizeof_hdr = ntohl(hdr.hk.sizeof_hdr);
		hdr.hk.extents = ntohl(hdr.hk.extents);
		swab(hdr.dime.dim,tmp,8*sizeof(short));
		memcpy(hdr.dime.dim,tmp,8*sizeof(short));
		hdr.dime.datatype = ntohs(hdr.dime.datatype);
		hdr.dime.bitpix = ntohs(hdr.dime.bitpix);
		swab(hdr.dime.pixdim,tmp,8*sizeof(float));
		swaw(tmp,hdr.dime.pixdim,8*sizeof(float)/2);
		swab(&hdr.dime.funused1,tmp,sizeof(float));
        swaw(tmp,&hdr.dime.funused1,sizeof(float)/2);
		hdr.dime.glmax = ntohl(hdr.dime.glmax);
		hdr.dime.glmin = ntohl(hdr.dime.glmin);
	}
	if ((fd = fopen (argv[2], "w")) == NULL) {
		perror(argv[2]);
		exit (FAIL);
	}
	if (fwrite(&hdr,sizeof(struct dsr),1,fd) < 0) {
		 perror(argv[2]));
		exit (FAIL);
	}
}
