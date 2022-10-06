/* $Log: atlas_scalers.c,v $
 * Revision 2.1  1996/10/11  14:43:43  tom
 * added zref for atlas.xdim, atlas.ydim
 *
 * Revision 2.0  1996/05/10  21:57:57  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.2  1995/11/10  18:47:50  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1994/10/03  16:17:13  tom
 * Initial revision
 *
 */
/*	========================================================================
 *	Module:			atlas_scalers.c
 *	Date:			22-Sep-94
 *	Author:			Tom Videen
 *	Description:	Determine the transformation scalers from PET -> Atlas.
 *
 *  Input:
 *      sxr 	-   sxr file structure;
 *		atlas	-	brain atlas structure;
 *		center	-	reference point in PET image which has a known displacement from the atlas;
 *		angle	-	transformation angles PET -> atlas;
 *		last_slice	-	last slice from which an x-scaler may be accepted; (Unused)
 *
 *  Output:
 *		scale	-	scalers for PET -> atlas;
 *
 *	Return:
 *		OK or ERROR
 *
 *	History:
 *		22-Sep-94	separated module from atlas_transform in t88mean
 *	========================================================================
 */
#include <stdio.h>
#include <math.h>
#include <nr.h>
#include <nrutil.h>
#include <imgutil.h>		 /* SXR_DATA */
#include <myvalues.h>		 /* BOOLEAN, TRUE, FALSE */
#include <matrixlib.h>		 /* vector_mult */
#include <atlas.h>			 /* BRAIN_ATLAS */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/atlas_scalers.c,v 2.1 1996/10/11 14:43:43 tom Exp $";
#endif

/*	-----------------------
 *	Function atlas_scalers
 *	-----------------------
 */

int             atlas_scalers (sxr, atlas, center, angle, scale, last_slice, zorigin)
	SXR_DATA        sxr;
	BRAIN_ATLAS     atlas;
	float          *center;
	float          *angle;
	float          *scale;
	int            *last_slice;

{
	int             matrix_transform ();
	float         **map;		 /* conversion matrix Atlas -> PET	 */
	float          *a;			 /* vector in ATL-space; a[4] */
	float          *p;			 /* vector in PET-space; p[4] */
	float           x_width;	 /* pet width for atlas 0 */
	float           a_width;	 /* atlas width for pet slice zmin or zmax */
	float           f;			 /* weighting factor */
	float           pz;			 /* PET z-location for a vector defined in atlas space */
	int             zmin, zmax;	 /* range of PET slices permitted for computing x-scaler */
								 /* (indexed from 1) */
	int             i;
	BOOLEAN         not_in_brain;

/*
 *	Initializations
 */

	if (*last_slice == 1 || *last_slice > sxr.number_slices)
		*last_slice = sxr.number_slices;
	x_width = 0;
	if (sxr.number_slices >= 31) {
		zmin = 3;
		zmax = sxr.number_slices - 2;
	} else {
		zmin = 1;
		zmax = sxr.number_slices;
	}
	if ((map = matrix (1, 4, 1, 4)) == NULL) {
		fprintf (stderr, "ERROR [atlas_scaler]: Cannot malloc memory for matrix\n");
		return (ERROR);
	}
/*
 *	Calculate y- and z-scalers from sxr file
 */

	scale[0] = sxr.xscale;
	scale[1] = sxr.magnification_factor * sxr.apxray / atlas.ydim[atlas.zref];
	scale[2] = sxr.magnification_factor * sxr.zxray / atlas.zdim;

/*
 *	If there is an x-scaler in the sxr file, use it
 */

	if (scale[0] > 0) {
		free_matrix (map, 1, 4, 1, 4);
		return (OK);
	}
/*
 *	If no x-scaler in sxr file, then it must be estimated:
 *	Get the transformation matrix MAP for given translation, rotations, and y- and z-scalers
 */

	if ((matrix_transform (sxr, atlas, center, angle, scale, map)) == ERROR)
		return (ERROR);

/*
 *	Determine the PET slice which intersects the ac-pc plane at y = 0.
 *	Compute X width and Scaling for X using the PET slice (interpolated if necessary).
 *	p[] is the atlas origin tranformed into PET coordinates; pz is its z-coordinate;
 */

	a = vector (1, 4);
	p = vector (1, 4);
	a[1] = (float) (atlas.x0);
	a[2] = (float) (atlas.y0);
	a[3] = (float) (atlas.z0);
	a[4] = 1;
	vector_mult (map, a, p, 4, 4);
	pz = p[3];

/*
 *	NOTE:  sxr.rlpetdim is indexed starting with 0.
 *	Everything else is referenced starting with 1:
 *		(zmin, zmax, sxr.xdim, sxr.ydim, atlas.z0, a, p)
 */

/*
 *	If all PET slices are below the AC-PC plane, take the most superior PET slice
 *	and determine which atlas slices it falls between; then compute the x-scaler
 *	from this PET slice and the interpolated width of the atlas slices.
 */
 
	not_in_brain = TRUE;
	if (pz <= zmin) {
		x_width = sxr.rlpetdim[zmin-1] * sxr.pixel_size;
		for (i = atlas.z0; i < atlas.nz; i++) {
			a[3] = (float) (i);
			vector_mult (map, a, p, 4, 4);
			pz = p[3];
			if (pz >= zmin) {
				f = pz - zmin;
				a_width = ((1 - f) * atlas.xdim[i]) + (f * atlas.xdim[i - 1]);
				scale[0] = x_width / a_width;
				not_in_brain = FALSE;
				break;
			}
		}

/*
 *	If all PET slices are above the AC-PC plane, take the most inferior PET slice
 *	and determine which atlas slices it falls between; then compute the x-scaler
 *	from this PET slice and the interpolated width of the atlas slices.
 */

	} else if (pz >= (float) zmax) {
		x_width = sxr.rlpetdim[zmax-1] * sxr.pixel_size;
		for (i = atlas.z0 - 1; i >= 0; i--) {
			a[3] = (float) (i);
			vector_mult (map, a, p, 4, 4);
			pz = p[3];
			if (pz <= zmax) {
				f = zmax - pz;
				a_width = ((1 - f) * atlas.xdim[i + 1]) + (f * atlas.xdim[i]);
				scale[0] = x_width / a_width;
				not_in_brain = FALSE;
				break;
			}
		}

/*
 *	Compute x-scaler using interpolated PET slice width corresponding to the AC-PC plane
 */

	} else {
		for (i = zmin + 1; i <= zmax; i++) {
			if (pz <= (float) i) {
				f = pz - (float) (i - 1);
				x_width = ((1 - f) * sxr.rlpetdim[i-2]) + (f * sxr.rlpetdim[i-1]);
				scale[0] = x_width * sxr.pixel_size / atlas.xdim[atlas.zref];
				not_in_brain = FALSE;
				break;
			}
		}
	}
	if (not_in_brain) {
		printf ("ERROR: No PET slice found within the Atlas Brain\n");
		scale[0] = 0;
		free_vector (a, 1, 4);
		free_vector (p, 1, 4);
		free_matrix (map, 1, 4, 1, 4);
		return (ERROR);
	} else {
		free_vector (a, 1, 4);
		free_vector (p, 1, 4);
		free_matrix (map, 1, 4, 1, 4);
		return (OK);
	}
}
