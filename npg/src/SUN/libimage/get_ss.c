/*  $Id: get_ss.c,v 1.2 2001/11/21 17:01:28 tom Exp $
    $Log: get_ss.c,v $
    Revision 1.2  2001/11/21 17:01:28  tom
    elim param

 * Revision 1.1  2000/03/13  18:23:14  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		get_ss
 *  Date:           Mar-2000
 *  Author:         Tom Videen
 *	Description:	Read sinogram subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_ss.c,v 1.2 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_ss (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             fformat;
	DataSetType     datatype;

	fformat = file->mh->file_format;
	datatype = (DataSetType) file->mh->filetype;

	if (datatype != Sinogram && datatype != AttenCor && datatype != Short3dSinogram){
		(void) fprintf (stderr, "ERROR [get_ss]: Unsupported file type: %s\n", file->name);
		return (ERROR);
	} else if (fformat == MATRIX_REV7) {
		if (get_ss7 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_ss]: Cannot read subheader in %s\n", file->name);
			return (ERROR);
		}
		if (use_ss7 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_ss]: Cannot convert subheader in %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (get_ss6 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_ss]: Cannot read subheader in %s\n", file->name);
			return (ERROR);
		}
		if (use_ss6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_ss]: Cannot convert subheader in %s\n", file->name);
			return (ERROR);
		}
	} else {
		(void) fprintf (stderr, "ERROR [get_ss]: Unsupported file type: %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
