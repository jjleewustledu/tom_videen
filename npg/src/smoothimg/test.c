/*	$Log */
/*	===============================================================================
 *	Module:			smoothimg.c
 *	Date:			Sept 2012
 *	Author:			Tom Videen
 *	Description:    Average pixel intensities 
 *	===============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/smoothimg/RCS/invert_t2.c,v 1.3 2012/07/07 20:47:40 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <image.h>
#include <math.h>
#include <values.h>
#include <ANALYZE.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	struct dsr      hdr;			 /* ANALYZE.h */
	FILE           *fp;				 /* file pointer */
	char           *in, *inhdr;		 /* input file */
	char           *out, *outhdr;	 /* output file */
	long            dim1, dim2, dim3; /* dimensions of image */
	long            len, i;
	int				val, maxval;
	int				scaler;
	short          *img;			 /* input image */

/*
 *	Get command line arguments and initialize filenames:
 */

	if (argc != 5) {
		fprintf (stderr, "Usage: smoothimg inhdr in outhdr out\n");
		return (ERROR);
	}
	inhdr = argv[1];
	in = argv[2];
	outhdr = argv[3];
	out = argv[4];

/*
 * Read input image
 */
	if ((fp = fopen (inhdr, "r")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: opening: %s\n", inhdr);
		return (ERROR);
	}
	if ((fread (&hdr, sizeof (struct dsr), 1, fp)) != 1) {
		fprintf (stderr, "ERROR [smoothimg]: reading: %s\n", inhdr);
		return (ERROR);
	}
	if (hdr.dime.bitpix != 16) {
		fprintf (stderr, "ERROR [smoothimg]: reading: %s\n", inhdr);
		fprintf (stderr, "Bits/pixel = %d\n", hdr.dime.bitpix);
		return (ERROR);
	}
	dim1 = hdr.dime.dim[1];
	dim2 = hdr.dime.dim[2];
	dim3 = hdr.dime.dim[3];
	len = dim1 * dim2 * dim3;
	(void) fclose (fp);

	if ((fp = fopen (in, "r")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: opening: %s\n", in);
		return (ERROR);
	}
	img = (short *) calloc (len, sizeof (short));
	if (img == NULL) {
		fprintf (stderr, "ERROR [smoothimg]: calloc img\n");
		return (ERROR);
	}
	if ((fread (img, sizeof (short), len, fp)) != len) {
		fprintf (stderr, "ERROR [smoothimg]: reading: %s\n", in);
		return (ERROR);
	}
	(void) fclose (fp);

	maxval = 0;
	for (i = 0; i < len; i++)
		if (img[i] > maxval)
			maxval = img[i];

	scaler = maxval;

	fprintf (stdout, "scaler %d\n", scaler);
	for (i = 0; i < len; i++) {
		if (img[i] > 0) {
			val = maxval - img[i];
			img[i] = (val*val/scaler);
		} else
			img[i] = 0;
	}

/* Write output */

	if ((fp = fopen (outhdr, "w")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: creating ANALYZE header %s\n", outhdr);
		return (ERROR);
	}
	if ((fwrite (&hdr, sizeof (struct dsr), 1, fp)) != 1) {
		fprintf (stderr, "ERROR [smoothimg]: writing: %s\n", outhdr);
		return (ERROR);
	}
	(void) fclose (fp);

	if ((fp = fopen (out, "w")) == 0) {
		fprintf (stderr, "ERROR [smoothimg]: creating ANALYZE image %s\n", out);
		return (ERROR);
	}
	if ((fwrite (img, sizeof (short), len, fp)) != len) {
		fprintf (stderr, "ERROR [smoothimg]: writing: %s\n", out);
		return (ERROR);
	}
	(void) fclose (fp);
	free (img);
	return (OK);
}
