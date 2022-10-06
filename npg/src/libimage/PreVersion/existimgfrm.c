/*  $Id: existimgfrm.c,v 1.2 2000/07/18 15:40:14 tom Exp $
 *	$Log: existimgfrm.c,v $
 * Revision 1.2  2000/07/18  15:40:14  tom
 * add Interfile compatibility
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		existimgfrm
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Check if an image frame exists.
 *					Bypasses calls with error messages.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/existimgfrm.c,v 1.2 2000/07/18 15:40:14 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             existimgfrm (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             fformat;

	fformat = file->mh->file_format;
	if (fformat == MATRIX_REV7)
		return (existfrm7 (file, indx));
	else if (fformat == MATRIX_REV6)
		return (existfrm6 (file, indx));
	else if (fformat == PETT6)
		return (existp6 (file, indx));
	else if (fformat == INTERFILE)	
		return (existfif (file,indx));
	else
		return (ERROR);
}
