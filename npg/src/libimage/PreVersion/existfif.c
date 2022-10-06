/*  $Id: existfif.c,v 1.1 2000/07/18 15:39:46 tom Exp $
 *  $Log: existfif.c,v $
 * Revision 1.1  2000/07/18  15:39:46  tom
 * Initial revision
 * */
/* =============================================================================
 *  Function:       existfif
 *  Date:           Jul 2000
 *  Author:         Tom Videen
 *  Description:    Check if an image frame exists.
 *                  Bypasses calls with error messages.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/existfif.c,v 1.1 2000/07/18 15:39:46 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             existfif (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	MainHeader_if  *ifh;
	long            offset, frmsize;

	if (file->ih == NULL)
		return (ERROR);
	if (file->ih->mh == NULL)
		return (ERROR);
	ifh = (MainHeader_if *) file->ih->mh;
	if (file->fp != NULL) 
		(void) fclose (file->fp);
	file->fp = fopen (ifh->data_file, file->mode);
	if (file->fp == NULL)
			return (ERROR);
	if (ifh->dim4 < indx->frm)
		return (ERROR);
	frmsize = (long) ifh->dim1 * (long) ifh->dim2 * (long) ifh->dim3 * (long) ifh->nbytes;
    offset = (long) ((indx->frm) - 1) * frmsize;
	if (fseek (file->fp, offset, SEEK_SET) != 0)
		return (ERROR);
	else
		return (OK);
}
