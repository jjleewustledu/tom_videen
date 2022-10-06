/* ===============================================================================
 *	Module:			cpattn.c
 *	Date:			Sep-2000
 *	Author:			Tom Videen
 *	Description:	Read ECAT7 image and write to sinogram
 * ===============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	ImageFile      *file;
	Index           indx;
	char           *sname;			 /* original sinogram filename */
	char           *oname;			 /* forward-projected sinogram filename */
	float        ***sino;			 /* sinogram */
	float          *ssino;			 /* sinogram plane for output */
	int             numray, numang, numpln;
	long            i, j, k, p;

	if (argc < 3) {
		fprintf (stderr, "Usage: cpattn input_sino output_sino\n");
		return (ERROR);
	}
	sname = argv[1];
	oname = argv[2];

/*	Read image */

	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

	file = make_imgfile (sname);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: Cannot open %s\n", sname);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: Cannot get MainHeader from %s\n", sname);
		return (ERROR);
	}
	if (get_sa7 (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: Cannot read subheader in %s\n", sname);
		return (ERROR);
	}
	if (use_sa7 (file) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: use_sa7\n");
		return (ERROR);
	}

	numray = (long) file->sh->dim1;
	numang = (long) file->sh->dim2;
	numpln = (long) file->sh->dim3;

	if ((sino = f3tensor (0, numpln - 1, 0, numang - 1, 0, numray - 1)) == NULL) {
		fprintf (stderr, "ERROR [cpattn]: Cannot malloc sino\n");
		return (ERROR);
	}

    if ((ssino = (float *) calloc (numang * numray, sizeof (float))) == NULL) {
        fprintf (stderr, "ERROR [cpattn]: Cannot calloc ssino\n");
        return (ERROR);
    }

	for (p = 0; p < numpln; p++) {
		indx.pln = p + 1;
		ssino = rdattn7 (file, indx);
		if (ssino == NULL) {
			fprintf (stderr, "ERROR [cpattn]: Cannot read plane %d of %s\n",
				 (int) p + 1, oname);
			return (ERROR);
		}
		k = 0;
		for (i = 0; i < numang; i++) {
			for (j = 0; j < numray; j++) {
				sino[p][i][j] = ssino[k];
				k++;
			}
		}
	}
	close_imgfile (file);

    strcpy (file->name, oname);
    (void) strcpy (file->mode, "w");
    if (open_imgfile (file) == ERROR) {
        fprintf (stderr, "ERROR [cpattn]: Cannot open %s\n", oname);
        return (ERROR);
    }
	fprintf (stderr, "open %s\n",oname);

	if (put_mh7 (file) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: Cannot read MainHeader to %s\n", oname);
		return (ERROR);
	}

	if (put_sa7 (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [cpattn]: Cannot write file subheader to %s\n", oname);
		return (ERROR);
	}

	for (p = 0; p < numpln; p++) {
		indx.pln = p + 1;
		k = 0;
		for (i = 0; i < numang; i++) {
			for (j = 0; j < numray; j++) {
				ssino[k] = sino[p][i][j];
				k++;
			}
		}
		if (wrattn7 (file, indx, ssino) == ERROR) {
			fprintf (stderr, "ERROR [cpattn]: Cannot write plane %d of %s\n",
				 (int) p + 1, oname);
			return (ERROR);
		}
	}
	free (sino);
	free (ssino);
	close_imgfile (file);
	free_imgfile (file);
	return (OK);
}
