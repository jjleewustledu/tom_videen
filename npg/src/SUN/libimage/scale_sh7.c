/*  $Id: scale_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: scale_sh7.c,v $
    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		scale_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Scale subheader scale_factor.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/scale_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             scale_sh7 (file, indx, factor)
	ImageFile      *file;
	Index          *indx;
	float           factor;
{
	DataSetType     dtype;

	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [scale_sh7]: No version 7 header defined\n");
		return (ERROR);
	}
	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [scale_sh7]: file type %d not supported for %s\n", dtype, file->name);
		return (ERROR);
	}
	if ((get_sh7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [scale_sh7]: Could not read image subheader for %s frame %d\n",
			 file->name, indx->frm);
		return (ERROR);
	}
	file->m7->sh->scale_factor *= factor;
	if ((put_sh7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [scale_sh7]: Could not write image subheader for %s frame %d\n",
			 file->name, indx->frm);
		return (ERROR);
	} else
		return (OK);
}
