/* $Log: imgsum.c,v $
 * Revision 1.2  2000/04/14  18:06:02  tom
 * elim copyright
 *
 * Revision 1.1  1997/01/16  18:58:38  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			imgsum.c
 *	Date:			Jan-97
 *	Author:			Tom Videen
 *	Description:
 *		Add 2 PET images (PETT VI or ECAT) with scale factors.
 *	===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/imgsum/RCS/imgsum.c,v 1.2 2000/04/14 18:06:02 tom Exp tom $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libnrimg.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>
#include <nrutil.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	ImageFile      *file1;			 /* input image file structure */
	Index           indx;			 /* frame, plane, gate, data, bed */
	FILE           *recfp;

	char            program[MAXFNAME];
	char            version[MAXFNAME];
	char            runstamp[40];
	char           *scan1;
	char           *scan2;
	char           *outfile;
	char           *recfile;
	int             pdim[3];
	int             i, j, k;
	int             n;
	float           f1, f2;
	float        ***img1;
	float        ***img2;
	float        ***sumimg;

/*
 *	Read command line arguments and initialize filenames.
 */

	if (argc != 15) {
		printf ("Usage: imgsum img1 f1 img2 f2 out\n");
		return (ERROR);
	}
	scan1 = argv[1];
	f1 = atof (argv[2]);
	scan2 = argv[3];
	f2 = atof (argv[4]);
	outfile = argv[5];
	recfile = argv[6];
	n = 7;
	sprintf (runstamp, "%s %s %s %s %s %s", argv[n], argv[n + 1], argv[n + 2], argv[n + 4], argv[n + 5], argv[n + 7]);

	recfp = fopen (recfile, "a");
	if (recfp == (FILE *) NULL) {
		fprintf (stderr, "ERROR [imgsum]: cannot open %s\n", recfile);
		return (ERROR);
	}
	sscanf (rcsid, "%s %s", program, version);
	strcat (program, " ");
	strcat (program, version);
	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

	file1 = make_imgfile (scan1);
	if (open_imgfile (file1) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot open %s\n", scan1);
		return (ERROR);
	}
	if (get_mh (file1) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot get MainHeader from %s\n", scan1);
		return (ERROR);
	}
	if (existimgfrm (file1, &indx) == ERROR) {
		indx.frm = 2;
		if (existimgfrm (file1, &indx) == ERROR) {
			fprintf (stderr, "ERROR [imgsum]: Cannot read scan %s frame %d\n", scan1, indx.frm);
			return (ERROR);
		}
	}
	if (get_sh (file1, &indx) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot read file subheader in %s\n", scan1);
		return (ERROR);
	}
	pdim[0] = file1->sh->dim1;
	pdim[1] = file1->sh->dim2;
	pdim[2] = file1->sh->dim3;

	if ((img1 = rdtensor (scan1, &indx, pdim)) == NULL) {
		fprintf (stderr, "ERROR [imgsum]: Cannot read %s\n", scan1);
		return (ERROR);
	}
	recout (recfp, scan1);
	close_imgfile (file1);

	file1->name = scan2;
	indx.frm = 1;
	if (open_imgfile (file1) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot open %s\n", scan2);
		return (ERROR);
	}
	if (get_mh (file1) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot get MainHeader from %s\n", scan2);
		return (ERROR);
	}
	if (existimgfrm (file1, &indx) == ERROR) {
		indx.frm = 2;
		if (existimgfrm (file1, &indx) == ERROR) {
			fprintf (stderr, "ERROR [imgsum]: Cannot read scan %s frame %d\n", scan2, indx.frm);
			return (ERROR);
		}
	}
	if (get_sh (file1, &indx) == ERROR) {
		fprintf (stderr, "ERROR [imgsum]: Cannot read file subheader in %s\n", scan2);
		return (ERROR);
	}
	if ((img2 = rdtensor (scan2, &indx, pdim)) == NULL) {
		fprintf (stderr, "ERROR [imgsum] Cannot read %s\n", scan2);
		return (ERROR);
	}
	recout (recfp, scan2);
	close_imgfile (file1);

	if ((sumimg = f3tensor (1, pdim[2], 1, pdim[1], 1, pdim[0])) == NULL) {
		fprintf (stderr, "ERROR [imgsum]: Cannot malloc summed image\n");
		return (ERROR);
	}
	printf ("Computing (%f * %s) + (%f * %s)\n", f1, scan1, f2, scan2);
	for (i = 1; i <= pdim[2]; i++)
		for (j = 1; j <= pdim[1]; j++)
			for (k = 1; k <= pdim[0]; k++)
				sumimg[i][j][k] = f1 * img1[i][j][k] + f2 * img2[i][j][k];

	file1->name = outfile;
	strcpy (file1->mode, "w");
	indx.frm = 1;
	printf ("Writing %s\n", outfile);
	if (open_imgfile (file1) == ERROR) {
		fprintf (stderr, "ERROR [sumimg]: Cannot open %s\n", outfile);
		exit (ERROR);
	}
	save_rcs (file1, rcsid, runstamp);
	file1->mh->nframe = indx.frm;
	file1->mh->orientation = 0;
	if (put_mh (file1) == ERROR) {
		fprintf (stderr, "ERROR [imgsum] writing %s\n", outfile);
		exit (ERROR);
	}
	if (writetensor (file1, &indx, sumimg) == ERROR) {
		fprintf (stderr, "ERROR [imgsum] writing %s\n", outfile);
		exit (ERROR);
	}
	close_imgfile (file1);
	free_f3tensor (img1, 1, pdim[2], 1, pdim[1], 1, pdim[0]);
	free_f3tensor (img2, 1, pdim[2], 1, pdim[1], 1, pdim[0]);
	free_f3tensor (sumimg, 1, pdim[2], 1, pdim[1], 1, pdim[0]);
	free_imgfile (file1);
	fclose (recfp);
	return (0);
}
