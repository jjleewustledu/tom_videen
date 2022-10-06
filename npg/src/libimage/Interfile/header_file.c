/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       header_file
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Create interfile header filename.
 *  History:
 *      Mar-96  TOV Modified.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header$";
#endif

#include <stdio.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

char           *header_file (fname)
	char           *fname;
{
	char           *ptr, *hdrfil;

	hdrfil = (char *) malloc (1 + strlen (fname));
	strcpy (hdrfil, fname);
	ptr = strrchr (hdrfil, '.');
	if (ptr != NULL)
		*ptr = NULL;
	strcat (hdrfil, ".ifh");
	return (hdrfil);
}
