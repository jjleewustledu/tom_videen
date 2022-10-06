/*	$Log: getatlas_t88br.c,v $
 * Revision 1.4  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.3  1996/10/11  14:36:39  tom
 * add zref
 *
 * Revision 1.2  1996/10/11  14:22:31  tom
 * malloc atlas->ydim
 *
 * Revision 1.1  1996/09/10  20:03:14  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getatlas_t88br.c
 *	Date:			Aug-93
 *	Author:			Tom Videen
 *	Description:	Get values for Talairach 1988 atlas for t88trans
 *					(Brain dimensions! compare with getatlas_t88.c)
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/getatlas_t88br.c,v 1.4 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <atlas.h>				 /* BRAIN_ATLAS */

void     getatlas_t88br (atlas)
	BRAIN_ATLAS    *atlas;			 /* brain atlas definition */
{
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

    atlas->xdim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
    atlas->ydim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
	atlas->xdim[atlas->zref] = 13.0;
	atlas->ydim[atlas->zref] = 17.2;
	atlas->zdim = 7.4;
}
