/*  $Id: get_sh.c,v 1.4 2000/10/12 15:24:56 tom Exp $
    $Log: get_sh.c,v $
    Revision 1.4  2000/10/12 15:24:56  tom
    read Interfile

 * Revision 1.3  2000/07/18  15:40:29  tom
 * add Interfile compatibility
 *
 * Revision 1.2  2000/03/13  18:23:14  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read file subheader.
 *	Note:	A generic subheader is returned which contains a field pointing
 *			to the original version 6 or 7 subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_sh.c,v 1.4 2000/10/12 15:24:56 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_sh (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             fformat;
	DataSetType     datatype;

	fformat = file->mh->file_format;
	datatype = (DataSetType) file->mh->filetype;

	if (fformat == MATRIX_REV7) {
		if (datatype == PetVolume) {
			if ((get_sh7 (file, indx)) == ERROR) {
				fprintf (stderr, "ERROR [get_sh]: Cannot read SubHeader from %s\n", file->name);
				return (ERROR);
			}
			if (use_sh7 (file) == ERROR)
				return (ERROR);
			mhnames7 (file);
		} else {
			fprintf (stderr, "ERROR [get_sh]: Unsupported data type %d in %s\n",
				 datatype, file->name);
			return (ERROR);
		}

	} else if (fformat == MATRIX_REV6) {
		if (datatype == PetImage) {
			if ((get_sh6 (file, indx)) == ERROR) {
				fprintf (stderr, "ERROR [get_sh]: Cannot read SubHeader from %s\n", file->name);
				return (ERROR);
			}
			if (use_sh6 (file) == ERROR)
				return (ERROR);
			mhnames6 (file);
		} else {
			fprintf (stderr, "ERROR [get_sh]: Unsupported data type %d in %s\n",
				 datatype, file->name);
			return (ERROR);
		}

	} else if (fformat == PETT6) {
		if ((get_hp6 (file, indx)) == ERROR) {
			fprintf (stderr, "ERROR [get_sh]: Cannot read Header from %s\n", file->name);
			return (ERROR);
		}
		if (use_shp6 (file) == ERROR)
			return (ERROR);

	} else if (fformat == INTERFILE) {
		if ((get_ifh (file)) == ERROR) {
			fprintf (stderr, "ERROR [get_sh]: Cannot read SubHeader from %s\n", file->name);
			return (ERROR);
		}
		if (use_shif (file) == ERROR)
			return (ERROR);

	} else
		return (ERROR);

	return (OK);
}
