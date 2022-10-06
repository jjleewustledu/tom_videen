/*	$Log$ */
/*	===============================================================================
 *	Module:			smoothimg.c
 *	Date:			Sep 2012
 *	Author:			Tom Videen
 *	Description:	Average local pixels
 *	===============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/smoothimg/RCS/smoothimg.c,v 2.2 2008/11/07 14:04:18 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <myvalues.h>
#include <ANALYZE.h>

/*	----------------------------
 *	Function:			smoothimg
 *	----------------------------
 */
int main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* ANALYZE.h */
	FILE           *fd_hdr;			 /* file for ANALYZE hdr */
	FILE           *fd_img;			 /* file for ANALYZE image */
	char           *img1, *hdr1;		 /* input file */
	char           *img2, *hdr2;		 /* output file */
	long            dim1, dim2, dim3;	 /* dimensions of image */
	long            num, i;
	short          *imga, *imgb;		 /* image */
	float          *fimga, *fimgb;		 /* float image */
	int             itype1 = 0;		 /* 0 = int, 1 = float	 */
	int             itype2 = 0;		 /* 0 = int, 1 = float	 */

	if (argc != 5) {
		printf ("Usage: smoothimg img1 hdr1 img2 hdr2\n");
		return (ERROR);
	}
	hdr1 = argv[1];
	img1 = argv[2];
	hdr2 = argv[3];
	img2 = argv[4];

/*	Get first image */

	if ((fd_hdr = fopen (hdr1, "r")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: opening %s\n", hdr1);
		return (ERROR);
	}
	if ((fread (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		fprintf (stderr, "ERROR [smoothimg]: reading %s\n", hdr1);
		return (ERROR);
	}
	if (hdr.dime.dim[0] < 3) {
		fprintf (stderr, "ERROR [smoothimg]: reading %s\n", hdr1);
		fprintf (stderr, "Dimensions = %d\n", hdr.dime.dim[0]);
		return (ERROR);
	}
	dim1 = hdr.dime.dim[1];
	dim2 = hdr.dime.dim[2];
	dim3 = hdr.dime.dim[3];
	num = dim1 * dim2 * dim3;
	(void) fclose (fd_hdr);

	if (hdr.dime.datatype == 4 && hdr.dime.bitpix == 16) {
		itype1 = 0;
		imga = (short *) calloc (num, sizeof (short));
		if (imga == NULL) {
			fprintf (stderr, "ERROR [smoothimg]: calloc imga\n");
			return (ERROR);
		}
		if ((fd_img = fopen (img1, "r")) == 0) {
			fprintf (stderr, "ERROR [smoothimg]: opening %s\n", img1);
			return (ERROR);
		}
		if ((fread (imga, sizeof (short), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: reading: %s\n", img1);
			return (ERROR);
		}
		(void) fclose (fd_img);
		fprintf (stdout, "Read %s\n", img1);
	} else if (hdr.dime.datatype == 16 && hdr.dime.bitpix == 32) {
		itype1 = 1;
		fimga = (float *) calloc (num, sizeof (float));
		if (fimga == NULL) {
			fprintf (stderr, "ERROR [smoothimg]: calloc fimga\n");
			return (ERROR);
		}
		if ((fd_img = fopen (img1, "r")) == 0) {
			fprintf (stderr, "ERROR [smoothimg]: opening %s\n", img1);
			return (ERROR);
		}
		if ((fread (fimga, sizeof (float), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: reading: %s\n", img1);
			return (ERROR);
		}
		(void) fclose (fd_img);
		fprintf (stdout, "Read %s\n", img1);
	} else {
		fprintf (stderr, "ERROR [smoothimg]: Reading: %s\n", hdr1);
		fprintf (stderr, "Datatype = %d\n", hdr.dime.datatype);
		fprintf (stderr, "Bits/pixel = %d\n", hdr.dime.bitpix);
		return (ERROR);
	}

/*	Smooth */

/*	Write image */

	if ((fd_img = fopen (img2, "w")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: opening %s\n", img2);
		return (ERROR);
	}
	if (itype1 == 0 && itype2 == 0) {
		for (i = 0; i < num; i++)
			imga[i] -= imgb[i];
		if ((fwrite (imga, sizeof (short), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: writing %s\n", img2);
			return (ERROR);
		}
		fprintf (stderr, "Wrote %s as integer\n", img2);
		free (imga);
		free (imgb);
	} else if (itype1 == 0 && itype2 == 1) {
		hdr.dime.datatype = 16;
		hdr.dime.bitpix = 32;
		for (i = 0; i < num; i++)
			fimgb[i] -= imga[i];
		if ((fwrite (fimgb, sizeof (float), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: writing %s\n", img2);
			return (ERROR);
		}
		fprintf (stderr, "Wrote %s as float\n", img2);
		free (imga);
		free (fimgb);
	} else if (itype1 == 1 && itype2 == 0) {
		hdr.dime.datatype = 16;
		hdr.dime.bitpix = 32;
		for (i = 0; i < num; i++)
			fimga[i] -= imgb[i];
		if ((fwrite (fimga, sizeof (float), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: writing %s\n", img2);
			return (ERROR);
		}
		fprintf (stderr, "Wrote %s as float\n", img2);
		free (fimga);
		free (imgb);
	} else {
		hdr.dime.datatype = 16;
		hdr.dime.bitpix = 32;
		for (i = 0; i < num; i++)
			fimga[i] -= fimgb[i];
		if ((fwrite (fimga, sizeof (float), num, fd_img)) != num) {
			fprintf (stderr, "ERROR [smoothimg]: writing %s\n", img2);
			return (ERROR);
		}
		fprintf (stderr, "Wrote %s as float\n", img2);
		free (fimga);
		free (fimgb);
	}
	(void) fclose (fd_img);

	if ((fd_hdr = fopen (hdr2, "w")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: openng %s\n", hdr2);
		return (ERROR);
	}
	if ((fwrite (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		fprintf (stderr, "ERROR [smoothimg]: writing %s\n", hdr2);
		return (ERROR);
	}
	(void) fclose (fd_hdr);
	return (OK);
}
