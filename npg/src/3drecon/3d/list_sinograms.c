/*$Id: list_sinograms.c,v 1.2 1995/01/30 16:21:33 ty7777 Exp $*/
/*$Log: list_sinograms.c,v $
 * Revision 1.2  1995/01/30  16:21:33  ty7777
 * Added program version.
 **/

/* @(#)list_sinograms.c	1.8 6/2/92 */

/*
	This programs generates a list of sinograms for 3d reconstruction
	for various scanner configurations. (Default for 16-ring 953/921).
*/

#include <math.h>
#include "matrix.h"

static char sccsid [] = "@(#)list_sinograms.c	1.8 6/2/92 Copyright 1992 CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/list_sinograms.c,v 1.2 1995/01/30 16:21:33 ty7777 Exp $";

#define NUM_RINGS 16
#define RING_SPACING 0.675
#define RING_DIAMETER 76.0

#define max(a,b) ((a)>(b)?(a):(b))

	static char *prgnam;

main( argc, argv)
  int argc;
  char **argv;
{
	int rdmax, rd, matnum, a, b;
	float zoff, phi;
	int i, j, k, nslice, n_synt, mat_type, n_meas;
	int num_rings, nx, nx2, nr2, verbose=0;
	float ring_spacing, ring_diameter;
	char *ptype;
	int c;
	extern char *optarg;
	extern int optind, opterr;

	if (argc < 2)
	{
	  fprintf( stderr, "%s\n", sccsid);
	  crash( "usage: %s rdmax[,vtype,num_rings,ring_spacing,ring_diameter]\n", argv[0]);
	}
	rdmax = 5;
	n_synt = n_meas = mat_type = 0;
	num_rings = NUM_RINGS;
	ring_spacing = RING_SPACING;
	ring_diameter = RING_DIAMETER;
	while ((c=getopt( argc, argv, "v")) != -1)
	  switch(c)
	  {
		case 'v':	verbose = 1;
				break;
	  }
	sscanf( argv[optind++], "%d,%d,%d,%f,%f",
	  &rdmax, &mat_type, &num_rings, &ring_spacing, &ring_diameter);
	nslice = num_rings*2-1;
	for (rd=-rdmax; rd<=rdmax; rd++)
	{
	  nx = nslice + 2*abs(rd);
	  nx2 = nx/2;
	  nr2 = num_rings/2;
	  a = nr2-rd-1-nx2/2;
	  b = nr2+rd-nx2+nx2/2;
	  phi = 180.*atan( 2*rd*ring_spacing/ring_diameter)/M_PI;
	  printf("*** rd %d, nx %d, phi %g ***\n", rd, nx, phi);
	  for (i=0; i<nx; i++)
	  {
	    zoff = (a+b-num_rings+1)*ring_spacing/2.0;
	    if (a<0 || b<0 || a+1>num_rings || b+1>num_rings)
	    {
		n_synt++;
		printf("%d,%d,%g,%g,%d,synthetic %d,%d\n",
		  rd, i, phi, zoff, n_synt, a, b);
	    }
	    else
	    {
		n_meas++;
		if (mat_type < 0)
		  matnum = mat_numcod( -mat_type, i+1, 1, 0, 0);
		else
		  matnum = matnumx( a, b, mat_type);
		ptype = "measured";
		if (a==b) ptype = "measured-direct";
		printf("%d,%d,%g,%g,%d,%s %d,%d\n",
		  rd, i, phi, zoff, matnum, ptype, a, b);
	    }
	    if ((nx2+i)%2==0) a++;
	    else b++;
	  }
	}
	if (verbose)
	  fprintf( stderr, "%s: %d measured + %d synthetic = %d total\n",
	    argv[0], n_meas, n_synt, n_meas+n_synt);
}
