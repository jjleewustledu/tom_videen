/*  $Id: make_sa6.c,v 1.1 2000/02/18 21:49:13 tom Exp $
    $Log: make_sa6.c,v $
 * Revision 1.1  2000/02/18  21:49:13  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		make_sa6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 6 attenuation subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/make_sa6.c,v 1.1 2000/02/18 21:49:13 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_sa6 (file)
	ImageFile      *file;
{

	if (file->m6->atn == NULL) {
		if ((file->m6->atn = (Attn_subheader *) malloc (sizeof (Attn_subheader))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_sa6]: cannot malloc m6->atn\n");
			return (ERROR);
		}
	}
	file->m6->atn->data_type = 0;
	return (OK);
}
