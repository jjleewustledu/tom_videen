/*$Id: getatlas_hd6.c,v 1.3 1994/03/03 20:08:48 ty7777 Exp $*/
/*$Log: getatlas_hd6.c,v $
 * Revision 1.3  1994/03/03  20:08:48  ty7777
 * Use Header.
 **/

/*________________________________________________________________________
	File:		getatlas_hd6.c

	Function:	PUBLIC void getatlas_hd6 (BRAIN_ATLAS *brain_atlas)

	Argument:	atlas,	an BRAIN_ATLAS structure defined in 
				/usr/local/include/petutil/atlas.h.
				The fields in atlas will be changed 
				after calling this function.

	Description:	Fill in values for HD6 brain in Talairach 1967 atlas.

	Calling:	None.

	Called by:	locate.c in the program locate.
			t88mean.

	Authors:		Tom Videen and Tom Yang.
	Date:		12-Aug-93
________________________________________________________________________*/

#include <petutil/atlas.h>		/* BRAIN_ATLAS */

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/getatlas_hd6.c,v 1.3 1994/03/03 20:08:48 ty7777 Exp $";

float hd6_xdim [] = {10.90,	
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
}; /* right-left (x) dimension of atlas slices (cm) */ 

extern void getatlas_hd6 (atlas)
BRAIN_ATLAS	*atlas;				
{
	int	i;

	strcpy (atlas->name, "HD6 Talairach 1967");	
	atlas->pixel_size	= 0.2;		/* cm */
	atlas->slice_separation	= 0.2;		/* cm */
	atlas->nx		= 128;		/* x-dimension in pixels */ 
	atlas->ny		= 128;		/* y-dimension in pixels */
	atlas->nz		= 49;		/* number of slices */
	atlas->x0		= 63.5;		/* x origin (pixel;counting from 0) */
	atlas->y0		= 63.5;		/* y origin (pixel;counting from 0) */
	atlas->z0		= 30;		/* z origin (slice;counting from 0) */
	atlas->zdim		= 7.0;		/* vertical distance from AC-PC line to skull (cm) */

	atlas->xdim = (float *) malloc (atlas->nz * sizeof (float));
	for (i = 0; i < atlas->nz; i++)
	{
		atlas->xdim [i]	= hd6_xdim [i];
	}

	atlas->ydim = (float *) malloc (atlas->nz * sizeof (float));
	atlas->ydim [atlas->z0]	= 16.3;		/* length of AC-PC line (cm) */
}
