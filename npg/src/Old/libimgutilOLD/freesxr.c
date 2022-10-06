/*	$Id: freesxr.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: freesxr.c,v $
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			freesxr.c
 *	Date:			Dec-95
 *	Author:			Tom Videen
 *	Description:	Free an SXR_DATA structure.
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/freesxr.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdlib.h>
#include <imgutil.h>

void      freesxr (sxr)
	SXR_DATA     *sxr;
{
    free (sxr->rlpetdim);
    free (sxr->rlcenter);
    free (sxr->vertical_petdim);
    free (sxr->rlslicedim);
	free (sxr);
}
