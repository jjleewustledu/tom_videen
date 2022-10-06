/*	$Id: getatlas_hd6.c,v 2.5 1999/12/13 17:04:17 tom Exp $
	$Log: getatlas_hd6.c,v $
 * Revision 2.5  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.4  1996/10/11  14:39:43  tom
 * *** empty log message ***
 *
 * Revision 2.3  1996/10/11  14:36:46  tom
 * add zref
 *
 * Revision 2.2  1996/10/11  14:22:40  tom
 * malloc atlas->ydim
 * atlas->xdim[i] = hd6_xdim[i-1]
 *
 * Revision 2.1  1996/05/10  21:35:52  tom
 * atlas.xdim is now indexed from 1
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getatlas_hd6.c
 *	Date:			Aug-93
 *	Author:			Tom Videen
 *	Description:	Get values for Talairach 1967 atlas (HD6 brain)
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/getatlas_hd6.c,v 2.5 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <atlas.h>				 /* BRAIN_ATLAS */

static float           hd6_xdim[] = {10.90,
	11.35,
	11.80,
	12.10,
	12.40,
	12.70,
	12.90,
	13.10,
	13.30,
	13.50,
	13.65,
	13.80,
	13.70,
	13.60,
	13.70,
	13.80,
	13.90,
	14.00,
	14.05,
	14.10,
	14.23,
	14.36,
	14.50,
	14.45,
	14.43,
	14.46,
	14.50,
	14.50,
	14.50,
	14.30,
	14.15,
	14.25,
	14.20,
	14.18,
	14.02,
	13.86,
	13.70,
	13.63,
	13.57,
	13.50,
	13.50,
	13.50,
	13.50,
	13.33,
	13.17,
	13.00,
	12.20,
	11.40,
	10.60
};						 /* right-left (x) dimension of atlas slices (cm) */

void     getatlas_hd6 (atlas)
	BRAIN_ATLAS    *atlas;
{
	int             i;

	(void) strcpy (atlas->name, "HD6 Talairach 1967");
	atlas->pixel_size = 0.2;		 /* cm */
	atlas->slice_separation = 0.2;	 /* cm */
	atlas->nx = 128;			 /* x-dimension in pixels */
	atlas->ny = 128;			 /* y-dimension in pixels */
	atlas->nz = 49;				 /* number of slices */
	atlas->x0 = 64.5;			 /* x origin (pixel;counting from 1) */
	atlas->y0 = 64.5;			 /* y origin (pixel;counting from 1) */
	atlas->z0 = 31;				 /* z origin (slice;counting from 1) */
	atlas->zref = 31;			 /* z index to xdim, ydim (slice;counting from 1) */
	atlas->zdim = 7.0;			 /* vertical distance from AC-PC line to skull (cm) */

	atlas->ydim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
	atlas->ydim[atlas->zref] = 16.3;		 /* length of AC-PC line (cm) */

/*
 * Note that atlas->xdim is offset by 1!
 */
	atlas->xdim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
	for (i = 1; i <= atlas->nz; i++)
		atlas->xdim[i] = hd6_xdim[i-1];

}
