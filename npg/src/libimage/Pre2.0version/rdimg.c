/*  $Id: rdimg.c,v 1.2 2000/07/18 15:40:36 tom Exp $
    $Log: rdimg.c,v $
 * Revision 1.2  2000/07/18  15:40:36  tom
 * add Interfile compatibility
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		rdimg
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read an image file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/rdimg.c,v 1.2 2000/07/18 15:40:36 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

float          *rdimg (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	float          *image;
	int             fformat;

	fformat = file->mh->file_format;
	if (fformat == MATRIX_REV7) {
		image = rdimg7 (file, indx);
	} else if (fformat == MATRIX_REV6) {
		image = rdimg6 (file, indx);
	} else if (fformat == PETT6) {
		image = rdimgp6 (file, indx);
	} else if (fformat == INTERFILE) {
		image = rdimgif (file, indx);
	} else
		return (NULL);
	return (image);
}
