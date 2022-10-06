/*$Id: imageweld.h,v 1.4 1993/10/15 20:53:17 ty7777 Exp $*/
/*$Log: imageweld.h,v $
 * Revision 1.4  1993/10/15  20:53:17  ty7777
 * Before interpolation.
 **/

#ifndef IMAGEWELD_INCLUDED
#define IMAGEWELD_INCLUDED

#include <petutil/imglist.h>

static char rcsid_imageweld [] = "$Id: imageweld.h,v 1.4 1993/10/15 20:53:17 ty7777 Exp $";

#define	MAX_MATRIX_LIST_SIZE	30

typedef struct {
	char	scanid [MAXNAME];	
	float	scale_factor;		/* scaling factor obtained from rmsmat */
	float	displacement [3];	/* displacement in the x, y and z axes, in mm */	
					/* i.e., dx, dy, dz */
	float	rotation_angle [3];	/* angle of rotation about the x, y and z axes, in radians */
					/* i.e., alpha, beta, gamma */
	float	rotation_center [3];	/* center of rotation, in mm */	
					/* i.e., cx, cy, cz */
} MATRIX_DATA;				/* This will define t4 */		

PUBLIC int GetIndexingScanOffset ();
PUBLIC int GetSamplingMode ();
PUBLIC int WeldedImages ();
PUBLIC int getfactor ();

#endif /* IMAGEWELD_INCLUDED */
