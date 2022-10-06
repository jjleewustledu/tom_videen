/*  $Id: get_sa.c,v 1.2 2001/11/21 17:01:28 tom Exp $
    $Log: get_sa.c,v $
 * Revision 1.2  2001/11/21  17:01:28  tom
 * elim param
 *
 * Revision 1.1  2000/09/28  17:17:31  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		get_sa
 *  Date:           Sep-2000
 *  Author:         Tom Videen
 *	Description:	Read attenuation subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/get_sa.c,v 1.2 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_sa (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             fformat;
	DataSetType     datatype;

	fformat = file->mh->file_format;
	datatype = (DataSetType) file->mh->filetype;

	if (datatype != AttenCor){
		(void) fprintf (stderr, "ERROR [get_sa]: Unsupported file type: %s\n", file->name);
		return (ERROR);
	} else if (fformat == MATRIX_REV7) {
		if (get_sa7 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sa]: Cannot read subheader in %s\n", file->name);
			return (ERROR);
		}
		if (use_sa7 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sa]: Cannot convert subheader in %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (get_sa6 (file, indx) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sa]: Cannot read subheader in %s\n", file->name);
			return (ERROR);
		}
		if (use_sa6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_sa]: Cannot convert subheader in %s\n", file->name);
			return (ERROR);
		}
	} else {
		(void) fprintf (stderr, "ERROR [get_sa]: Unsupported file type: %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
