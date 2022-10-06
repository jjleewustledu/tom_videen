/*  $Id */
/*  $Log */
/* =============================================================================
 *	Function:		rdimg_decaycor
 *  Date:           Aug 2008
 *  Author:         Tom Videen
 *	Description:	Read an image file.
 * =============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/rdimg.c,v 2.4 2004/02/20 16:06:24 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

float          *rdimg_decaycor (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	float          *image;
	int             fformat;

	fformat = file->mh->file_format;
	if (fformat == MATRIX_REV7) {
		image = rdimg7_decaycor (file, indx);
	} else
		return (NULL);
	return (image);
}
