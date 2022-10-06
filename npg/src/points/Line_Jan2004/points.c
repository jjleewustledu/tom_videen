/*	$Log: points.c,v $
 * Revision 1.4  2004/01/27  20:39:02  tom
 * 7 small spherical hotspots
 *
 * Revision 1.3  2002/11/27  17:10:05  tom
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/06  19:42:28  tom
 * strcpy filename
 *
 * Revision 1.1  2000/05/24  14:46:02  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			points.c
 *	Date:			Jan 2004
 *	Author:			Tom Videen
 *	Description:	Create simulated points in image file.
 * ===============================================================================
 */
/*@unused@*/
static char     rcsid[] = "$Header: /home/npggw/tom/src/points/RCS/points.c,v 1.4 2004/01/27 20:39:02 tom Exp tom $";

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <libnrimg.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

main (argc, argv)
	int             argc;
	char           *argv[];
{
	ImageFile      *file1;
	Index           indx;
	float        ***img;
	char           *infile;
	char           *outfile;
	int             pdim[3];
	int             i, j, k, m;
	float           xs, ys, zs;
	int             x = 64;
	int             y = 64;
/*
	int             x[7] = {38, 64, 90, 64, 38, 64, 90};
	int             y[7] = {49, 34, 49, 64, 79, 94, 79};
*/
	int             z = 24;

	infile = argv[1];
	outfile = argv[2];

	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;
	file1 = make_imgfile (infile);
	if (open_imgfile (file1) == ERROR) {
		fprintf (stderr, "ERROR [points]: Could not open %s\n", infile);
		return (ERROR);
	}
	if (get_mh (file1) == ERROR) {
		fprintf (stderr, "ERROR [points]: Could not get MainHeader from %s\n", infile);
		return (ERROR);
	}
	if (existimgfrm (file1, &indx) == ERROR) {
		fprintf (stderr, "ERROR [points]: cannot find %s frame %d\n", infile, indx.frm);
		return (ERROR);
	}
	if (get_sh (file1, &indx) == ERROR) {
		fprintf (stderr, "ERROR [points]: Could not read file subheader in %s\n", infile);
		return (ERROR);
	}
	pdim[0] = file1->sh->dim1;
	pdim[1] = file1->sh->dim2;
	pdim[2] = file1->sh->dim3;

	fprintf (stdout, "Reading %s, frame %d\n", file1->name, indx.frm);
	if ((img = rdtensor (file1, &indx, pdim)) == NULL) {
		fprintf (stderr, "ERROR [points]: Cannot read %s\n", infile);
		return (ERROR);
	}
	close_imgfile (file1);

	for (i = 1; i <= pdim[2]; i++)
		for (j = 1; j <= pdim[1]; j++)
			for (k = 1; k <= pdim[0]; k++)
				img[i][j][k] = 0;
	for (i = 1; i <= pdim[2]; i++)
		img[i][64][64] = 1000;

/*	Spheres 	for (m = 0; m < 7; m++) { 
	for (i = 1; i <= pdim[2]; i++) { */

/*	Cylinder 7 pixels *0.1 cm long
	6.5 pixels * 0.05 cm outer diameter
	1.5 pixels * 0.05 cm outer diameter
	for (i = 21; i <= 27; i++) {
		for (j = 1; j <= pdim[1]; j++) {
			ys = pow ((double) (j - y), 2.);
			for (k = 1; k <= pdim[0]; k++) {
				xs = pow ((double) (k - x), 2.);
				if (sqrt (xs + ys) <= 1)
					img[i][j][k] = 0;
				else if (sqrt (xs + ys) < 2)
					img[i][j][k] = 1000 * (sqrt (xs + ys) - 1.);
				else if (sqrt (xs + ys) <= 6)
					img[i][j][k] = 1000;
				else if (sqrt (xs + ys) < 7)
					img[i][j][k] = 1000 * (7. - sqrt (xs + ys));
			}
		}
	}
 */

/*	Write image */

	file1->sh->sample_size_1 = 0.05;
	file1->sh->sample_size_2 = 0.05;
	file1->sh->sample_size_3 = 0.1;
	file1->mh->slicesep = 0.1;

	fprintf (stdout, "Writing %s\n", outfile);
	file1->name = outfile;
	(void) strcpy (file1->mode, "w");
	indx.frm = 1;
	if (open_imgfile (file1) == ERROR) {
		fprintf (stderr, "ERROR [points]: Could not open %s\n", outfile);
		return (ERROR);
	}
	file1->mh->nframe = indx.frm;
	file1->mh->orientation = 0;
	if (put_mh (file1) == ERROR) {
		fprintf (stderr, "ERROR [points] writing %s\n", outfile);
		return (ERROR);
	}
	fprintf (stdout, "%d %d\n", indx.frm, indx.pln);
	if (writetensor (file1, &indx, img) == ERROR) {
		fprintf (stderr, "ERROR [points] writing %s\n", outfile);
		return (ERROR);
	}
	close_imgfile (file1);
	free_f3tensor (img, 1, pdim[2], 1, pdim[1], 1, pdim[0]);
	free_imgfile (file1);
	return (OK);
}
