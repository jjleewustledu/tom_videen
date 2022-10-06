/*  $Id: existfrm7.c,v 1.1 1996/04/19 19:33:20 tom Exp $
    $Log: existfrm7.c,v $
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Function:       existfrm7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *  Description:    Check if an image frame exists.
 *                  Bypasses calls with error messages.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/existfrm7.c,v 1.1 1996/04/19 19:33:20 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             existfrm7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             matnum, endrec;
	int             pln1 = 1;

	if (file->fp == NULL) {
		file->fp = fopen (file->name, file->mode);
		if (file->fp == NULL)
			return (ERROR);
	}
	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if (getmatpos (file, matnum, &endrec) == ERROR)
		return (ERROR);
	else
		return (OK);
}
