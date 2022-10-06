/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		free_mh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Free MainHeader and its malloced data.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimg/RCS/cti.c,v 1.15 1996/03/12 22:43:02 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

void            free_mh (file)
	ImageFile      *file;
{
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
	}
}
