/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       free_hif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Free Interfile header.
 *	History:
 *		Mar-96	TOV	Modified name, return value, and calling parameters.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header$";

#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

void            free_hif (file)
	ImageFile      *file;
{
	IFheader       *ifh;
	int             i, j;

	*ifh = *(file->ifh);
	for (i = 0; i < ifh->mh->dim4; i++) {
		free (ifh->fh[i]);
		for (j = 0; j < ifh->mh->dim3; j++) {
			if (ifh->sh[i][j]->comment != NULL)
				free (ifh->sh[i][j]->comment);
			free (ifh->sh[i][j]);
		}
	}
	free (ifh->mh->data_file);
	free (ifh->mh->patient_id);
	free (ifh->mh->study_date);
	free (ifh->mh->sw_revision);
	free (ifh->mh);
	if (ifh->fname != NULL)
		free (ifh->fname);
	free (ifh);
}
