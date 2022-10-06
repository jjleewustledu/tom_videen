/*$Id: matnumx.c,v 1.2 1995/01/30 16:22:32 ty7777 Exp $*/
/*$Log: matnumx.c,v $
 * Revision 1.2  1995/01/30  16:22:32  ty7777
 * Added program version.
 **/

/* @(#)matnumx.c	1.2 10/26/92 */

#include "matrix.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/matnumx.c,v 1.2 1995/01/30 16:22:32 ty7777 Exp $";

main( argc, argv)
  int argc;
  char **argv;
{
	int a,b,vflag;
	int matnum;
	struct Matval mv;
	int frame,dflag;

	a=b=vflag=0;
	frame=1;
	dflag=0;
	if (argc>1)
	  sscanf( argv[1], "%d,%d,%d,%d,%d", &a, &b, &vflag, &frame, &dflag);
	matnum = matnumfx( a, b, vflag, frame, dflag);
	mat_numdoc( matnum, &mv);
	printf("%d,%d,%d,%d,%d\n",
	  mv.frame, mv.plane, mv.gate, mv.data, mv.bed);
}
