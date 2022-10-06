/*$Id: rotation.c,v 1.1 1993/01/26 20:58:27 ty7777 Exp $*/
/*$Log: rotation.c,v $
 * Revision 1.1  1993/01/26  20:58:27  ty7777
 * Initial revision
 **/
#ifndef lint
static char sccsid [] = "@(#)rotation.c	11/04/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Id: rotation.c,v 1.1 1993/01/26 20:58:27 ty7777 Exp $";

/*___________________________________________________________________________
  File:		rotate.c

  Function:	void GetRotationAngles ()

  Arguments:	

  Description:	This function computes the rotation angles based on the ear markers.

  History:	Created by Tom Yang on 11/04/92. 
___________________________________________________________________________*/

#include "xray.h"
#include <math.h>

PUBLIC void GetRotationAngles (sxr_output)
	SXR_OUTPUT		*sxr_output;
{
	float	angy;
	float	angz;

	angy	= atan (sxr_output->magnification_factor * sxr_output->eardz 
			/ sxr_output->ear_separation);
	sxr_output->degy	= radian2deg (angy);

	angz	= atan (sxr_output->magnification_factor * sxr_output->eardap 
			/ sxr_output->ear_separation);
	sxr_output->degz	= radian2deg (angz);
}
