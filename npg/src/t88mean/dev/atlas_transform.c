/* $Log: atlas_transform.c,v $
 * Revision 3.6  1996/05/10  22:00:07  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 3.5  1995/11/10  18:47:52  tom
 * remove petutil parent directory from include statements
 *
 * Revision 3.4  1995/01/19  19:17:03  tom
 * weights
 *
 * Revision 3.3  1994/10/03  16:17:13  tom
 * Separate functions atlas_scalers and matrix_transform;
 *
 * Revision 3.2  1994/09/14  14:40:24  tom
 * Allow > 49 slices in atlas and any z-origin
 *
 * Revision 3.1  1994/05/18  15:53:30  tom
 */
/*	========================================================================
 *	Module:			atlas_transform.c
 *	Date:			19-Aug-93
 *	Author:			Tom Videen
 *	Description:	Transform a PET image into Atlas space
 *
 *  Input:
 *		pet		-	PET image;
 *		pdim	-	PET image dimensions;
 *		adim	-	atlas image dimensions;
 *		weight	-	weighting factor for this image;
 *		mask	-	masking image;
 *		map		-	conversion matrix Atlas -> PET
 *		use_mask -	if true, only include pixels in mask;
 *		compute_sd - if true, compute sum of squares;
 *      first_slice, last_slice -   range of slices from which an x-scaler may be accepted;
 *
 *  Output:
 *		atl		-	atlas image;
 *		sd		-	sum-of-squares image;
 *		totwt	-	total weight of images contributing to each pixel in atlas image;
 *
 *	History:
 *		20-Aug-93	Modified atlas_trans.c; center and angle are passed in
 *		24-Aug-93	Modified to include use_pet to compute y-scaler from PET
 *		30-Sep-93	Modified to compute running sum and sd;
 *					(pass additional parameters: sd, compute_sd)
 *		04-Oct-93	Eliminate atl = UNDEFINED; atl is now altered only when it is defined;
 *		14-Oct-93	Angles correspond with those returned from imgrot.
 *		28-Apr-94   added first_slice and last_slice to limit PET slices included
 *		29-Apr-94   subtract 10 cm from atlas.ydim[atlas.z0] when use_pet = TRUE
 *		10-May-94   corrected order of matrix concatenation;
 *					eliminate use_pet option for y-width
 *		13-Sep-94	use atlas.nz for loop through atlas array;
 *		20-Sep-94	if no PET slice contains the AC-PC plane, compute an x-scaling factor
 *					based on the nearest slice to it
 *		22-Sep-94	Remove as separate functions: atlas_scalers and matrix_transform
 *		05-Mar-96	Convert indexing to Numerical Recipes standard (1-based).
 *	========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <nr.h>
#include <nrutil.h>
#include <myvalues.h>		 /* BOOLEAN, TRUE, FALSE */
#include <matrixlib.h>		 /* vector_mult */

#define TOLERANCE 0.001		 /* max distance from a slice for which to use 2-D interpolation */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/atlas_transform.c,v 3.6 1996/05/10 22:00:07 tom Exp $";
#endif

/*	-----------------------
 *	Function atlas_transform
 *	-----------------------
 */
void            atlas_transform (pet, pdim, atl, adim, sd, weight, totwt, mask, map,
				                 use_mask, compute_sd, first_slice, last_slice)
	float        ***pet;		 /* PET image array */
	int            *pdim;		 /* pet image dimensions */
	float        ***atl;		 /* atlas image array */
	int            *adim;		 /* atlas image dimensions */
	float        ***sd;			 /* sum of squares */
	float           weight;
	float        ***totwt;		 /* num of images contributing to each pixel in atl array */
	float        ***mask;		 /* mask for PET images */
	float         **map;		 /* conversion matrix Atlas -> PET   */
	BOOLEAN         use_mask;	 /* if true, only include pixels in mask */
	BOOLEAN         compute_sd;	 /* if true, compute sum of squares */
	int             first_slice;
	int             last_slice;

