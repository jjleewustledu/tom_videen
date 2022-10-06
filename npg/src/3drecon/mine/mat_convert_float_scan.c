/*$Id: mat_convert_float_scan.c,v 1.2 1995/01/30 17:03:15 ty7777 Exp $*/
/*$Log: mat_convert_float_scan.c,v $
 * Revision 1.2  1995/01/30  17:03:15  ty7777
 * Added program version.
 **/

/* @(#)mat_convert_float_scan.c	1.1 4/25/91 */

#include "matrix.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/mat_convert_float_scan.c,v 1.2 1995/01/30 17:03:15 ty7777 Exp $";

mat_convert_float_scan( ssh, fdata, idata)
  Scan_subheader *ssh;
  float *fdata;
  short int *idata;
{
	int i, nvals, tot;
	float fmax, scale;

	nvals = ssh->dimension_1*ssh->dimension_2;
	fmax = *fdata;
	for (i=0; i<nvals; i++)
	  if (fdata[i] > fmax) fmax = fdata[i];
	scale = 1.0;
	if (fmax > 0.0) scale = 32767./fmax;
	tot = 0;
	for (i=0; i<nvals; i++)
	{
	  idata[i] = scale*fdata[i];
	  tot += idata[i];
	}
	ssh->scan_min = 0;
	ssh->scan_max = fmax*scale;
	ssh->net_trues = tot;
	ssh->scale_factor = 1.0/scale;
}
