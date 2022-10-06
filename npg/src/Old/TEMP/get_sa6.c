/*  $Id: get_sa6.c,v 1.2 2002/12/17 21:55:15 tom Exp $
 *	$Log: get_sa6.c,v $
 *	Revision 1.2  2002/12/17 21:55:15  tom
 *	fix pointer/cast errors found with gcc
 *
 *	Revision 1.1  2000/02/18 21:49:13  tom
 *	Initial revision
 * */
/* =============================================================================
 *	Function:		get_sa6
 *  Date:           Feb-2000
 *  Author:         Tom Videen
 *	Description:	Read Version 6 ECAT attenuation subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_sa6.c,v 1.2 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_sa6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	struct MatDir  *matdir;
	int             matnum;

	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [get_sa6]: Could not get file pointer for %s", file->name);
		return (ERROR);
	}
	matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed);
	if ((matdir = matrix_find (matnum, file->m6->mfp)) == NULL) {
		(void) fprintf (stderr, "ERROR [get_sa6]: Did not find matrix dir for %s, plane = %d frame =%d\n",
			 file->name, indx->pln, indx->frm);
		return (ERROR);
	}
	if (mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed) == ERROR) {
		(void) fprintf (stderr, "ERROR [get_sa6]: Did not find matrix dir for %s, plane = %d frame =%d\n",
			 file->name, indx->pln, indx->frm);
		return (ERROR);
	}
	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != AttenCor) {
		(void) fprintf (stderr, "ERROR [get_sa6]: Unsupported data type %d in %s\n", dtype, file->name);
		return (ERROR);
	}
	if (file->m6->atn == NULL) {
		if ((make_sa6 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sa6]: Could not malloc memory for m6->atn\n");
			return (ERROR);
		}
	}
	mat_read_attn_subheader (file->m6->mfp->fptr, matdir->strtblk, file->m6->atn);
	free (matdir);
	return (OK);
}
