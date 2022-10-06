/* $Log: t88mean.c,v $
 * Revision 3.40  1996/11/21  17:56:00  tom
 * change libimgutil (processOxygen.c) to restrict OEF to <=1.1 and CMRO2 to <= 100
 *
 * Revision 3.39  1996/11/06  17:33:23  tom
 * correct scaling of cbf and cbv images
 *
 * Revision 3.38  1996/10/16  17:18:59  tom
 * sync with Version
 * new libimage
 *
 * Revision 3.28  1996/10/11  14:44:17  tom
 * added zref for atlas.xdim, atlas.ydim in atlas_scalers.c
 *
 * Revision 3.27  1996/09/23  17:27:03  tom
 * allow scanner = 0
 *
 * Revision 3.26  1996/09/11  20:50:25  tom
 * set orientation of output image
 *
 * Revision 3.25  1996/09/03  18:22:55  tom
 * add imgmode to get_input_lists
 * fix bug in writehdr
 *
 * Revision 3.24  1996/08/26  18:01:54  tom
 * new libimage with fixed scaling in wrimg7
 *
 * Revision 3.23  1996/08/23  17:38:18  tom
 * new libimage
 *
 * Revision 3.22  1996/08/14  16:48:01  tom
 * *** empty log message ***
 *
 * Revision 3.21  1996/08/13  13:27:12  tom
 * atlas.z0 is set before calling getatlas_t88
 *
 * Revision 3.20  1996/08/05  14:33:56  tom
 * new libimage and libnrimg
 *
 * Revision 3.19  1996/05/10  22:45:47  tom
 * add mode='w' before wrtensor
 *
 * Revision 3.18  1996/05/10  21:59:55  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 3.17  1995/11/10  18:48:21  tom
 * remove petutil parent directory from include statements
 *
 * Revision 3.16  1995/11/10  18:46:55  tom
 * change rec file output of matrix
 *
 * Revision 3.15  1995/09/18  20:21:16  tom
 * changed version output in rcsid
 *
 * Revision 3.14  1995/09/18  19:50:30  tom
 * corrected z0 error in version 3.12
 *
 * Revision 3.13  1995/08/31  21:59:37  tom
 * option added-- atlas: none
 *
 * Revision 3.12  1995/05/22  20:14:15  tom
 * error checking for imglist size
 *
 * Revision 3.11  1995/05/22  15:00:32  tom
 * changed IMGLIST_SIZE to a local definition of 2000
 *
 * Revision 3.10  1995/01/19  22:42:11  tom
 * weights.
 *
 * Revision 3.9  1995/01/19  19:17:17  tom
 * weights
 *
 * Revision 3.8  1994/12/27  22:59:41  tom
 * free mask in imgrot.c
 *
 * Revision 3.7  1994/10/11  21:59:03  tom
 * Fix bug in creating filenames (sd and n) when output file has no "."
 *
 * Revision 3.6  1994/10/03  17:54:18  tom
 * set EOT to 0 on exit
 *
 * Revision 3.5  1994/10/03  16:17:13  tom
 * Changed atlas_transform to compute the x-scaling factor from the nearest slice
 * to the AC-PC plane if no slice contains the AC-PC plane;
 * The transformation matrix (and scaling factors) are written to the rec file;
 * Change calling parameters to sxr_center;
 * Separate functions atlas_scalers and matrix_transform;
 *
 * Revision 3.4  1994/09/14  14:41:51  tom
 * Allow > 49 slices in atlas and any z-origin
 *
 * Revision 3.3  1994/05/24  22:31:27  tom
 * fixed nmask comparison in image_scale.c
 *
 * Revision 3.2  1994/05/24  17:12:11  tom
 * image_scale fixed to use nmask properly
 *
 * Revision 3.1  1994/05/18  15:53:30  tom
 * changed $Id: t88mean.c,v 3.40 1996/11/21 17:56:00 tom Exp tom $ to $Header: /home/npggw/tom/src/t88mean/RCS/t88mean.c,v 3.40 1996/11/21 17:56:00 tom Exp tom $
 *
 * Revision 3.0  1994/05/11  17:19:52  tom
 * Corrected Version with proper matrix concatenation
 *
 * Revision 2.2  1994/05/10  15:33:34  tom
 * fixed bug in atlas_transform.c
 * eliminate use_pet
 *
 * Revision 2.1  1994/04/29  17:37:32  tom
 * correct y-scaling when use_pet = true
 *
 * Revision 2.0  1994/04/28  22:31:10  tom
 * last_slice must be <= number of slices
 *
 * Revision 1.99  1994/04/28  22:08:37  tom
 * added parameter slices:
 *
 * Revision 1.98  1994/04/12  22:09:06  tom
 * added nmask option in smg_include (changed image_scale.c)
 *
 * Revision 1.97  1994/04/01  16:46:01  tom
 * added copyright
 *
 * Revision 1.96  1994/03/30  14:55:05  tom
 * correct plane_separation in main_header of output image
 *
 * Revision 1.95  1994/03/21  18:11:54  tom
 * new libpetarc.a
 *
 * Revision 1.94  1994/02/22  16:27:27  tom
 * use getatlas_t88 from libhp.a
 * write pixel_size and slice_separation to output files
 *
 * Revision 1.93  1994/02/14  20:14:44  tom
 * fixed {} in symmetry
 *
 * Revision 1.92  1994/01/18  17:19:30  tom
 * new libhp
 *
 * Revision 1.91  1994/01/14  17:09:40  tom
 * OEF forced to fall 0-1.
 *
 * Revision 1.90  1994/01/06  19:39:46  tom
 * new libhp.a
 *
 * Revision 1.87  1994/01/06  19:38:51  tom
 * new libph.a
 *
 * Revision 1.86  1994/01/05  23:48:00  tom
 * fixed process_oxygen
 *
 * Revision 1.85  1993/11/18  21:55:59  tom
 * Rebuilt with updated libhp (to read ml-reconstructed images)
 *
 * Revision 1.84  1993/11/08  15:40:32  tom
 * check for sqrt(0) in image_scale.c
 *
 * Revision 1.83  1993/11/02  21:29:28  tom
 * Rebuilt with new libhp which allows parameter "suffix" to be used
 *
 * Revision 1.82  1993/10/19  14:03:09  tom
 * symmetry is working for single and paired imagelists
 *
 * Revision 1.81  1993/10/08  18:01:10  tom
 * libhp has been fixed to read PETT VI images in ECAT format in correct orientation
 *
 * Revision 1.80  1993/10/05  21:07:02  tom
 * new version to flip ecat images on x-axis
 *
 * Revision 1.78  1993/10/04  16:44:35  tom
 * eliminated atl = undefined from atlas_transform
 *
 * Revision 1.77  1993/10/01  16:41:34  tom
 * fixed calculation of sdimage
 *
 * Revision 1.76  1993/09/29  16:01:52  tom
 * added metabolic processing and hdr files (cbf, cbv, oef, cmor2)
 *
 * Revision 1.75  1993/08/27  20:43:19  tom
 * added pixel_size and plane_separation to prm file list
 *
 * Revision 1.7  1993/08/26  17:26:35  tom
 * changed atlas y-dim to used 5 mm between brain and skull
 *
 * Revision 1.6  1993/08/20  20:24:07  tom
 * first version with Talairach 1988 atlas
 *
 * Revision 1.5  1993/08/19  22:55:48  tom
 * a corrected hd6mean-atlas version of t88mean
 *
 * Revision 1.4  1993/08/12  22:58:02  tom
 * equivalent to hd6mean
 *
 * Revision 1.3  1993/08/06  15:16:49  tom
 * reads prm and list files
 *
 * Revision 1.1  1993/06/24  18:57:01  tom
 * Initial revision
 *
 */
