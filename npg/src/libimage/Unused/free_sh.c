/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		free_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Free SubHeader and its malloced data.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimg/RCS/cti.c,v 1.15 1996/03/12 22:43:02 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

void            free_sh (file)
	ImageFile      *file;
{
	if (file->sh != NULL) {
		if (file->sh->comment != NULL)
			free (file->sh->comment);
		free (file->sh);
	}
}
