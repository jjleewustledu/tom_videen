/*$Id: getatlas_t88.c,v 1.3 1994/03/03 20:10:38 ty7777 Exp $*/
/*$Log: getatlas_t88.c,v $
 * Revision 1.3  1994/03/03  20:10:38  ty7777
 * Added header.
 *
 * Revision 1.2  1994/02/04  20:53:45  ty7777
 * Use extern.
 *
 * Revision 1.1  1994/02/04  20:45:53  ty7777
 * Initial revision
 **/

/*________________________________________________________________________
	File:		getatlas_t88.c

	Function:	PUBLIC void getatlas_t88 (BRAIN_ATLAS *brain_atlas)

	Argument:	atlas,	an BRAIN_ATLAS structure defined in 
				/usr/local/include/petutil/atlas.h.
				The fields in atlas will be changed 
				after calling this function.

	Description:	Fill in values for detailed brain in Talairach 1988 atlas.
			(comparable to /usr/local/lib/view25d/Talairach_transverse_d_???.img)

	Calling:	None.

	Called by:	locate.c in the program locate.
			t88mean.

	Author:		Tom Videen and Tom Yang.
	Date:		23-Aug-93
________________________________________________________________________*/

#include <petutil/atlas.h>		/* BRAIN_ATLAS */

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/getatlas_t88.c,v 1.3 1994/03/03 20:10:38 ty7777 Exp $";

float t88_xdim [] = {8.8,
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
}; /* right-left (x) dimension of atlas slices (cm) */ 


extern void getatlas_t88 (atlas)
BRAIN_ATLAS	*atlas;	/* brain atlas definition */
{
	int	i;

	strcpy (atlas->name, "Talairach 1988");	/* atlas name */
	atlas->pixel_size	= 0.2;	/* cm */
	atlas->slice_separation	= 0.2;	/* cm */
	atlas->nx		= 128;	/* x-dimension in pixels */
	atlas->ny		= 128;	/* y-dimension in pixels */
	atlas->nz		= 49;	/* number of slices */
	atlas->x0		= 63.5;	/* x origin (pixel;counting from 0) */
	atlas->y0		= 63.5;	/* y origin (pixel;counting from 0) */
	atlas->z0		= 30;	/* z origin (slice;counting from 0) */

	/* 
	 * Vertical distance from AC-PC line to top of brain (cm):
	 * This distance is the measured distance taken at the bisection of the AC-PC line
	 * (verticofrontal section -16; also see CENTERZ in /usr/local/lib/view25d/Talairach_coronal_d.atl)
	 */
	atlas->zdim	= 7.4;

	/* 
	 * Distance from front of brain to back of brain along a line passing through the AC-PC (cm):
	 * This distance is the measured distance in horizontal section +1; 
	 * (also see /usr/local/lib/view25d/Talairach_transverse_d.atl)
	 * PLUS an estimated distance from brain to the inner table of the skull
	 * (mean difference between distance estimated from HO emission and sxr in 15 normals was
	 * 1.6 cm (sd = 0.5 cm) -- TOV 24-Aug-93)
	 * (but I have chosen to use 1.0 cm until this is checked with MRI)
	 */
	atlas->ydim		= (float *) malloc (atlas->nz * sizeof (float));
	atlas->ydim [atlas->z0]	= 17.2 + 1.0;

	atlas->xdim	= (float *) malloc (atlas->nz * sizeof (float));
	for (i = 0; i < atlas->nz; i++)
		atlas->xdim [i]	= t88_xdim [i];
}
