/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		wrimg
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write Version 6 or 7 scan or image data.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimg/RCS/cti.c,v 1.15 1996/03/12 22:43:02 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

int             wrimg (file, indx, image)
	ImageFile      *file;
	Index          *indx;
	float          *image;
{
	int             fformat;
	DataSetType     dtype;

	fformat = file->mh->file_format;
	dtype = (DataSetType) file->mh->filetype;

	if (fformat == MATRIX_REV7) {
		if (dtype != PetVolume) {
			fprintf (stderr, "ERROR [wrimg]: Unsupported data type %d in %s\n", dtype, file->name);
			return (ERROR);
		}
		use_mh7 (file);
		use_sh7 (file);
		if ((wrimg7 (file, indx, image)) == ERROR) {
			fprintf (stderr, "ERROR [wrimg]: Could not write plane %d to %s\n", indx->pln, file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (dtype != PetImage) {
			fprintf (stderr, "ERROR [wrimg]: Unsupported data type %d in %s\n", dtype, file->name);
			return (ERROR);
		}
		use_mh6 (file);
		use_sh6 (file);
		if ((wrimg6 (file, indx, image)) == ERROR) {
			fprintf (stderr, "ERROR [wrimg]: ERROR [wrimg]: Could not write ",
				 "plane %d to %s\n", indx->pln, file->name);
			return (ERROR);
		}
	} else if (fformat == PETT6) {
		use_mhp6 (file);
		use_shp6 (file);
		if (wrimgp6 (file, image) == ERROR) {
			fprintf (stderr, "ERROR [wrimg]: ERROR [wrimg]: Could not write ",
				 "plane %d to %s\n", indx->pln, file->name);
			return (ERROR);
		}
#ifdef UNDEF
	} else if (fformat == INTERFILE) {
		use_mhif (file);
		use_shif (file);
		if ((wrimgif (file, indx, image)) == ERROR) {
			fprintf (stderr, "ERROR [wrimg]: Could not write plane %d to %s\n", indx->pln, file->name);
			return (ERROR);
		}
#endif
	} else {
		fprintf (stderr, "ERROR [wrimg]: Unsupported file format %d in %s\n", fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
