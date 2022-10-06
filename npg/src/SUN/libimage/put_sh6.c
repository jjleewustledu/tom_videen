/*  $Id: put_sh6.c,v 1.3 2002/12/17 21:55:15 tom Exp $
    $Log: put_sh6.c,v $
    Revision 1.3  2002/12/17 21:55:15  tom
    fix pointer/cast errors found with gcc

    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_sh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write subheader for Version 6 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/put_sh6.c,v 1.3 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             put_sh6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	struct MatDir  *matdir;
	int             matnum;

	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [put_sh6]: Unsupported data type %d for %s\n", dtype, file->name);
		return (ERROR);
	}
	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [put_sh6]: Could not get file pointer for %s\n", file->name);
		return (ERROR);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [put_sh6]: Could not get matrix number for %s\n", file->name);
		return (ERROR);
	}
	if ((matdir = matrix_find (matnum, file->m6->mfp)) == NULL) {
		(void) fprintf (stderr, "ERROR [put_sh6]: Could not find matrix directory for %s\n", file->name);
		return (ERROR);
	}
	if ((mat_write_image_subheader (file->m6->mfp->fptr, matdir->strtblk, file->m6->sh)) == ERROR) {
		(void) fprintf (stderr, "ERROR [put_sh6]: Writing subheader for %s\n", file->name);
		return (ERROR);
	}
	free (matdir);
	return (OK);
}
