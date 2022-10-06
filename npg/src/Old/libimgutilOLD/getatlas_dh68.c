/*	$Log: getatlas_dh68.c,v $
 * Revision 1.6  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.5  1996/10/11  14:36:32  tom
 * add zref
 *
 * Revision 1.4  1996/10/11  14:22:06  tom
 * malloc atlas->ydim
 *
 * Revision 1.3  1996/10/01  22:28:44  tom
 * index from 1
 *
 * Revision 1.2  1996/08/13  20:16:21  tom
 * correct pixel_size and slice_separation
 *
 * Revision 1.1  1996/07/17  21:23:52  tom
 * Initial revision
 *
 */
/* ===============================================================================
 *	Module:			getatlas_dh68.c
 *	Date:			Jul-96
 *	Author:			Tom Videen
 *	Description:	Get values for Davis & Huffman 1968 Baboon atlas
 *
 *	Note for future reference that using this system, atlas (0.0, 0.0, 0.0) refers to a
 *	nameless point in the cerebellum. The center of the AC, used for the fixed point
 *	in my spreadsheet calculations (DH2MRI.XLS), has atlas coordinate (0.0, 21.0, 7.0).
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/getatlas_dh68.c,v 1.6 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <atlas.h>				 /* BRAIN_ATLAS */

void            getatlas_dh68 (atlas)
	BRAIN_ATLAS    *atlas;			 /* brain atlas definition */
{
	(void) strcpy (atlas->name, "Davis & Huffman 1968");
	atlas->pixel_size = 0.1;		/* cm */
	atlas->slice_separation = 0.1;	/* cm */
	atlas->nx = 128;		/* x-dimension in pixels */
	atlas->ny = 128;		/* y-dimension in pixels */
	atlas->nz = 64;			/* number of slices */
	atlas->x0 = 64.5;		/* x origin (pixel;counting from 1) */
	atlas->y0 = 64.5;		/* y origin (pixel;counting from 1) */
	atlas->z0 = 43;			/* z origin (slice;counting from 1) */
	atlas->zref = 43;		/* z index to xdim, ydim (slice;counting from 1) */
    atlas->xdim = (float *) malloc ((1 + atlas->nz) * sizeof (float));
    atlas->ydim = (float *) malloc ((1 + atlas->nz) * sizeof (float));

	atlas->xdim[atlas->zref] = 19.5;/* left-right interputamen distance, on the
								 	 * coronal slice containing the center of the AC */
	atlas->ydim[atlas->zref] = 14.5;/* distance from center of AC to top edge of PC */
	atlas->zdim = 23.2;				/* average (of left & right) vertical distance
								  	 * between the top of the caudate nucleus and the
								  	 * bottom of the optic tract, on the coronal slice
								  	 * containing the center of the AC */
}
