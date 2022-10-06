/*  $Id: free_imgfile.c,v 1.8 2002/03/13 21:56:41 tom Exp $
    $Log: free_imgfile.c,v $
    Revision 1.8  2002/03/13 21:56:41  tom
    lint cleaning

 * Revision 1.7  2000/09/06  19:02:06  tom
 * set pointers to NULL after free
 *
 * Revision 1.6  2000/08/28  14:38:55  tom
 * restore free(file->name)
 *
 * Revision 1.4  2000/07/18  15:40:25  tom
 * add Interfile compatibility
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/09/25  15:18:35  tom
 * remove free
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		free_imgfile
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Free an ImageFile
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/free_imgfile.c,v 1.8 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <matrix.h>

void            free_imgfile (file)
	ImageFile      *file;
{
	struct file    *fsr;

	if (file->fp != NULL) {
		if (ftell(file->fp) >= 0)
			(void) fclose (file->fp);
		file->fp = NULL;
	}

	if (file->name != NULL) {
		free (file->name);
		file->name = NULL;
	}

	if (file->mh != NULL) {
		if (file->mh->data_file != NULL)
			free (file->mh->data_file);
		if (file->mh->sw_revision != NULL)
			free (file->mh->sw_revision);
		if (file->mh->filetime != NULL)
			free (file->mh->filetime);
		if (file->mh->gen_data != NULL)
			free (file->mh->gen_data);
		free (file->mh);
		file->mh = NULL;
	}
	if (file->sh != NULL) {
		if (file->sh->comment != NULL)
			free (file->sh->comment);
		free (file->sh);
		file->sh = NULL;
	}
	if (file->ih != NULL) {
		if (file->ih->mh != NULL) {
			if (file->ih->mh->modality != NULL)
				free (file->ih->mh->modality);
			if (file->ih->mh->sw_name != NULL)
				free (file->ih->mh->sw_name);
			if (file->ih->mh->orig_system != NULL)
				free (file->ih->mh->orig_system);
			if (file->ih->mh->institution != NULL)
				free (file->ih->mh->institution);
			if (file->ih->mh->data_file != NULL)
				free (file->ih->mh->data_file);
			if (file->ih->mh->subject_id != NULL)
				free (file->ih->mh->subject_id);
			if (file->ih->mh->study_date != NULL)
				free (file->ih->mh->study_date);
			free (file->ih->mh);
		}
		free (file->ih);
		file->ih = NULL;
	}
	if (file->m6 != NULL) {
		if (file->m6->mfp != NULL)
			matrix_close (file->m6->mfp);	/* frees file->m6->mh */
		if (file->m6->sh != NULL)
			free (file->m6->sh);

/*	matrix_close also frees file->m6->mh */

		free (file->m6);
		file->m6 = NULL;
	}
	if (file->m7 != NULL) {
		if (file->m7->mh != NULL)
			free (file->m7->mh);
		if (file->m7->sh != NULL)
			free (file->m7->sh);
		if (file->m7->fsr != NULL) {
			fsr = file->m7->fsr;
			free (fsr->dir);
			free (fsr);
		}
		free (file->m7);
		file->m7 = NULL;
	}
	if (file->p6 != NULL) {
		free (file->p6);
		file->p6 = NULL;
	}
	free (file);
}
