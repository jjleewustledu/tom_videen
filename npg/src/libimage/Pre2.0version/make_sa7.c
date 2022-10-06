/*  $Log: make_sa7.c,v $
 * Revision 1.2  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		make_sa7
 *  Date:           Jan-00
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 7 attencor subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/make_sa7.c,v 1.2 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_sa7 (file)
	ImageFile      *file;
{

	if (file->m7->atn == NULL) {
		if ((file->m7->atn = (Attn_subheader7 *) calloc (1, sizeof (Attn_subheader7))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_sa7]: cannot calloc m7->atn\n");
			return (ERROR);
		}
	}
	file->m7->atn->data_type = 0;
	return (OK);
}
