/*	$Id: getatlas_t88.c,v 2.5 1999/12/13 17:04:17 tom Exp $
	$Log: getatlas_t88.c,v $
 * Revision 2.5  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.4  1996/10/11  14:39:55  tom
 * *** empty log message ***
 *
 * Revision 2.3  1996/10/11  14:36:54  tom
 * add zref
 *
 * Revision 2.2  1996/10/11  14:23:51  tom
 * malloc atlas->ydim
 * atlas->xdim[i] = t88_xdim[i-1]
 *
 * Revision 2.1  1996/05/10  21:36:08  tom
 * atlas.xdim is now indexed from 1
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getatlas_t88.c
 *	Date:			Aug-93
 *	Author:			Tom Videen
 *	Description:	Get values for Talairach 1988 atlas (detailed brain)
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/getatlas_t88.c,v 2.5 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <atlas.h>				 /* BRAIN_ATLAS */

static float           t88_xdim[] = {8.8,
	8.8,
	10.1,
	10.1,
	10.8,
	10.8,
	10.8,
	11.2,
	11.2,
	12.0,
	12.0,
	12.0,
	12.5,
	12.5,
	12.8,
	12.8,
	12.9,
	12.9,
	13.1,
	13.1,
	13.0,
	13.0,
	13.3,
	13.3,
	13.2,
	13.2,
	13.3,
	13.3,
	13.1,
	13.1,
	13.0,
	13.0,
	13.0,
	13.0,
	13.0,
	13.0,
	12.6,
	12.6,
	12.4,
	12.4,
	12.1,
	12.1,
	11.7,
	11.7,
	11.4,
	11.4,
	11.2,
	11.2,
	10.6
};						 /* right-left (x) dimension of atlas slices (cm) */

void     getatlas_t88 (atlas)
	BRAIN_ATLAS    *atlas;			 /* brain atlas definition */
{
	int             i;

	(void) strcpy (atlas->name, "Talairach 1988");	 /* atlas name */
	atlas->pixel_size = 0.2;		 /* cm */
	atlas->slice_separation = 0.2;	 /* cm */
	atlas->nx = 128;			 /* x-dimension in pixels */
	atlas->ny = 128;			 /* y-dimension in pixels */
	atlas->nz = 49;				 /* number of slices */
	atlas->x0 = 64.5;			 /* x origin (pixel;counting from 1) */
	atlas->y0 = 64.5;			 /* y origin (pixel;counting from 1) */
	atlas->z0 = 31;				 /* z origin (slice;counting from 1) */
	atlas->zref = 31;			 /* z index to xdim, ydim (slice;counting from 1) */

/*
 * Vertical distance from AC-PC line to top of brain (cm): This distance is the measured
 * distance taken at the bisection of the AC-PC line (verticofrontal section -16; also see
 * CENTERZ in /usr/local/lib/view25d/Talairach_coronal_d.atl)
 */
	atlas->zdim = 7.4;

/*
 * Distance from front of brain to back of brain along a line passing through the AC-PC
 * (cm): This distance is the measured distance in horizontal section +1; (also see
 * /usr/local/lib/view25d/Talairach_transverse_d.atl) PLUS an estimated distance from brain
 * to the inner table of the skull (mean difference between distance estimated from HO
 * emission and sxr in 15 normals was 1.6 cm (sd = 0.5 cm) -- TOV 24-Aug-93) (but I have
 * chosen to use 1.0 cm until this is checked with MRI)
 */
	atlas->ydim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
	atlas->ydim[atlas->zref] = 17.2 + 1.0;

/*
 * Note that atlas->xdim is offset by 1!
 */
	atlas->xdim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
	for (i = 1; i <= atlas->nz; i++)
		atlas->xdim[i] = t88_xdim[i-1];
}
