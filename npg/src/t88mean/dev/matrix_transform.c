/* $Log: matrix_transform.c,v $
 * Revision 2.0  1996/05/10  21:59:01  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.2  1995/11/10  18:48:08  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1994/10/03  16:17:13  tom
 * Initial revision
 *
 */
/*	========================================================================
 *	Module:			matrix_transform.c
 *	Date:			22-Sep-94
 *	Author:			Tom Videen
 *	Description:	Create the transformation matrix from PET -> Atlas.
 *
 *  Input:
 *      sxr 	-   sxr file structure;
 *		atlas	-	brain atlas structure;
 *		center	-	reference point in PET image which has a known displacement from the atlas;
 *		angle	-	transformation angles PET -> atlas;
 *		scale	-	scalers for PET -> atlas;
 *
 *  Output:
 *		map		-	matrix to transform Atlas to PET coordinate system
 *
 *	Algorithm:
 *	1)  Compute the translation matrix to array origin:
 *		The atlas image is shifted an additional 1.8 cm anteriorly as its origin is not
 *		at the center of the brain:
 *		(17.2 / 2) - 7.0 = 1.8 cm  (based on horizontal section +1 or -1 in Tal 1988)
 *	2)  Compute the translation matrix to PET origin.
 *	3)  Compute rotation matrix about the matrix origin.
 *	4)  Compute scaling matrix  (Atlas dim --> PET dim).
 *	5)  Compute scaling matrix  (PET dim --> atlas dim).
 *	6)  Concatenate matrices and return.
 *
 *	History:
 *		22-Sep-94	separated module from atlas_transform in t88mean
 *	========================================================================
 */
#include <stdio.h>
#include <math.h>
#include <nr.h>
#include <nrutil.h>
#include <imgutil.h>				 /* SXR_DATA */
#include <matrixlib.h>				 /* matrix_mult */
#include <atlas.h>				 /* BRAIN_ATLAS */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/matrix_transform.c,v 2.0 1996/05/10 21:59:01 tom Exp $";
#endif

/*	-----------------------
 *	Function matrix_transform
 *	-----------------------
 */

int             matrix_transform (sxr, atlas, center, angle, scale, map)
	SXR_DATA        sxr;
	BRAIN_ATLAS     atlas;
	float           center[3];
	float           angle[3];
	float           scale[3];
	float         **map;		 /* conversion matrix Atlas -> PET */

{
	float         **ta;			 /* translation matrix:  Atlas origin -> array origin */
	float         **tp;			 /* translation matrix:  array origin -> PET origin */
	float         **rx, **ry, **rz;		 /* rotation matrices:  Atlas -> PET	 */
	float         **sa;			 /* scaling matrix:	PET -> Atlas	 */
	float         **sp;			 /* scaling matrix:	Atlas -> output array	 */
	float         **ma, **mb, **mc, **md, **me;	/* intermediate matrix products */
	int             i, j;

/*
 *	Initializations
 */

	ta = matrix (1, 4, 1, 4);
	tp = matrix (1, 4, 1, 4);
	rx = matrix (1, 4, 1, 4);
	ry = matrix (1, 4, 1, 4);
	rz = matrix (1, 4, 1, 4);
	sa = matrix (1, 4, 1, 4);
	sp = matrix (1, 4, 1, 4);
	ma = matrix (1, 4, 1, 4);
	mb = matrix (1, 4, 1, 4);
	mc = matrix (1, 4, 1, 4);
	md = matrix (1, 4, 1, 4);
	if ((me = matrix (1, 4, 1, 4)) == NULL) {
		fprintf (stderr, "ERROR [matrix_transform]: Cannot malloc memory for matrix\n");
		return (ERROR);
	}
	for (i = 1; i <= 4; ++i) {
		for (j = 1; j <= 4; ++j) {
			ta[i][j] = 0;
			tp[i][j] = 0;
			sa[i][j] = 0;
			sp[i][j] = 0;
			rx[i][j] = 0;
			ry[i][j] = 0;
			rz[i][j] = 0;
		}
		ta[i][i] = 1;
		tp[i][i] = 1;
		sa[i][i] = 1;
		sp[i][i] = 1;
		rx[i][i] = 1;
		ry[i][i] = 1;
		rz[i][i] = 1;
	}

/*
 *	COMPUTE the transformation matrix MAP
 *	-------------------------------------
 *
 *	Translation to matrix origin:  (Atlas image origin x0,y0,z0 --> matrix origin 0,0,0)
 *	The atlas image is shifted an additional 1.8 cm anteriorly as its origin is not
 *	at the center of the brain:
 *		(17.2 / 2) - 7.0 = 1.8 cm  (based on horizontal section +1 or -1 in Tal 1988)
 */

	ta[1][4] = -atlas.x0;
	ta[2][4] = -atlas.y0 - (1.8 / atlas.pixel_size);
	ta[3][4] = -atlas.z0;

/*
 *	Translation to PET origin:
 */

	tp[1][4] = center[0];
	tp[2][4] = center[1];
	tp[3][4] = center[2];

/*
 *	Rotation: rotate about matrix origin
 */

	rx[2][2] = cos (angle[0]);
	rx[2][3] = sin (angle[0]);
	rx[3][2] = -sin (angle[0]);
	rx[3][3] = cos (angle[0]);

	ry[1][1] = cos (angle[1]);
	ry[1][3] = -sin (angle[1]);
	ry[3][1] = sin (angle[1]);
	ry[3][3] = cos (angle[1]);

	rz[1][1] = cos (angle[2]);
	rz[1][2] = sin (angle[2]);
	rz[2][1] = -sin (angle[2]);
	rz[2][2] = cos (angle[2]);

/*
 *	Scaling:  (Atlas dim --> PET dim)
 */

	sa[1][1] = scale[0];
	sa[2][2] = scale[1];
	sa[3][3] = scale[2];

/*
 *	Scaling:  (PET dim --> atlas dim)
 */

	sp[1][1] = atlas.pixel_size / sxr.pixel_size;
	sp[2][2] = atlas.pixel_size / sxr.pixel_size;
	sp[3][3] = atlas.slice_separation / sxr.slice_separation;

/*
 *	Concatenate matrices:
 */

	matrix_mult (sa, ta, ma, 4, 4, 4);
	matrix_mult (rx, ma, mb, 4, 4, 4);
	matrix_mult (ry, mb, mc, 4, 4, 4);
	matrix_mult (rz, mc, md, 4, 4, 4);
	matrix_mult (sp, md, me, 4, 4, 4);
	matrix_mult (tp, me, map, 4, 4, 4);

	free_matrix (ta, 1, 4, 1, 4);
	free_matrix (tp, 1, 4, 1, 4);
	free_matrix (rx, 1, 4, 1, 4);
	free_matrix (ry, 1, 4, 1, 4);
	free_matrix (rz, 1, 4, 1, 4);
	free_matrix (sa, 1, 4, 1, 4);
	free_matrix (sp, 1, 4, 1, 4);
	free_matrix (ma, 1, 4, 1, 4);
	free_matrix (mb, 1, 4, 1, 4);
	free_matrix (mc, 1, 4, 1, 4);
	free_matrix (md, 1, 4, 1, 4);
	free_matrix (me, 1, 4, 1, 4);
	return (OK);
}
