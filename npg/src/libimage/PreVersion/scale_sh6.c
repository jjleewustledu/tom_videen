/*  $Id: scale_sh6.c,v 1.5 2002/12/17 21:55:15 tom Exp $
 *	$Log: scale_sh6.c,v $
 * Revision 1.5  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *	Revision 1.4  2002/03/13 21:56:41  tom
 *	lint cleaning
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/04/30  19:40:23  tom
 * eliminate free(mdata)
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		scale_sh6
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *	Description:	Scale image subheader quant_factor.
 *	Note:	This avoids the assumption of SUN_I2 format which put_sh6 makes.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/scale_sh6.c,v 1.5 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             scale_sh6 (file, indx, factor)
	ImageFile      *file;
	Index          *indx;
	float           factor;
{
	MatrixData     *mdata;
	DataSetType     dtype;
	int             matnum;

	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [scale_sh6]: No version 6 header defined\n");
		return (ERROR);
	}
	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != PetImage) {
		(void) fprintf (stderr, "ERROR [scale_sh6]: Unsupported data type %d in %s\n", dtype, file->name);
		return (ERROR);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [scale_sh6]: Could not get matrix number for %s,%d,%d\n",
				file->name, indx->pln, indx->frm);
		return (ERROR);
	}
	if ((mdata = matrix_read (file->m6->mfp, matnum)) == NULL) {
		(void) fprintf (stderr, "ERROR [scale_sh6]: Could not read mdata for %s\n", file->name);
		return (ERROR);
	}
	file->m6->sh = (Image_subheader *) mdata->shptr;
	file->m6->sh->quant_scale *= factor;
	if (matrix_write (file->m6->mfp, matnum, mdata) == ERROR) {
		(void) fprintf (stderr, "ERROR [scale_sh6]: Could not write mdata for %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