{
	float          *a;			 /* vector in ATL-space; a[4] */
	float          *p;			 /* vector in PET-space; p[4] */
	float           wx, wy, wz;	 /* weighting factors for x, y, z */
	float           val;		 /* temporary value computed for ATL */
	float           temp;		 /* local real no. for z1 */
	float           zmin, zmax;

	long            xa, ya, za;		 /* indices to atl */
	long            x1, y1, z1;		 /* indices to PET */
	long            x2, y2, z2;		 /* indices to PET */

/*
 *	Initializations
 */
	if (last_slice < 1 || last_slice > pdim[2])
		last_slice = pdim[2];
	zmin = (float) first_slice - TOLERANCE;
	zmax = (float) last_slice + TOLERANCE;
	a = vector (1, 4);
	p = vector (1, 4);

/*
 *	LOOP for ALL ATLAS PIXELS:	Translation, Rotation, and Scaling
 *	--------------------------------------------------------------
 */

	fprintf (stdout, "z =   ");
	a[4] = 1;
	for (za = 1; za <= adim[2]; ++za) {
		fprintf (stdout, "%c%c%2d", 8, 8, za);
		fflush (stdout);
		a[3] = (float) (za);
		for (ya = 1; ya <= adim[1]; ++ya) {
			a[2] = (float) (ya);
			for (xa = 1; xa <= adim[0]; ++xa) {
				a[1] = (float) (xa);

/*
 *	Find coordinates of vector A in PET space (MAP*A --> P)
 */
				vector_mult (map, a, p, 4, 4);
				x1 = (long) p[1];
				y1 = (long) p[2];
				z1 = (long) p[3];
				x2 = x1 + 1;
				y2 = y1 + 1;
				z2 = z1 + 1;
				temp = (float) z1;
/*
 *	If outside PET array, ATL is not altered (empty statement follows if)
 */
				if (p[3] > zmax || p[3] < zmin || y2 > pdim[1] || y1 < 1 || x2 > pdim[0] || x1 < 1);
/*
 *	If P is within tolerance of a PET slice, use 2-D interpolation
 */
				else if (fabs (p[3] - temp) <= TOLERANCE) {
					if (use_mask && (!mask[z1][y1][x1] || !mask[z1][y2][x1] ||
							 !mask[z1][y1][x2] || !mask[z1][y2][x2]));
					else {
						wx = p[1] - (float) x1;
						wy = p[2] - (float) y1;
						val = (1 - wx) * (1 - wy) * pet[z1][y1][x1] +
							wx * (1 - wy) * pet[z1][y1][x2] +
							(1 - wx) * wy * pet[z1][y2][x1] +
							wx * wy * pet[z1][y2][x2];
						atl[za][ya][xa] += val * weight;
						totwt[za][ya][xa] += weight;
						if (compute_sd)
							sd[za][ya][xa] += val * val;
					}
				}
/*
 *  If P is not within TOLERANCE of slice first_slice or last_slice (zmin or zmax),
 *	then p[3] must be greater than first_slice. and less than last_slice
 */
				else if (z1 < first_slice || z2 > last_slice);
/*
 *	Otherwise use 3-D interpolation
 */
				else {
					if (use_mask && (!mask[z1][y1][x1] || !mask[z1][y1][x2] ||
							 !mask[z1][y2][x1] || !mask[z1][y2][x2] || !mask[z2][y1][x1] ||
							 !mask[z2][y1][x2] || !mask[z2][y2][x1] || !mask[z2][y2][x2]));
					else {
						wx = p[1] - (float) x1;
						wy = p[2] - (float) y1;
						wz = p[3] - (float) z1;
						val = (1 - wx) * (1 - wy) * (1 - wz) * pet[z1][y1][x1] +
							wx * (1 - wy) * (1 - wz) * pet[z1][y1][x2] +
							(1 - wx) * wy * (1 - wz) * pet[z1][y2][x1] +
							wx * wy * (1 - wz) * pet[z1][y2][x2] +
							(1 - wx) * (1 - wy) * wz * pet[z2][y1][x1] +
							wx * (1 - wy) * wz * pet[z2][y1][x2] +
							(1 - wx) * wy * wz * pet[z2][y2][x1] +
							wx * wy * wz * pet[z2][y2][x2];
						atl[za][ya][xa] += val * weight;
						totwt[za][ya][xa] += weight;
						if (compute_sd)
							sd[za][ya][xa] += val * val;
					}
				}		 /* end vector  */
			}			 /* end row    */
		}				 /* end column */
	}					 /* end plane  */
	fprintf (stdout, "\n");
	free_vector (a, 1, 4);
	free_vector (p, 1, 4);
}
