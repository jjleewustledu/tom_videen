/*$Log$ */
/*	===============================================================================
 *	Module:			ifs2analyze.c
 *	Date:			Aug 2005
 *	Author:			Tom Videen
 *	Description:	Create an Analyze hdr from Interfile ifh.
 *	===============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/ifs2analyze/RCS/ifs2analyze.c,v 2.1 2004/04/12 19:04:22 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <nrutil.h>
#include <ANALYZE.h>

#define MAXSTR 256
#define CORONAL    '\001'

/*	----------------------------
 *	Function:	ifs2analyze
 *	----------------------------
 */
main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* header for ANALYZE */
	FILE           *fd;
	char           *ifhdr;		 /* Interfile header filename	*/
	char           *ifimg;		 /* Interfile image filename	*/
	char           *anahdr;		 /* Analyze header filename	*/
	char           *anaimg;		 /* Analyze image filename	*/
	char            str[MAXSTR];
	char            imagename[MAXSTR] = "";
	char           *ptr;
	float           size[3];
	int             dim[3];
	int				pln;
	short          *img;
	short          *omg;
	short          *zmg;
	long            nvox, i;
	int             ndim = 0;
	int             ntype = 0;
	int             nbytes = 0;
	int				nplns = 20;

	if (argc != 6) {
		fprintf (stderr, "Usage: ifs2analyze ifhdr ifimg anahdr anaimg pln\n");
		return (ERROR);
	}
	ifhdr = argv[1];
	ifimg = argv[2];
	anahdr = argv[3];
	anaimg = argv[4];
	pln = atoi(argv[5]);

/*
 * Read Interfile header
 */

	fprintf (stderr, "Reading %s\n", ifhdr);
	if ((fd = fopen (ifhdr, "r")) == 0) {
		(void) printf ("Unable to open Interfile header %s\n", ifhdr);
		return (ERROR);
	}
	if (fgets (str, sizeof (str), fd) == NULL) {
		(void) printf ("ERROR [ifs2analyze]: reading %s\n", ifhdr);
		return (ERROR);
	}
	if (strstr (str, "INTERFILE") == NULL) {
		(void) printf ("ERROR [ifs2analyze]: %s is not an Interfile header\n", ifhdr);
		return (ERROR);
	}
	while (fgets (str, sizeof (str), fd) != NULL) {
		ptr = strstr (str, ":=");
		if (ptr != NULL) {
			ptr += 2;
			if (strstr (str, "name of data file") != NULL) {
				(void) sscanf (ptr, "%s", imagename);
				continue;
			}
			if (strstr (str, "number format") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (strstr (ptr, "signed integer") != NULL)
					ntype = 4;
				else if (strstr (ptr, "float") != NULL)
					ntype = 16;
				else {
					(void) printf ("ERROR [ifs2analyze]: image is not signed integer or float\n");
					return (ERROR);
				}
				continue;
			}
			if (strstr (str, "number of bytes per pixel") != NULL) {
				(void) sscanf (ptr, "%d", &nbytes);
				continue;
			}
			if (strstr (str, "number of dimensions") != NULL) {
				(void) sscanf (ptr, "%d", &ndim);
				if (ndim < 3 || ndim > 4) {
					(void) printf ("WARNING [ifs2analyze]: non-standard number of dimensions = %d\n",
						       ndim);
					(void) printf ("Analyze may not handle this image properly without ");
					(void) printf ("manually editing its header.\n");
				}
				continue;
			}
			if (strstr (str, "matrix size [1]") != NULL) {
				(void) sscanf (ptr, "%d", &dim[0]);
				continue;
			}
			if (strstr (str, "matrix size [2]") != NULL) {
				(void) sscanf (ptr, "%d", &dim[1]);
				continue;
			}
			if (strstr (str, "matrix size [3]") != NULL) {
				(void) sscanf (ptr, "%d", &dim[2]);
				if (dim[2] > 3) {
					(void) printf ("WARNING [ifs2analyze]: non-standard z-dimensions = %d\n",
						       dim[2]);
				}
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [1]") != NULL) {
				(void) sscanf (ptr, "%f", &size[0]);
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [2]") != NULL) {
				(void) sscanf (ptr, "%f", &size[1]);
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [3]") != NULL) {
				(void) sscanf (ptr, "%f", &size[2]);
				continue;
			}
		}
	}
	(void) fclose (fd);
	nvox = dim[0]*dim[1]*dim[2];

