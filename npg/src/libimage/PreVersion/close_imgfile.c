/*  $Id: close_imgfile.c,v 1.3 1999/12/13 17:45:50 tom Exp $
    $Log: close_imgfile.c,v $
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/09/25  15:18:22  tom
 * remove free
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		close_imgfile
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Close an ImageFile (but keep the header structures intact)
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/close_imgfile.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <matrix.h>

void            close_imgfile (file)
	ImageFile      *file;
{
	struct file    *fsr;

	if (file->m6 != NULL)
		if (file->m6->mfp != NULL) {
			matrix_close (file->m6->mfp);
			file->m6->mfp = NULL;
		}
	if (file->m7 != NULL)
		if (file->m7->fsr != NULL) {
			fsr = file->m7->fsr;
			free (fsr->dir);
			free (fsr);
			file->m7->fsr = NULL;
		}
	if (file->fp != NULL) {
		(void) fclose (file->fp);
		file->fp = NULL;
	}
}
