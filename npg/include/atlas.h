/*	$Log: atlas.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.3  2004/02/12  20:29:17  tom
 * sync Feb 2004
 *
 * Revision 1.2  1996/10/11  14:32:20  tom
 * add zref to BRAIN_ATLAS
 *
 * Revision 1.1  1996/03/05  22:43:13  tom
 * Initial revision
 *
	$Id: atlas.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/*	=====================================================
 *	Module:			atlas.h
 *	Date:			Mar 1993
 *	Author:			Tom Videen
 *	Description:	Structure for atlas definition.
 *	=====================================================
 */
#ifndef ATLAS_INCLUDED
#define ATLAS_INCLUDED

/*@unused@*/
#ifndef lint
static char     rcsid_atlas[] = "$Header: /usr/local/npg/include/RCS/atlas.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

typedef struct {
	char            name[20];		 /* atlas name */
	float           pixel_size;		 /* cm */
	float           slice_separation; /* cm */
	int             nx;			 /* x-dimension (pixels) */
	int             ny;			 /* y-dimension (pixels) */
	int             nz;			 /* z-dim (number of slices) */
	float           x0;			 /* x origin (pixel) */
	float           y0;			 /* y origin (pixel) */
	int             z0;			 /* z origin (slice) */
	int             zref;		 /* z index to xdim, ydim */
	float          *xdim;		 /* right-left (x) dimension of atlas slices (cm) */
	float          *ydim;		 /* ap (y) dimension of atlas slices (cm);
						  		  * ydim[atlas.zref] = AC-PC line */
	float           zdim;		 /* vertical distance from AC-PC line to skull (cm) */
}    BRAIN_ATLAS;

extern void     getatlas_hd6 ();
extern void     getatlas_t88 ();

#endif						 /* ATLAS_INCLUDED */