/*	===============================================================================
 *	Module:			t88mean.c
 *	Date:			24-Jun-93
 *	Author:			Tom Videen
 *	Description:
 *		Transform PET images (PETT VI or ECAT) into atlas space (Talairach 1988).
 *		Optionally create summed images (difference, mean, or single).
 *		Optionally process images as cvf, cbv, oef, cmro2.
 *
 *	Command line parameters:
 *		parameter file
 *		image list
 *		output filename
 *
 *	Input Files:
 *		parameter file  (prm (5))
 *		image list      (imagelist (5))
 *		PET image files (PETT VI, ECAT image format; any readable by getrealimg)
 *		hdr files       (hdr (5))
 *		image mask      (PETT VI, ECAT image format; any readable by getrealimg)
 *		sxr files       (xray (1))
 *
 *	Output Files:
 *		atlas image     (ECAT image format)
 *		rec file        (rec (5))
 *		hdr file        (hdr (5))
 *		sd image        (ECAT image format)
 *		n image         (ECAT image format)
 *
 *	History:
 *		Version 1.1 (24-Jun-93) was a replication of the current hd6mean (SARP) algorithm.
 *		Version 1.4 (11-Aug-93) read prm and lst files; paired image processing.
 *			Read mask file for each activation image.
 *			Create sd and n images.
 *			Output images are equivalent to hd6mean (setting x_trans in each slice).
 *		Version 1.5 (19-Aug-93)
 *			if smg_mode = diff, no scalers are read from the inputlist;
 *			add smg_symmetry as option to rotate about Y and Z axes;
 *			renamed as t88mean;
 *			add sxr_xcenter;
 *			add recfile;
 *		Version 1.6 (20-Aug-93)
 *			switched to Talairach 1988 atlas  (getatlas_t88 replaces getatlas_hd6);
 *			changed atlas_transform to shift atlas origin anterior;
 *			added 5 mm between brain and skull at front, back, and top in calculations
 *		Version 1.7 (24-Aug-93)
 *			added option smg_scaletopet
 *			changed: 5 mm between brain and skull at front, back, and 0 at top in calculations
 *			changed signs of angles passed to and from imgrot (needs valildation)
 *		Version 1.75 (27-Aug-93)
 *			added parameters for default_pixel_size and default_plane_separation;
 *		Version 1.76 (28-Sep-93)
 *			can read image lists in 3 modes (standard, difference, or multitracer);
 *			modified get_input_lists;
 *			eliminated list.h; replaced INPUT_FILE_LIST structure with scan and sxrfile;
 *			added writehdr for creating hdr files for metabolic images;
 *			added process_pet and process_oxygen;
 *		Version 1.78 (04-Oct-93)
 *			fix algorithm for computing SD image (image_scale);
 *			eliminated array imagesum; atlas_image is used instead with summing
 *				executed within atlas_transform instead of in a separate function;
 *			eliminated function image_sum;
 *		Version 1.80 (05-Oct-93)
 *			output image files will now always have right brain on right
 *		Version 1.81 (08-Oct-93) New libraries.
 *		Version 1.82 (14-Oct-93)
 *			Symmetry correction (angles) debugged (atlas_transform and imgrot)
 *		Version 1.84 (08-Nov-93)
 *			Check for sqrt(0) in image_scale.c
 *		Version 1.85 (05-Jan-94)
 *			Fix bug in process_oxygen.c
 *		Version 1.91 (14-Jan-94)
 *			OEF forced to be 0.0-1.0 (process_oxygen.c)
 *		Version 1.92 (18-Jan-94)
 *			New libhp (bug in readimglist.c to construct sxr short names)
 *		Version 1.93 (14-Feb-94)
 *			Correct {} is symmetry loop.
 *		Version 1.94 (22-Feb-94)
 *			Use getatlas_t88 in libhp.a.
 *			Set pixel_size in output file subheaders.
 *		Version 1.95 (21-Mar-94)
 *			Rebuild with new libpetarc to recognize wobbled images with filter cutoff.
 *		Version 1.96 (30-Mar-94)
 *			add correct plane_separation to main_header
 *		Version 1.97 (12-Apr-94)
 *			added nmask to image_scale to allow selective masking of output image to n subjects
 *		Version 1.99 (28-Apr-94)
 *			added first_slice, last_slice to getprms_t88 and atlas_transform
 *		Version 2.00 (28-Apr-94)
 *			last_slice cannot be greater than number of slices
 *		Version 2.1 (29-Apr-94)
 *			fixed bug in atlas_transform when using PET for scaling y_dim
 *		Version 3.0 (11-May-94)
 *			fixed major bug in atlas_transform!
 *		Version 3.1 (18-May-94)
 *			changed Id to Header and added ifdef to allow proper compiling on HPs
 *		Version 3.2 (24-May-94)
 *			fixed nmask option in image_scale.  Ran t88mean.c through indent.
 *		Version 3.3 (24-May-94)
 *			fixed nmask again (changed > nmask to >= nmask in image_scale.c)
 *		Version 3.4 (13-Sep-94)
 *			allow the origin and number of slices in the output images to vary:
 *			added zorigin and zmax as parameters (getprms_t88)
 *			changed references to ZATL to atlas.nz (t88mean, atlas_transform)
 *		Version 3.5 (03-Oct-94)
 *			Changed atlas_transform to compute the x-scaling factor from the nearest slice
 *			to the AC-PC plane if no slice contains the AC-PC plane;
 *			The transformation matrix (and scaling factors) are written to the rec file;
 *			Change calling parameters to sxr_center;
 *			Separate functions atlas_scalers and matrix_transform;
 *		Version 3.6 (03-Oct-94)
 *			Set EOT to 0 on exit.
 *		Version 3.7 (11-Oct-94)
 *			Fix bug in creating filenames (sd and n) when output file has no "."
 *		Version 3.8 (27-Dec-94)
 *			Add free mask to imgrot.c
 *		Version 3.?	(23-Aug-95)
 *			Add atlas parameter (with options t88 or none)
 *	===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/t88mean.c,v 3.40 1996/11/21 17:56:00 tom Exp tom $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>				 /* M_PI */
