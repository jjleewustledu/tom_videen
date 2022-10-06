/*  $Id: get_sh6.c,v 1.3 2002/12/17 21:55:15 tom Exp $
 *	$Log: get_sh6.c,v $
 *	Revision 1.3  2002/12/17 21:55:15  tom
 *	fix pointer/cast errors found with gcc
 *
 *	Revision 1.2  1999/12/13 17:45:50  tom
 *	tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_sh6
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *	Description:	Read Version 6 ECAT subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_sh6.c,v 1.3 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_sh6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	struct MatDir  *matdir;
	int             matnum;

	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [get_sh6]: Could not get file pointer for %s", file->name);
		return (ERROR);
	}
	matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed);
	if ((matdir = matrix_find (matnum, file->m6->mfp)) == NULL) {
		(void) fprintf (stderr, "ERROR [get_sh6]: Did not find matrix dir for %s, plane = %d frame =%d\n",
			 file->name, indx->pln, indx->frm);
		return (ERROR);
	}
	if (mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed) == ERROR) {
		(void) fprintf (stderr, "ERROR [get_sh6]: Did not find matrix dir for %s, plane = %d frame =%d\n",
			 file->name, indx->pln, indx->frm);
		return (ERROR);
	}
	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != PetImage) {
		(void) fprintf (stderr, "ERROR [get_sh6]: Unsupported data type %d in %s\n", dtype, file->name);
		return (ERROR);
	}
	if (file->m6->sh == NULL) {
		if ((make_sh6 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sh6]: Could not malloc memory for m6->sh\n");
			return (ERROR);
		}
	}
	mat_read_image_subheader (file->m6->mfp->fptr, matdir->strtblk, file->m6->sh);
	free (matdir);
	return (OK);
}
