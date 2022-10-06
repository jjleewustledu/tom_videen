/*	$Log: atlas.h,v $
 * Revision 1.1  1996/03/05  22:43:13  tom
 * Initial revision
 *
	$Id: atlas.h,v 1.1 1996/03/05 22:43:13 tom Exp $
 */
/*	=====================================================
 *	Module:			atlas.h
 *	Date:			Mar-93
 *	Author:			Tom Videen
 *	Description:	Structure for atlas definition.
 *	=====================================================
 */
#ifndef ATLAS_INCLUDED
#define ATLAS_INCLUDED

#ifndef lint
static char     rcsid_atlas[] = "$Header: /usr/local/npg/include/RCS/atlas.h,v 1.1 1996/03/05 22:43:13 tom Exp $";
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
	float          *xdim;		 /* right-left (x) dimension of atlas slices (cm) */
	float          *ydim;		 /* ap (y) dimension of atlas slices (cm);
						  		  * ydim[atlas.z0] = AC-PC line */
	float           zdim;		 /* vertical distance from AC-PC line to skull (cm) */
}    BRAIN_ATLAS;

/*
 * Global Functions
 */
void     getatlas_hd6 ();
void     getatlas_t88 ();

#endif						 /* ATLAS_INCLUDED */