/*
 * Read Interfile image
 */
	fprintf (stderr, "Reading %s\n", ifimg);
    img = (short *) calloc (nvox, sizeof (short));
    if (img == NULL) {
        fprintf (stderr, "ERROR [ifs2analyze]: malloc img\n");
        return (ERROR);
    }
	if ((fd = fopen (ifimg, "r")) == 0) {
        fprintf (stderr, "ERROR [ifs2analyze]: opening %s\n", ifimg);
		return (ERROR);
	}
    if ((fread (img, sizeof (short), nvox, fd)) != nvox) {
       fprintf (stderr, "ERROR [ifs2analyze]: reading: %s\n", ifimg);
       return (ERROR);
    }
	(void) fclose (fd);

/*
 *	Re-orient Image	
 */

    omg = (short *) calloc (nvox, sizeof (short));
    if (omg == NULL) {
        fprintf (stderr, "ERROR [ifs2analyze]: malloc omg\n");
        return (ERROR);
    }
	for (i = 0; i < nvox; i++)
		omg[nvox-i-1] = img[i];

/*	Null image for zero planes	*/

    zmg = (short *) calloc (nvox, sizeof (short));
    if (zmg == NULL) {
        fprintf (stderr, "ERROR [ifs2analyze]: malloc zmg\n");
        return (ERROR);
    }

/*
 * Write re-oriented Analyze image
 */
	fprintf (stderr, "Reading %s\n", anaimg);
	if ((fd = fopen (anaimg, "w")) == 0) {
        fprintf (stderr, "ERROR [ifs2analyze]: opening %s\n", anaimg);
		return (ERROR);
	}
	for (i = nplns-1; i > pln; i--) {
    	if ((fwrite (zmg, sizeof (short), nvox, fd)) != nvox) {
       		fprintf (stderr, "ERROR [ifs2analyze]: writing: %s\n", anaimg);
       		return (ERROR);
    	}
	}
    if ((fwrite (omg, sizeof (short), nvox, fd)) != nvox) {
       fprintf (stderr, "ERROR [ifs2analyze]: writing: %s\n", anaimg);
       return (ERROR);
    }
	for (i = pln-1; i > 0; i--) {
    	if ((fwrite (zmg, sizeof (short), nvox, fd)) != nvox) {
       		fprintf (stderr, "ERROR [ifs2analyze]: writing: %s\n", anaimg);
       		return (ERROR);
    	}
	}
	(void) fclose (fd);

/*
 *	Create Analyze hdr file
 */

	printf ("Writing %s\n", anahdr);
	(void) strncpy (hdr.hk.db_name, imagename, 17);
	hdr.hk.sizeof_hdr = sizeof (struct dsr); /* required by developers */
	hdr.hk.extents = 16384;			 /* recommended by developers  */
	hdr.hk.regular = 'r';			 /* required by developers */
	hdr.dime.dim[0] = 4;			 /* typically 4 dimensions  */
	hdr.dime.dim[1] = dim[0];		 /* x dimension of atlas   */
	hdr.dime.dim[2] = nplns;		 /* number of slices in volume (should match coronal MRI) */
	hdr.dime.dim[3] = dim[1];		 /* y dimension of atlas   (FLIPPED from input) */
	hdr.dime.dim[4] = 1;			 /* only one volume typically  */
	hdr.dime.unused8 = 0;			 /* despite the label, some of this is used */
	hdr.dime.unused9 = 0;
	hdr.dime.unused10 = 0;
	hdr.dime.unused11 = 0;
	hdr.dime.unused12 = 0;
	hdr.dime.unused13 = 0;
	hdr.dime.unused14 = 0;
	hdr.dime.datatype = ntype;		 /* 4-> 16-bit signed integer; 8-> float */
	hdr.dime.bitpix = 8 * nbytes;		 /* number of bits/pixel */
	hdr.dime.pixdim[1] = size[0];		 /* in mm */
	hdr.dime.pixdim[2] = size[2];		 /* in mm (FLIPPED from input) */
	hdr.dime.pixdim[3] = size[1];		 /* in mm (FLIPPED from input) */
	hdr.dime.funused8 = 0;
	hdr.dime.funused9 = 0;
	hdr.dime.funused10 = 0;
	hdr.dime.funused11 = 0;
	hdr.dime.funused12 = 0;
	hdr.dime.funused13 = 0;
	hdr.dime.glmax = 64;
	hdr.dime.glmin = 0;
	(void) strncpy (hdr.hist.descrip, imagename, 79);
	(void) strcpy (hdr.hist.originator, "ifs2analyze");
	hdr.hist.orient = CORONAL;
	if ((fd = fopen (anahdr, "w")) == 0) {
		(void) printf ("Unable to create ANALYZE header %s\n", anahdr);
		return (ERROR);
	}
	if ((fwrite (&hdr, sizeof (struct dsr), 1, fd)) != 1) {
		(void) printf ("Error writing to: %s\n", anahdr);
		return (ERROR);
	}
	(void) fclose (fd);
	free (img);
	free (omg);
	free (zmg);
	return (0);
}
