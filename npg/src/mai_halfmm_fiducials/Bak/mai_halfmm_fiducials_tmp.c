/*	$Log: mai_halfmm_fiducials.c,v $
 * Revision 1.1  2006/01/31  19:27:02  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:		mai_halfmm_fiducials.c
 *	Date:		Jan 2006
 *	Author:		Tom Videen
 *	Intent:		Create new image from existing Mai Atlas image with
 *				256x256x128 1x1x1mm voxels
 *	Fiducial Points at integral voxels:
 *		AC  at 257, 257, 200
 *		PC  at 257, 314, 200
 *		OX  at 257, ---, 230
 *		OTL at 221
 *		OTR at 293
 *	===============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/mai_halfmm_fiducials/RCS/mai_halfmm_fiducials.c,v 1.1 2006/01/31 19:27:02 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <nrutil.h>
#include <matrixlib.h>
#include <myvalues.h>
#include <ANALYZE.h>

/*	----------------------------
 *	Function:			mai_halfmm_fiducials
 *	----------------------------
 */
main (argc, argv)
	int             argc;
	char           *argv[];
{

	struct dsr      hdr;			 /* ANALYZE.h */
	FILE           *fd_hdr;			 /* file for Analyze hdr */
	FILE           *fd_img;			 /* file for Analyze image */
	char           *hdr1;			 /* input hdr */
	char           *img2, *hdr2;		 /* output file */
	int             ntype;			 /* data type	 */
	int             nx, ny, nz;		 /* dimensions	 */
	int             x, y, z;		 /* indices to array	 */

	int             blur, b;
	int             xac;			 /* ac = anterior commissure	 */
	int             yac;			 /* ac	 */
	int             zac;			 /* ac	 */
	int             ypc;			 /* pc = posterior commissure	 */
	int             zox;			 /* ox = optic chiasm	 */
	int             xleft;			 /* left fiducial (OT, PT or BE)	 */
	int             xright;			 /* right fiducial (OT, PT or BE)	 */

	short           bval;
	short        ***simg;			 /* image array */
	short          *row;			 /* single row of input image */
	short           val = 1000;

	if (argc != 5) {
		fprintf (stdout, "Usage: mai_halfmm_fiducials in.hdr out.img out.hdr blur\n");
		return (ERROR);
	}
	hdr1 = argv[1];
	hdr2 = argv[2];
	img2 = argv[3];
	blur = atoi (argv[4]);

	if ((fd_hdr = fopen (hdr1, "r")) == 0) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: opening %s\n", hdr1);
		return (ERROR);
	}
	if ((fread (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: reading %s\n", hdr1);
		return (ERROR);
	}
	if (hdr.dime.dim[0] != 4) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: reading %s\n", hdr1);
		fprintf (stderr, "Dimensions = %d\n", hdr.dime.dim[0]);
		return (ERROR);
	}
	ntype = hdr.dime.datatype;
	if (ntype != 4) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: reading %s\n", hdr1);
		fprintf (stderr, "Datatype = %d\n", ntype);
		return (ERROR);
	}
	if (hdr.dime.bitpix != 16) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: reading %s\n", hdr1);
		fprintf (stderr, "Bits/pixel = %d\n", hdr.dime.bitpix);
		return (ERROR);
	}
	nx = hdr.dime.dim[1];
	ny = hdr.dime.dim[2];
	nz = hdr.dime.dim[3];
	(void) fclose (fd_hdr);

/*	Analyze image array is reversed	*/

	xac = (nx + 1) - 257;
	yac = (ny + 1) - 257;
	zac = (nz + 1) - 200;
	ypc = (ny + 1) - 314;
	zox = (nz + 1) - 230;
	xleft = (nx + 1) - 221;
	xright = (nx + 1) - 293;

	row = (short *) calloc (nx, sizeof (short));
	if (row == NULL) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: calloc row\n");
		return (ERROR);
	}
	simg = s3tensor (1, nz, 1, ny, 1, nx);
	if (simg == NULL) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: malloc simg\n");
		return (ERROR);
	}
#ifdef SKIP
#endif
/*	========================================================= */
	if (blur < 1) {

/*	Midline, left and right edges	*/

		for (y = ypc; y <= yac; y++) {
			simg[zac][y][xac] = val;
			simg[zac][y][xleft] = val;
			simg[zac][y][xright] = val;
		}

/*	Midline	at Chiasm	*/

		for (y = 0; y < ny; y++)
			simg[zox][y][xac] = val;

/*	AC and PC	*/

		for (x = xright; x <= xleft; x++) {
			simg[zac][yac][x] = val;
			simg[zac][ypc][x] = val;
		}

/*	========================================================= */
	} else {

/*	Midline, left and right edges	*/

		for (b = 0; b <= blur; b++) {
			for (y = ypc-b; y <= yac+b; y++) {
				bval = val * (blur - b) / blur;
				simg[zac][y][xac + b] = bval;
				simg[zac][y][xleft + b] = bval;
				simg[zac][y][xright + b] = bval;
			}
		}

/*	Midline	at Chiasm	*/

		for (y = 0; y < ny; y++) {
			for (b = -blur; b <= blur; b++) {
				bval = val * (blur - abs(b)) / blur;
				simg[zox][y][xac + b] = bval;
				simg[zox + b][y][xac] = bval;
			}
		}

/*	AC and PC	*/

		for (x = xright; x <= xleft; x++) {
			for (b = -blur; b <= blur; b++) {
				bval = val * (blur - abs(b)) / blur;
				simg[zac][yac + b][x] = bval;
				simg[zac + b][yac][x] = bval;
				simg[zac + b][ypc][x] = bval;
			}
		}
	}
/*	========================================================= */

/*	Write Atlas image	*/

	if ((fd_hdr = fopen (hdr2, "w")) == 0) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: opening %s\n", hdr2);
		return (ERROR);
	}
	if ((fwrite (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: writing %s\n", hdr2);
		return (ERROR);
	}
	(void) fclose (fd_hdr);
	if ((fd_img = fopen (img2, "w")) == 0) {
		fprintf (stderr, "ERROR [mai_halfmm_fiducials]: opening %s\n", img2);
		return (ERROR);
	}
	for (z = 1; z <= nz; z++) {
		for (y = 1; y <= ny; y++) {
			for (x = 1; x <= nx; x++)
				row[x - 1] = simg[z][y][x];
			if (fwrite (row, sizeof (short), nx, fd_img) != nx) {
				fprintf (stderr, "ERROR [mai_halfmm_fiducials]: writing %s\n", img2);
				return (ERROR);
			}
		}
	}
	(void) fclose (fd_img);
	free_s3tensor (simg, 1, nz, 1, ny, 1, nx);
	free (row);
	return (OK);
}
