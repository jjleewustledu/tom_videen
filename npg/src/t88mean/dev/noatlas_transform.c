/*	$Log: noatlas_transform.c,v $
 * Revision 2.0  1996/05/10  21:59:07  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.2  1995/11/10  18:48:10  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/08/31  21:59:21  tom
 * Initial revision
 *
	$Header: /home/npggw/tom/src/t88mean/RCS/noatlas_transform.c,v 2.0 1996/05/10 21:59:07 tom Exp $
 */
/*	========================================================================
 *	Module:			noatlas_transform.c
 *	Date:			31-Aug-95
 *	Author:			Tom Videen
 *	Description:	Sum up images with no transform
 *	========================================================================
 */
#include <stdio.h>
#include <myvalues.h>				 /* BOOLEAN, TRUE, FALSE */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/noatlas_transform.c,v 2.0 1996/05/10 21:59:07 tom Exp $";

#endif

/*	-----------------------
 *	Function noatlas_transform
 *	-----------------------
 */
void            noatlas_transform (pet, pdim, atl, adim, sd, weight, totwt, mask, use_mask, compute_sd)
	float        ***pet;			 /* PET image array */
	float          *pdim;			 /* pet image dimensions */
	float        ***atl;			 /* atlas image array */
	float          *adim;			 /* atlas image dimensions */
	float        ***sd;			 	 /* sum of squares */
	float           weight;
	float        ***totwt;			 /* number of images contributing to each pixel in atlas array */
	float        ***mask;			 /* mask for PET images */
	BOOLEAN         use_mask;		 /* if true, only include pixels in mask */
	BOOLEAN         compute_sd;		 /* if true, compute sum of squares */

{
	float           val;			 /* temporary value computed for ATL */
	long            x, y, z;		 /* indices to ATL */
	long           	x0, y0, z0;		 /* offset to PET array */
	long           	xmin, ymin, zmin;
	long           	xmax, ymax, zmax;

/*	PET image and Atlas image may have different dimensions */

	if (adim[0] > pdim[0]) {
		xmin = (adim[0] - pdim[0])/2;
		xmax = xmin + pdim[0] - 1;
		x0 = xmin - 1;
	} else {
		xmin = 1;
		xmax = adim[0];
		x0 = 0;
	}
	if (adim[1] > pdim[1]) {
		ymin = (adim[1] - pdim[1])/2;
		ymax = ymin + pdim[1] - 1;
		y0 = ymin - 1;
	} else {
		ymin = 1;
		ymax = adim[1];
		y0 = 0;
	}
	if (adim[2] > pdim[2]) {
		zmin = (adim[2] - pdim[2])/2;
		zmax = zmin + pdim[2] - 1;
		z0 = zmin - 1;
	} else {
		zmin = 1;
		zmax = adim[2];
		z0 = 0;
	}

	fprintf (stdout, "z =   ");
	for (z = zmin; z <= zmax; ++z) {
		fprintf (stdout, "%c%c%2d", 8, 8, z + 1);
		fflush (stdout);
		for (y = ymin; y <= ymax; ++y) {
			for (x = xmin; x <= xmax; ++x) {
				if (use_mask && !mask[z][y][x]);
				else {
					val = pet[z-z0][y-y0][x-x0];
					atl[z][y][x] += val * weight;
					totwt[z][y][x] += weight;
					if (compute_sd)
						sd[z][y][x] += val * val;
				}
			}
		}
	}
	fprintf (stdout, "\n");
}