#include <image.h>
#include <libimage.h>
#include <nr.h>
#include <nrutil.h>
#include <libnrimg.h>				 /* rdtensor, wrtensor */
#include <imgutil.h>				 /* IMAGE_LIST_DATA, MAXPATH, SXR_DATA, HEADER_DATA */
#include <atlas.h>				 /* BRAIN_ATLAS */

#define  EARLY_EXIT	3			 /* exit status to delete the rec file */
#define  IMGLIST_SIZE 2000			 /* max size of imglist */

/*	----------------------------
 *	Function:	t88mean
 *	----------------------------
 */
main (argc, argv)
	int             argc;
	char           *argv[];
{
	int             atlas_scalers ();
	int             getair ();
	int             matrix_transform ();
	void            atlas_transform ();
	void            printmat ();

	ImageFile      *file;
	Index           indx;
	FILE           *recfp;			 /* rec file */
	SXR_DATA       *sxr;			 /* xray & PET measurements */
	BRAIN_ATLAS     atlas;			 /* brain atlas definition */
	PARAMETER_DATA  prm;			 /* processing parameters */
	IMAGE_LIST_DATA *imglist;		 /* list of images to process */
	HEADER_DATA    *hdr;			 /* hdr structure */
	float         **xmat;			 /* matrix from sxr */
	float         **amat;			 /* AIR alignment matrix */
	float         **amat2;			 /* AIR alignment matrix */
	float         **map;			 /* conversion matrix Atlas -> PET   */

	BOOLEAN         use_mask = 0;
	BOOLEAN         compute_sd = 0;
	BOOLEAN         paired_images = 0;
	BOOLEAN         airproc = 0;
	BOOLEAN         oxygen_mode = 0;
	char           *lstfile;
	char           *prmfile;
	char           *outfile;
	char           *recfile;
	char            maskfile[MAXPATH];
	char            sdfile[MAXPATH];
	char            wtfile[MAXPATH];
	char           *ext;
	char            runstamp[40];		 /* version, date and userid info */
	char           *today;
	char           *userid;
	char           *activation_scan;
	char           *control_scan;
	char           *flow_scan1;
	char           *vol_scan1;
	char           *flow_scan2;
	char           *vol_scan2;
	int             lstsize;
	int             filtmode;
	int             imgmode;
	int             i, j, k, m;		 /* array indices */
	float           scaler;			 /* scale individual PET images */
	float           pixel_size;
	float           plane_separation;
	float           default_pixel_size;
	float           default_plane_separation;
	float           angle[3];
	float           center[3];
	float           scale[3];
	float           weight;
	float           maxwt;
	int             scanner;		 /* 6, 953, 961 */
	long            odim[3];		 /* dimensions of output atlas images */
	long            idim[3];		 /* dimensions of input images */
	float        ***image1;			 /* first (activation) PET image */
	float        ***image2;			 /* second (control) PET image */
	float        ***image3;			 /* flow PET image for oxygen processing */
	float        ***image4;			 /* volume PET image for oxygen processing */
	float        ***sdimage;		 /* SD image */
	float        ***mask;			 /* mask for PET images */
	float        ***atlas_image;		 /* atlas-transformed image */
	float        ***atlas_image2;		 /* atlas-transformed control image */
	float        ***totwt;			 /* total of weighted images contributing to each
						  * element of atlas_image */
	char         ***scan;			 /* scan filenames */
	char         ***airfile;		 /* air filename */
	char          **sxrfile;		 /* sxr filenames */
	char           *string;

/*
 *	parameters read from prm file
 */

	float           smg_norm;
	float           smg_mskpct;
	int             first_slice;
	int             last_slice;
	int             zorigin;		 /* atlas.z0 from prm file */
	int             zmax;			 /* atlas.nz from prm file */
	int             processing;
	char            smg_air[10];
	char            smg_mode[10];
	char            smg_divisor[10];
	char            smg_included[10];
	char            smg_mask[10];
	char            smg_compute[10];
	char            smg_sdimage[10];
	char            smg_nimage[10];
	char            smg_symmetry[10];
	char            atlas_name[10];

	filtmode = IMGFILTER_MODE;
	maxwt = 0;

/*
 *	Read command line arguments and initialize filenames.
 *	---------------------------------------------------
 */
	printf ("Copyright 1991-1996, Washington University\n");
	printf ("All rights reserved. Unauthorized reproduction prohibited.\n");
	if (argc != 13) {
		printf ("Usage: t88mean prmlist imglist outfile recfile date\n");
		exit (ERROR);
	}
	prmfile = argv[1];
	lstfile = argv[2];
	outfile = argv[3];
	recfile = argv[4];
	sprintf (runstamp, "%s %s %s %s %s %s", argv[5], argv[6], argv[7], argv[9], argv[10], argv[11]);
	today = argv[11];
	userid = argv[12];

	strcpy (sdfile, outfile);
	ext = strrchr (sdfile, '.');
	if (ext != NULL)
		strcpy (ext, ".sd");
	else
		strcat (sdfile, ".sd");

	strcpy (wtfile, outfile);
	ext = strrchr (wtfile, '.');
	if (ext != NULL)
		strcpy (ext, ".n");
	else
		strcat (wtfile, ".n");

	recfp = fopen (recfile, "a");
	if (recfp == (FILE *) NULL) {
		fprintf (stderr, "ERROR [t88mean]: cannot open %s\n", recfile);
		exit (ERROR);
	}
	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

/*
 *	Read imagelist into structure imglist
 *	----------------------------------------
 */

	imglist = (IMAGE_LIST_DATA *) malloc (IMGLIST_SIZE * sizeof (IMAGE_LIST_DATA));

	if (readimglist (lstfile, imglist, &lstsize) == ERROR) {
		fprintf (stderr, "ERROR [t88mean]: reading %s\n", lstfile);
		exit (ERROR);
	}
	if (lstsize > IMGLIST_SIZE) {
		fprintf (stderr, "ERROR [t88mean]: imglist size %d > %d\n", lstsize, IMGLIST_SIZE);
		exit (ERROR);
	}
/*
 *	Read parameters into structure prm
 *	---------------------------------------------
 */

	scan = (char ***) malloc (lstsize * sizeof (char **));
	airfile = (char ***) malloc (lstsize * sizeof (char **));
	for (i = 0; i < lstsize; i++) {
		scan[i] = (char **) malloc (6 * sizeof (char *));
		airfile[i] = (char **) malloc (6 * sizeof (char *));
		for (j = 0; j < 6; j++) {
			scan[i][j] = (char *) malloc (sizeof (MAXPATH));
			airfile[i][j] = (char *) malloc (sizeof (MAXPATH));
		}
	}
	sxrfile = (char **) malloc (lstsize * sizeof (char *));
	for (i = 0; i < lstsize; i++)
		sxrfile[i] = (char *) malloc (sizeof (MAXPATH));
	if (getprms_t88 (prmfile, &smg_norm, smg_divisor, smg_included, &processing, smg_mode, smg_mask,
	     smg_sdimage, smg_nimage, smg_symmetry, &smg_mskpct, smg_compute, &default_pixel_size,
			 &default_plane_separation, &first_slice, &last_slice, &zorigin, &zmax,
			 atlas_name, smg_air) == ERROR) {
		fprintf (stderr, "ERROR [t88mean]: reading parameter file %s\n", prmfile);
		exit (ERROR);
	}
	if (strcmp (smg_mode, "paired") == 0)
		paired_images = 1;
	if (strcmp (smg_air, "yes") == 0)
		airproc = 1;
	if (processing == PROC_OEF || processing == PROC_CMRO2) {
		oxygen_mode = 1;
		imgmode = MULTITRACER_IMAGE_MODE;
	} else if (strcmp (smg_mode, "diff") == 0)
		imgmode = DIFIMAGE_MODE;
	else
		imgmode = STDIMAGE_MODE;

/*
 * Get full names of activation, control and .sxr files.
 */

	get_input_lists (prmfile, lstsize, imgmode, filtmode, scan, sxrfile, airfile, &prm, imglist, smg_air);

#if DEBUG
	if (imgmode == MULTITRACER_IMAGE_MODE) {
		for (i = 0; i < lstsize; i++) {
			printf ("Set %d\n", i + 1);
			for (j = 0; j < 3; j++) {
				printf ("%s\n", scan[i][j]);
			}
			if (paired_images) {
				for (j = 3; j < 6; j++) {
					printf ("%s\n", scan[i][j]);
				}
			}
		}
	} else {
		for (i = 0; i < lstsize; i++) {
			printf ("Activation = %s\n", scan[i][0]);
			if (strcmp (smg_mode, "paired") == 0)
				printf ("Control    = %s\n", scan[i][1]);
			printf ("SXR        = %s\n", sxrfile[i]);
		}
	}
#endif

/*
 *	Get atlas values and allocate memory
 *	------------------------------------
 */

	if (!(strcmp (atlas_name, "t88"))) {
		getatlas_t88 (&atlas);
		if (zorigin > 0)
			atlas.z0 = zorigin;
		if (zmax > 0)
			atlas.nz = zmax;
	} else {
		activation_scan = scan[0][0];
		getatlas_none (&atlas, activation_scan);
		zorigin = 1;
		if (default_pixel_size > 0)
			atlas.pixel_size = default_pixel_size;
		if (default_plane_separation > 0)
			atlas.slice_separation = default_plane_separation;
	}

	odim[0] = (long) atlas.nx;
	odim[1] = (long) atlas.ny;
	odim[2] = (long) atlas.nz;
	if ((atlas_image = f3tensor (1, odim[2], 1, odim[1], 1, odim[0])) == NULL) {
		fprintf (stderr, "ERROR [t88mean]: Cannot malloc image for atlas_image\n");
		exit (ERROR);
	}
	if (paired_images && airproc) {
		if ((atlas_image2 = f3tensor (1, odim[2], 1, odim[1], 1, odim[0])) == NULL) {
			fprintf (stderr, "ERROR [t88mean]: Cannot malloc image for atlas_image2\n");
			exit (ERROR);
		}
	}
	if ((totwt = f3tensor (1, odim[2], 1, odim[1], 1, odim[0])) == NULL) {
		fprintf (stderr, "ERROR [t88mean]: Cannot malloc image for totwt\n");
		exit (ERROR);
	}
	hdr = (HEADER_DATA *) malloc (sizeof (HEADER_DATA));
	map = matrix (1, 4, 1, 4);
	xmat = matrix (1, 4, 1, 4);
	amat = matrix (1, 4, 1, 4);
	amat2 = matrix (1, 4, 1, 4);
	for (i = 1; i <= 4; i++) {
		for (j = 1; j <= 4; j++) {
			amat[j][i] = 0;
			amat2[j][i] = 0;
		}
		amat[i][i] = 1;
		amat2[i][i] = 1;
	}
	string = (char *) malloc (sizeof (MAXPATH));

/*
 *	LOOP for each image or image-pair in the list:
 *	---------------------------------------------
 */

	for (i = 0; i < lstsize; ++i) {
		activation_scan = scan[i][0];
		if (oxygen_mode) {
			flow_scan1 = scan[i][1];
			vol_scan1 = scan[i][2];
			if (paired_images) {
				control_scan = scan[i][3];
				flow_scan2 = scan[i][4];
				vol_scan2 = scan[i][5];
			}
		} else if (paired_images) {
			control_scan = scan[i][1];
		}
/*
 *	First image, first time -- get scanner and dimensions; zero arrays (atlas_image, totwt, sdimage)
 *	------------------------------------------------------------------
 */
		if (i == 0) {
			file = make_imgfile (activation_scan);
			if (open_imgfile (file) == ERROR) {
				fprintf (stderr, "ERROR [t88mean]: Cannot open %s\n", activation_scan);
				return (ERROR);
			}
			if (get_mh (file) == ERROR) {
				fprintf (stderr, "ERROR [t88mean]: Cannot get MainHeader from %s\n", activation_scan);
				return (ERROR);
			}
			if (existimgfrm (file, &indx) == ERROR) {
				if (indx.frm == 1) {
					indx.frm = 2;
					if (existimgfrm (file, &indx) == ERROR) {
						fprintf (stderr, "ERROR [t88mean]: cannot find %s frame 1 or 2\n", file);
						return (ERROR);
					}
				} else {
					fprintf (stderr, "ERROR [t88mean]: cannot find %s frame %d\n", file, indx.frm);
					return (ERROR);
				}
			}
			if (get_sh (file, &indx) == ERROR) {
				fprintf (stderr, "ERROR [t88mean]: Cannot read file subheader in %s\n", activation_scan);
				return (ERROR);
			}
			scanner = file->mh->scanner_type;
			if (scanner != 6 && scanner != 953 && scanner != 961 && scanner != 0) {
				fprintf (stderr, "ERROR [t88mean]: file is not a recognized PET image\n");
				exit (ERROR);
			}
			idim[0] = odim[0];
			idim[1] = odim[1];
			idim[2] = (long) file->sh->dim3;
			pixel_size = file->sh->sample_size_1;
			plane_separation = file->sh->sample_size_3;
			close_imgfile (file);

			if (default_pixel_size > 0)
				pixel_size = default_pixel_size;
			if (default_plane_separation > 0)
				plane_separation = default_plane_separation;

			if (strcmp (smg_mask, "yes") == 0)
				use_mask = 1;
			if (strcmp (smg_sdimage, "yes") == 0) {
				compute_sd = 1;
				if ((sdimage = f3tensor (1, odim[2], 1, odim[1], 1, odim[0])) == NULL) {
					fprintf (stderr, "ERROR [t88mean]: Cannot malloc image for sdimage\n");
					exit (ERROR);
				}
			} else
				sdimage = (float ***) NULL;
			for (m = 1; m <= odim[2]; ++m) {
				for (j = 1; j <= odim[1]; ++j) {
					for (k = 1; k <= odim[0]; ++k) {
						atlas_image[m][j][k] = 0.;
						totwt[m][j][k] = 0.;
						if (compute_sd)
							sdimage[m][j][k] = 0.;
					}
				}
			}
		}
/*
 *	Read the mask file (optional)
 *	-----------------------------
 */

		if (use_mask) {
			strcpy (maskfile, activation_scan);
			ext = strrchr (maskfile, '.');
			if (ext != NULL)
				strcpy (ext, ".msk");
			else
				strcat (maskfile, ".msk");
			if ((mask = rdtensor (maskfile, indx, odim)) == NULL) {
				fprintf (stderr, "ERROR [t88mean] reading %s\n", maskfile);
				exit (ERROR);
			}
			recout (recfp, maskfile);
		}
/*
 *	Get sxr file information
 *	------------------------
 */
		if (strcmp (atlas_name, "none") != 0) {
			sxr = getsxr (sxrfile[i]);
			fprintf (recfp, "%s\n", sxrfile[i]);
			adjust_markers (sxr);	 /* Correct sxr->dz to specific geometry */
			sxr->pixel_size = pixel_size;	/* Correct for error in sxr file for PETT
							 * VI */
			sxr->slice_separation = plane_separation;

/*
 *	Get y,z origin (pixel indices for Numerical Recipes' arrays) and x-angle (radians) from sxr.
 *	Note: for this first step, the y-origin is the bisection of the ac-pc line.
 *		The shift from the brain origin to the atlas origin (anterior shift)
 *		is performed in atlas_transform in atlas space.
 */

			center[0] = 0;
			center[1] = (sxr->apcenter) + (sxr->dap * sxr->magnification_factor / sxr->pixel_size);
			center[2] = (sxr->reference_slice) - (sxr->dz / sxr->slice_separation);
			angle[0] = -M_PI * sxr->degx / 180;
			angle[1] = 0;
			angle[2] = 0;
			scale[0] = 0;
			scale[1] = 0;
			scale[2] = 0;

/*
 *	Get translation values (center):
 *		If smg_symmetry, compute y and z-angles and use x center-of-mass for x-origin
 *			(for paired images, use the control scan for symmetry)
 *		Otherwise, get x-origin from mean center of PET slices
 */

			if (strcmp (smg_symmetry, "yes") == 0) {
				fprintf (recfp, "Symmetry operation is no longer supported\n");
				fprintf (stdout, "Symmetry operation is no longer supported\n");
			} else
				sxr_xcenter (sxr, center);

/*
 *	If smg_compute = no, then set sxr->xscale to 0 and recompute it
 */

			if (strcmp (smg_compute, "no") == 0)
				sxr->xscale = 0;

/*
 *  Get the transformation scalers
 */

			if (atlas_scalers (sxr, atlas, center, angle, scale, &last_slice) == ERROR) {
				fprintf (stderr, "ERROR [t88mean]: PET does not overlap Atlas space.\n");
				exit (ERROR);
			} else {
				fprintf (recfp, "Center: %9.3f %9.3f %9.3f   pixels\n", center[0], center[1], center[2]);
				fprintf (recfp, "Angle:  %9.3f %9.3f %9.3f   radians\n", angle[0], angle[1], angle[2]);
				fprintf (recfp, "Scale:  %9.3f %9.3f %9.3f\n", scale[0], scale[1], scale[2]);
				fprintf (stdout, "Center: %9.3f %9.3f %9.3f   pixels\n", center[0], center[1], center[2]);
				fprintf (stdout, "Angle:  %9.3f %9.3f %9.3f   radians\n", angle[0], angle[1], angle[2]);
				fprintf (stdout, "Scale:  %9.3f %9.3f %9.3f\n", scale[0], scale[1], scale[2]);
			}
/*
 *	If smg_compute = no, then exit
 */

			if (strcmp (smg_compute, "no") == 0) {
				fprintf (stdout, "-------------------------------------\n");
				fprintf (stdout, "X-SCALE =%8.3f\n", scale[0]);
				exit (EARLY_EXIT);
			}
			if ((matrix_transform (sxr, atlas, center, angle, scale, xmat)) == ERROR)
				exit (ERROR);
		}
/*
 *	Read and process First (activation) image
 *	-----------------------------------------
 */
		if (oxygen_mode) {
			if ((image3 = rdtensor (flow_scan1, indx, odim)) == NULL) {
				fprintf (stderr, "ERROR [t88mean] reading %s\n", flow_scan1);
				exit (ERROR);
			}
			recout (recfp, flow_scan1);
			printf ("Flow = %s\n", flow_scan1);

			if ((image4 = rdtensor (vol_scan1, indx, odim)) == NULL) {
				fprintf (stderr, "ERROR [t88mean]: Cannot open %s\n", vol_scan1);
				return (ERROR);
			}
			recout (recfp, vol_scan1);
			printf ("Vol  = %s\n", vol_scan1);
		}
		if ((image1 = rdtensor (activation_scan, indx, odim)) == NULL) {
			fprintf (stderr, "ERROR [t88mean] reading %s\n", activation_scan);
			exit (ERROR);
		}
		recout (recfp, activation_scan);
		printf ("Activation  = %s\n", activation_scan);

		if (strcmp (smg_compute, "no") != 0) {
			if (imgmode == DIFIMAGE_MODE)
				scaler = 1.0;
			else if (smg_norm == 0) {
				fprintf (stderr, "ERROR [t88mean]: Parameter smg_norm in parameter list is 0\n");
				exit (ERROR);
			} else if (imglist[i].mean1 == 0) {
				fprintf (stderr, "ERROR [t88mean]: Normalization factor in image list is 0\n");
				exit (ERROR);
			} else
				scaler = smg_norm / imglist[i].mean1;
			printf ("%f  %f\n", smg_norm, imglist[i].mean1);
			printf ("Scale %s by %f\n", activation_scan, scaler);
			fprintf (recfp, "Scale %s by %f\n", activation_scan, scaler);
		} else
			scaler = 1.0;

		if (oxygen_mode) {
			if (processOxygen (image1, image3, image4, idim, scaler,
			    processing, activation_scan, flow_scan1, vol_scan1, recfp, hdr) > 0) {
				fprintf (stderr, "ERROR [t88mean] processing %s %s %s\n", activation_scan,
					 flow_scan1, vol_scan1);
				exit (ERROR);
			}
			free_f3tensor (image3, 1, idim[2], 1, idim[1], 1, idim[0]);
			free_f3tensor (image4, 1, idim[2], 1, idim[1], 1, idim[0]);
		} else if (processPET (image1, idim, scaler, processing,
				       activation_scan, recfp, hdr) > 0) {
			fprintf (stderr, "ERROR [t88mean] processing %s\n", activation_scan);
			exit (ERROR);
		}
		if (airproc) {
			if (getair (airfile[i][0], amat) == ERROR)
				exit (ERROR);
			strcpy (string, "SXR Matrix");
			printmat (recfp, string, xmat);
			strcpy (string, "AIR Matrix");
			printmat (recfp, string, amat);
		}
/*
 *	Read and process second (control) image (for paired lists)
 *	----------------------------------------------------------
 */

		if (paired_images) {
			if (oxygen_mode) {
				if ((image3 = rdtensor (flow_scan2, indx, odim)) == NULL) {
					fprintf (stderr, "ERROR [t88mean] reading %s\n", flow_scan2);
					exit (ERROR);
				}
				recout (recfp, flow_scan2);
				printf ("Flow = %s\n", flow_scan2);

				if ((image4 = rdtensor (vol_scan2, indx, odim)) == NULL) {
					fprintf (stderr, "ERROR [t88mean] reading %s\n", vol_scan2);
					exit (ERROR);
				}
				recout (recfp, vol_scan2);
				printf ("Vol  = %s\n", vol_scan2);
			}
			if ((image2 = rdtensor (control_scan, indx, odim)) == NULL) {
				fprintf (stderr, "ERROR [t88mean] reading %s\n", control_scan);
				exit (ERROR);
			}
			recout (recfp, control_scan);
			printf ("Control  = %s\n", control_scan);

			if (imgmode == DIFIMAGE_MODE)
				scaler = 1.0;
			else if (imglist[i].mean2 == 0) {
				fprintf (stderr, "ERROR [t88mean]: Normalization factor in image list is 0\n");
				exit (ERROR);
			} else
				scaler = smg_norm / imglist[i].mean2;
			fprintf (recfp, "Scale %s by %f\n", control_scan, scaler);

			if (oxygen_mode) {
				if (processOxygen (image2, image3, image4, idim, scaler,
				processing, control_scan, flow_scan2, vol_scan2, recfp, hdr) > 0) {
					fprintf (stderr, "ERROR [t88mean] processing %s %s %s\n", control_scan,
						 flow_scan2, vol_scan2);
					exit (ERROR);
				}
				free_f3tensor (image3, 1, idim[2], 1, idim[1], 1, idim[0]);
				free_f3tensor (image4, 1, idim[2], 1, idim[1], 1, idim[0]);
			} else if (processPET (image2, idim, scaler, processing,
					       control_scan, recfp, hdr) > 0) {
				fprintf (stderr, "ERROR [t88mean] processing %s\n", control_scan);
				exit (ERROR);
			}
			if (airproc) {
				if (getair (airfile[i][0], amat2) == ERROR)
					exit (ERROR);
				strcpy (string, "Control AIR Matrix");
				printmat (recfp, string, amat2);
			}
		}
/*
 *
 *  Build the SXR transformation matrix XMAT
 *	------------------------
 */
		if (strcmp (atlas_name, "none") == 0) {
			if (paired_images)
				subtensors (image1, image2, idim);
			weight = imglist[i].weight;
			maxwt += weight;
			noatlas_transform (image1, idim, atlas_image, odim, sdimage, weight, totwt, mask,
					   use_mask, compute_sd);

		} else {
			if (paired_images && airproc) {
				matrix_mult (xmat, amat, map, 4, 4, 4);
				strcpy (string, "Activation Matrix*Atlas -> PET");
				printmat (recfp, string, map);
				weight = imglist[i].weight;
				maxwt += weight;
				atlas_transform (image1, idim, atlas_image, odim, sdimage, weight, totwt,
					mask, map, use_mask, compute_sd, first_slice, last_slice);
				matrix_mult (xmat, amat2, map, 4, 4, 4);
				strcpy (string, "Control Matrix*Atlas -> PET");
				printmat (recfp, string, map);
				atlas_transform (image2, idim, atlas_image2, odim, sdimage, weight, totwt,
					mask, map, use_mask, compute_sd, first_slice, last_slice);
				subtensors (atlas_image, atlas_image2, odim);

			} else {
				matrix_mult (xmat, amat, map, 4, 4, 4);
				strcpy (string, "Matrix*Atlas -> PET");
				printmat (recfp, string, map);
				if (paired_images)
					subtensors (image1, image2, idim);
				weight = imglist[i].weight;
				maxwt += weight;
				atlas_transform (image1, idim, atlas_image, odim, sdimage, weight, totwt,
					mask, map, use_mask, compute_sd, first_slice, last_slice);
			}
			if (paired_images)
				free_f3tensor (image2, 1, idim[2], 1, idim[1], 1, idim[0]);
		}
		freesxr (sxr);
		free_f3tensor (image1, 1, idim[2], 1, idim[1], 1, idim[0]);
		if (use_mask)
			free_f3tensor (mask, 1, idim[2], 1, idim[1], 1, idim[0]);
	}

/*	End LOOP
 *	--------
 *
 *	Scale final atlas image and mask pixels
 *	---------------------------------------
 */
	image_scale (atlas_image, totwt, odim, maxwt, smg_divisor, smg_included,
		     compute_sd, sdimage);

	scaler = 1.0;
	if (prm.processing == PROC_CBF)
		scaler = SCALE_CBF;
	else if (prm.processing == PROC_CBV)
		scaler = SCALE_CBV;
	else if (prm.processing == PROC_OEF)
		scaler = SCALE_OEF;
	else if (prm.processing == PROC_CMRO2)
		scaler = SCALE_CMRO2;
	if (scaler != 1.0) {
		for (i = 1; i <= odim[2]; ++i) {
			for (j = 1; j <= odim[1]; ++j) {
				for (k = 1; k <= odim[0]; ++k) {
					atlas_image[i][j][k] *= scaler;
				}
			}
		}
	}
/*
 *	Write Summed Image to outfile
 *	-----------------------------
 */
	file->name = outfile;
	strcpy (file->mode, "w");
	file->mh->nplane = odim[2];
	file->mh->slicesep = atlas.slice_separation;
	file->mh->zorigin = atlas.z0;
	file->mh->orientation = 0;
	file->sh->sample_size_1 = atlas.pixel_size;
	file->sh->sample_size_2 = atlas.pixel_size;
	file->sh->sample_size_3 = atlas.slice_separation;
	file->sh->dim1 = (short) odim[0];
	file->sh->dim2 = (short) odim[1];
	file->sh->dim3 = (short) odim[2];
	save_rcs (file, rcsid, runstamp);
	if (wrtensor (file, indx, atlas_image) == ERROR) {
		fprintf (stderr, "ERROR [t88mean] writing %s\n", outfile);
		exit (ERROR);
	}
/*
 *	Write HDR file
 *	--------------
 */
	if (processing != PROC_NONE) {
		if (writehdr (hdr, outfile, userid, today, processing, (short) atlas.nz) == ERROR) {
			fprintf (stderr, "ERROR [t88mean] writing hdr file\n");
		}
	}
/*
 *	Write SD Image to sdfile
 *	------------------------
 */
	if (compute_sd) {
		file->name = sdfile;
		strcpy (file->mode, "w");
		if (wrtensor (file, indx, sdimage) == ERROR) {
			fprintf (stderr, "ERROR [t88mean] writing %s\n", sdfile);
			exit (ERROR);
		}
	}
/*
 *	Write Total Weight Image to wtfile
 *	----------------------------------
 */
	if (strcmp (smg_nimage, "yes") == 0) {
		file->name = wtfile;
		strcpy (file->mode, "w");
		if (wrtensor (file, indx, totwt) == ERROR) {
			fprintf (stderr, "ERROR [t88mean] writing %s\n", wtfile);
			exit (ERROR);
		}
	}
/*
 *	Free memory
 *	-----------
 */
	if (compute_sd)
		free_f3tensor (sdimage, 1, odim[2], 1, odim[1], 1, odim[0]);
	free_f3tensor (totwt, 1, odim[2], 1, odim[1], 1, odim[0]);
	free_f3tensor (atlas_image, 1, odim[2], 1, odim[1], 1, odim[0]);
	free (atlas.xdim);
	free (atlas.ydim);
	free (imglist);
	for (i = 0; i < lstsize; i++) {
		for (j = 0; j < 6; j++)
			free (scan[i][j]);
		free (scan[i]);
	}
	free (scan);
	for (i = 0; i < lstsize; i++)
		free (sxrfile[i]);
	free (sxrfile);
	free (hdr);
	free_imgfile (file);
	fclose (recfp);
	exit (0);
}
