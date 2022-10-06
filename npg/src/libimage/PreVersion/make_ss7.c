/*  $Log: make_ss7.c,v $
 * Revision 1.3  2000/09/27  19:03:28  tom
 * VLEN
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1998/12/10  18:41:49  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		make_ss7
 *  Date:           Dec-98
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 7 scan subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/make_ss7.c,v 1.3 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_ss7 (file)
	ImageFile      *file;
{

	if (file->m7->s3d == NULL) {
		if ((file->m7->s3d = (Scan3D_subheader7 *) calloc (1, sizeof (Scan3D_subheader7))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_ss7]: cannot calloc m7->s3d\n");
			return (ERROR);
		}
	}
	file->m7->s3d->data_type = 0;
	return (OK);
}
