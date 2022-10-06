/* $Log: image_scale.c,v $
 * Revision 2.0  1996/05/10  21:58:54  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.17  1995/11/10  18:48:02  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.16  1995/02/08  21:06:25  tom
 * fix smg_include:n=
 *
 * Revision 1.15  1995/01/19  19:17:25  tom
 * weights
 *
 * Revision 1.14  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.13  1994/05/24  22:31:08  tom
 * nmask fixed
 *
 * Revision 1.12  1994/05/24  17:11:54  tom
 * nmask fixed to mask
 *
 * Revision 1.11  1994/05/18  15:53:30  tom
 * changed $Id: image_scale.c,v 2.0 1996/05/10 21:58:54 tom Exp $ to $Header: /home/npggw/tom/src/t88mean/RCS/image_scale.c,v 2.0 1996/05/10 21:58:54 tom Exp $
 * */
/*	========================================================================
 *	Module:			image_scale.c
 *	Date:			10-Aug-93
 *	Author:			Tom Videen
 *	Description:	Scale image by smg_divisor and mask pixels using totwt.
 *					Compute sd image.
 *
 *	Inputs:
 *		a[nz][ny][nx]   - atlas image array
 *		sd[nz][ny][nx]  - sum-of-squares image array
 *		totwt[nz][ny][nx] - array of total weights per pixel
 *		dim[3]			-	dimensions of arrays
 *		maxwt           -   total weighted number of images processed
 *		smg_divisor     -   "global" or "local" or "none"
 *		smg_included    -   "all", "sampled" or "n= "
 *		compute_sd      -   TRUE -> compute sd
 *
 *	Output:
 *		a[nz][ny][nx]   - atlas image array (scaled and masked)
 *		sd[nz][ny][nx]  - sd image array
 *
 *	Algorithm:
 *
 *	1)  If (compute_sd) then compute standard deviation image, sd from:
 *			sd = sqrt ( (sumsqrs - ((sum * sum) / n)) / (n - 1) )
 *
 *	    where:
 *	        sumsqrs = sd[i][j][k] (on input)
 *	        sum     = a[i][j][k]
 *	        n       = totwt[i][j][k]
 *
 *	    sd = 0 for n <= 1 and when sqrt of negative value
 *
 *	2)  If (sum_included = all) then
 *	        If (smg_divisor = global) then divide by total number of images
 *	           a = a / maxwt
 *	        else divide by total weight contributing to each pixel
 *	            a = a / totwt
 *	    Else if (smg_included = n=?) then divide by number of images contributing
 *	        to each pixel for all pixels which totwt >= the specified n
 *	            a = a / totwt  where totwt > ?; otherwise a = 0
 *	    Else divide by total number of images for fully sampled pixels
 *	            a = a / totwt where totwt = maxwt; otherwise a = 0
 *
 *	History:
 *		11-Aug-93:	Compute sd image (sd) for all pixels within a mask (if a mask is specified)
 *		30-Sep-93:	Correct algorithm for computing sd
 *		12-Apr-94	added nmask to allow selective masking of output image to n subjects
 *		24-May-94	fixed nmask  (it was not zeroing outside of the mask)
 *	========================================================================
 */
#include <stdio.h>
#include <math.h>
#include <imgutil.h>
#include <atlas.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/image_scale.c,v 2.0 1996/05/10 21:58:54 tom Exp $";
#endif

/*	-----------------------
 *	Function image_scale
 *	-----------------------
 */
void            image_scale (a, totwt, dim, maxwt, smg_divisor, smg_included, compute_sd, sd)
	float        ***a, ***sd;
	float        ***totwt;
	long           *dim;
	float           maxwt;
	char           *smg_divisor;
	char           *smg_included;
	BOOLEAN         compute_sd;

{
	long            i, j, k;
	int             nmask;	 /* optional; specifies min number of subjects for
						  	  * each non-zero pixel */
	float           f;
	float           n;
	float           sum;
	float           sumsqrs;		 /* sum of the squares */
	float           ssqr;			 /* sigma squared */
	float			xmax, xmin;
	float			wmax;

/*
 *	Compute Standard Deviation image from sum-of-squares (sd), sum (a), and n (totwt)
 */

	if (compute_sd) {
		for (i = 1; i <= dim[2]; ++i) {
			for (j = 1; j <= dim[1]; ++j) {
				for (k = 1; k <= dim[0]; ++k) {
					n = totwt[i][j][k];
					sum = a[i][j][k];
					sumsqrs = sd[i][j][k];
					if (n > 1 && sumsqrs > 0) {
						ssqr = (sumsqrs - ((sum * sum) / n)) / (n - 1);
						if (ssqr > 0)
							sd[i][j][k] = sqrt (ssqr);
						else
							sd[i][j][k] = 0;
					} else
						sd[i][j][k] = 0;
				}
			}
		}
	}
/*
 *	All pixels; divide by total weighted number of images
 */

	if (strcmp (smg_included, "all") == 0) {
		if (strcmp (smg_divisor, "global") == 0) {
			f = 1. / maxwt;
			for (i = 1; i <= dim[2]; ++i) {
				for (j = 1; j <= dim[1]; ++j) {
					for (k = 1; k <= dim[0]; ++k) {
						a[i][j][k] *= f;
					}
				}
			}

/*
 *	All pixels; divide by weighted number of images contributing to each pixel
 */

		} else if (strcmp (smg_divisor, "local") == 0) {
			for (i = 1; i <= dim[2]; ++i) {
				xmin = 0;
				xmax = 0;
				wmax = 0;
				for (j = 1; j <= dim[1]; ++j) {
					for (k = 1; k <= dim[0]; ++k) {
						if (totwt[i][j][k] > 0) {
							a[i][j][k] /= totwt[i][j][k];
							wmax = totwt[i][j][k];
						}
						if (a[i][j][k] > xmax)
							xmax = a[i][j][k];
						else if (a[i][j][k] < xmin)
							xmin = a[i][j][k];
					}
				}
/*
				printf ("Z= %d  Wt= %f  Max= %f  Min= %f\n", i, wmax, xmax, xmin);
 */
			}
		}
/* Not that smg_included = "all" and smg_divisor = "none" drops through with no processing */

/*
 *	All pixels which had at least nmask images contributing to them;
 *	divide by number of images contributing to each pixel
 */

	} else if (strncmp (smg_included, "n=", 2) == 0) {
		nmask = atoi (smg_included + 2);
		for (i = 1; i <= dim[2]; ++i) {
			for (j = 1; j <= dim[1]; ++j) {
				for (k = 1; k <= dim[0]; ++k) {
					if (totwt[i][j][k] + .001 >= (float) nmask)
						a[i][j][k] /= totwt[i][j][k];
					else
						a[i][j][k] = 0;
				}
			}
		}

/*
 *	Only fully sampled pixels; divide by total weighted number of images
 */

	} else {
		f = 1. / maxwt;
		for (i = 1; i <= dim[2]; ++i) {
			for (j = 1; j <= dim[1]; ++j) {
				for (k = 1; k <= dim[0]; ++k) {
					if (fabs (totwt[i][j][k] - maxwt) < .001)
						a[i][j][k] *= f;
					else
						a[i][j][k] = 0;
				}
			}
		}
	}
}
