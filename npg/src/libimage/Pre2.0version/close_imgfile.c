/*  $Id: close_imgfile.c,v 1.5 2003/12/29 15:12:14 tom Exp $
    $Log: close_imgfile.c,v $
 * Revision 1.5  2003/12/29  15:12:14  tom
 * splint cleanup
 *
 * Revision 1.4  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
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
 *  Date:           Mar 1996
 *  Author:         Tom Videen
 *	Description:	Close an ImageFile (but keep the header structures intact)
 * =============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/close_imgfile.c,v 1.5 2003/12/29 15:12:14 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

void            close_imgfile (file)
	ImageFile      *file;
{

	if (file->m6 != NULL) {
		if (file->m6->mfp != NULL) {
			(void) matrix_close (file->m6->mfp);
			free (file->m6->mfp->mhptr);
			free (file->m6->mfp->dirlist);
			free (file->m6->mfp);
			free (file->m6->mh);
			if (file->m6->sh != NULL)
				free (file->m6->sh);
			if (file->m6->scn != NULL)
				free (file->m6->scn);
			if (file->m6->atn != NULL)
				free (file->m6->atn);
			if (file->m6->nrm != NULL)
				free (file->m6->nrm);
			free (file->m6);
		}
	}
	if (file->m7 != NULL) {
		if (file->m7->fsr != NULL) {
			if (file->m7->fsr->dir != NULL)
				free (file->m7->fsr->dir);
			free (file->m7->fsr);
		}
		free (file->m7);
	}
	if (file->fp != NULL)
		(void) fclose (file->fp);
	free (file);
}
