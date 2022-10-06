/*$Id: imgmean.c,v 1.14 1995/01/11 20:17:58 ty7777 Exp $*/
/*$Log: imgmean.c,v $
 * Revision 1.14  1995/01/11  20:17:58  ty7777
 * Rebuilt after new libpetarc.a.
 *
 * Revision 1.13  1994/10/03  16:07:58  tom
 * standardize format with indent
 * add msk_scan and norm_slices as input parameters;
 * replace checkimg with checklst;
 * read peakslice from sxr file;
 *
 * Revision 1.12  1994/09/14  15:08:04  ty7777
 * Update the documentation.
 *
 * Revision 1.11  1994/04/11  20:58:22  ty7777
 * Added cbf, cbv, oef, tissue_activity processing.
 *
 * Revision 1.10  1994/03/10  16:20:35  ty7777
 * Same as the last version.
 *
 * Revision 1.9  1993/09/09  19:00:50  ty7777
 * Change checklst to checkimg.
 *
 * Revision 1.8  1993/09/07  19:53:04  ty7777
 * Changing the order of readparm () and readimglist ().
 *
 * Revision 1.7  1993/09/03  21:23:47  ty7777
 * Updated for new list format.
 *
 * Revision 1.6  1993/08/12  17:07:29  ty7777
 * Calling modified checkimg ().
 *
 * Revision 1.5  1993/08/09  21:08:55  ty7777
 * Added extension to checkimg.
 *
 * Revision 1.4  1993/08/03  19:29:37  ty7777
 * Searching filtered images in local and tmp.
 *
 * Revision 1.3  1993/07/30  16:57:37  ty7777
 * Added rcsid.
 */

/*	=========================================================================
 *	File Name:		imgmean.c
 *	Author:			Tom Yang.
 *	Description:	imagemean is a program to fill in the means for the images
 *					specified in an image list file.
 *	History:
 *		Created by Tom Yang on 07/23/1993.
 *		30-Sep-94	Modified TOV
 *			a) standardize format with indent;
 *			b) add 2 parameters to input:
 *				1) msk_scan is the scan number to be used for masking regardless of whether
 *					or not a control scan or specified scan is named in the parameter file or
 *					imagelist.
 *				2) norm_slices is the range of slices over which to compute the mean.
 *					It overrides any slice range included in the parameter file (i.e., slices:)
 *			c) replace checkimg with checklst;
 *			d) read sxr file to get peakslice if it is defined and pass the peakslice to
 *				template to use for defining the threshold value used to create a mask;
 *			e) replace call to threshold in template with makemask (which allows the peakslice
 *				to be passed as a parameter);
 *
 *	Input Parameters:
 *	1) image list file.
 *	2) image parameter file which contains:
 *		mskimg: string [string]	# e.g., mskimg: control_image;
 *								# mskimg: specified image_name
 *		mskpct: integer			# threshold percentage, in [0, 100],
 *								# e.g., mskpct 40. Recommended: 40 for
 *								# PETT VI; 50 for ECAT images.
 *		mskfilter: string [real] # e.g. mskfilter: gauss 0.2;
 *								# mskfilter: butterworth 0.7 5;
 *								# mskfilter: none.
 *								# Recommended: gauss 0.2 for ECAT;
 *								# none for PETT VI.
 *		imgfilter: string [real] # e.g. imgfilter: gauss 0.2;
 *								# imgfilter: butterworth 0.7 5;
 *								# imgfilter: none.
 *								# Recommended: butterworth 0.7 5;
 * 								# none for PETT VI.
 *
 *	3) scan number to use for creating the template or mask;
 *	4) slice range to use for computing the mean values;
 *
 *   Other parameters used for determining path are:
 *        species:
 *        tracer:
 *        category:
 *        datatype:
 *        scanner:
 *        acquisition:
 *        reconstruction:
 *        recfilter:
 *
 *	Algorithm for imgmean:
 *	a) Read image parameter file and put parameters in a structure.
 *	b) Read image list file and put image pairs in a list of structures.
 *	c) Construct short image names from image list and parameters.
 *	d) Get the complete image name for each short image name.
 *	e) Determine the mask image to use for each activation/control
 *	   image from the parameter maskimg:
 *
 *		if (mskimg is control_image) then
 *			use the control image as the mask image
 *		else if (mskimg is specified) then
 *			use the image specified in the parameter file
 *		endif
 *
 *	f) Compute the activation mean and control mean by applying
 *	   a template on the activation and control images. The template
 *	   is createed by threshold the mask image to mskpct% of the whole
 *	   brain maximum.
 *
 *	g) Update the image list file with means added.
 *
 *	Calling:	buildfiles () in buildfiles.c
 *		pkg_malloc () in libhp.a
 *		readimglist () in readimglist.c
 *		readparm () in readparm.c
 *		savelist () in savelist.c
 *		template () in template.c
 *
 *	================================================================================= */

#include <values.h>
#include <petutil/imglist.h>

static char     rcsid[] = "$Id: imgmean.c,v 1.14 1995/01/11 20:17:58 ty7777 Exp $";

main (argc, argv)
	int             argc;
	char          **argv;
{
	BOOLEAN         demigrate;
	BOOLEAN         oxygen_processing;
	IMAGE_LIST_DATA *image_list;
	PARAMETER_DATA  parameter_data;
    SXR_OUTPUT      sxr; 
	char            full_imgfile[6][MAXPATH];
	char           *image_list_file;
	char            mskimg_filename[MAXLINE];
	char           *parameter_list_file;
	char            short_imgfile[6][MAXLINE];
	char            short_sxrfile[MAXLINE];
	char      		full_sxrfile [MAXPATH];
	char           *msk_scan;	/* scan number to mask to for all images */
	char           *norm_slices;	/* slice range to use for normalization */
	char            cbuffer[MAXNAME];	/* scratch space for string conversion */
	int             filter_mode;
	int             image_mode;
	int             i, j;
	int             peakslice;
	int             num_image_files;
	int             num_image_list;
	int             first_slice;
	int             last_slice;

/*
 *	Get command line arguments.
 */
	parameter_list_file = argv[1];
	image_list_file = argv[2];
	msk_scan = argv[3];
	norm_slices = argv[4];

/*
 *	Read parameters file into the structure INFO_TYPE finfo.
 */
	if (readparm (parameter_list_file, &parameter_data) == FAIL) {
		fprintf (stderr, "Error: from readparm ().\n");
		free (image_list);
		exit (FAIL);
	}
	parameter_data.voi_norm = 0.;
/*
 *	Override slice range in parameter list for normalization with range from command line
 */

	if (strncmp (norm_slices, "all", 3) != 0) {
		if (sscanf (norm_slices, "%dto%d", &first_slice, &last_slice) == 2) {
			if (first_slice <= last_slice) {
				parameter_data.first_slice = first_slice;
				parameter_data.last_slice = last_slice;
			} else {
				parameter_data.first_slice = last_slice;
				parameter_data.last_slice = first_slice;
			}
		}
	}
/*
 *	Allocate memory for image list structures.
 */
	image_list = (IMAGE_LIST_DATA *) pkg_malloc (sizeof (IMAGE_LIST_DATA)
						  * MAX_IMAGE_LIST_SIZE, "chklist", "image_list");

/*
 *	Read image_list_file into IMAGE_LIST_DATA structure
 */
	if (readimglist (image_list_file, image_list, &num_image_list) == FAIL) {
		fprintf (stderr, "Error: from readimglist ().\n");
		free (image_list);
		exit (FAIL);
	}
/*
 *	Set additional parameters
 */
	demigrate = TRUE;
	if (parameter_data.processing == PROCESSING_OEF
	    || parameter_data.processing == PROCESSING_CMRO2) {
		image_mode = parameter_data.imglist_mode;
		oxygen_processing = TRUE;
	} else {
		image_mode = STDIMAGE_MODE;
		oxygen_processing = FALSE;
	}
	filter_mode = RECFILTER_MODE;

/*
 *	Compute the mean for each image in the imagelist
 */
	for (i = 0; i < num_image_list; i++) {

/*
 *	If activation mean and control mean already exist in the image list file
 *	then skip to the next record of the image list file.
 */
		if (image_list[i].mean1 > 0.0 &&
		    (strlen (image_list[i].scanset2_id) == 0 ||
		     (strlen (image_list[i].scanset2_id) > 0 && image_list[i].mean2 > 0.0)))
			continue;

		if (build_shortnames (parameter_data, image_list[i], image_mode, filter_mode,
				    &num_image_files, short_imgfile, short_sxrfile) == FAIL) {
			fprintf (stderr, "Error in building short names from image list %s %s %f %s %f %s\n",
			 image_list[i].subject_id, image_list[i].scanset1_id, image_list[i].mean1,
			     image_list[i].scanset2_id, image_list[i].mean2, image_list[i].sxrid);
		}
		if ((oxygen_processing
		     && parameter_data.imglist_mode == MULTITRACER_IMAGE_MODE
		     && num_image_files != 3 && num_image_files != 6)
		    || ((!oxygen_processing
			 || parameter_data.imglist_mode != MULTITRACER_IMAGE_MODE)
			&& num_image_files != 1 && num_image_files != 2)) {
			fprintf (stderr, "Error: there are %d image files for one subject.\n", num_image_files);
			exit (FAIL);
		}
/*
 *	Clean the array of complete image files.
 */
		for (j = 0; j < num_image_files; j++)
			strcpy (full_imgfile[j], "");

    	if (checklst (parameter_data, image_list [i], image_mode, demigrate, num_image_files,
                short_imgfile, full_imgfile, short_sxrfile, full_sxrfile) == FAIL) {
			fprintf (stderr, "Error:  At least 1 file from the imagelist cannot be found\n");
			fprintf (stderr, "        %s %s %f %s %f %s\n\n", image_list[i].subject_id,
				 image_list[i].scanset1_id, image_list[i].mean1,
				 image_list[i].scanset2_id, image_list[i].mean2, image_list[i].sxrid);
		}
/*
 *	Determine the mask image to use.
 *	Note: if the flag "-s" appears on the command line, then that scan is used as the mask.
 *
 */
		if (strncmp (msk_scan, "0", 1) != 0) {
			strncpy (mskimg_filename, "\0", MAXLINE);
			strcpy (mskimg_filename, parameter_data.species);
			strcat (mskimg_filename, image_list[i].subject_id);
			strcat (mskimg_filename, parameter_data.tracer);
			strcat (mskimg_filename, msk_scan);
			strcat (mskimg_filename, parameter_data.extension);

/* be certain MASK_SPECIFIED is disabled or the named scan will not be filtered in template.c */

			parameter_data.mskimg_type = MASK_CONTROL;	
		} else if (parameter_data.mskimg_type == MASK_CONTROL &&
			   strlen (image_list[i].scanset2_id) > 0) {
			if (oxygen_processing)
				strcpy (mskimg_filename, full_imgfile[3]);
			else
				strcpy (mskimg_filename, full_imgfile[1]);
		} else if (parameter_data.mskimg_type == MASK_CONTROL)
			strcpy (mskimg_filename, full_imgfile[0]);
		else if (parameter_data.mskimg_type == MASK_SPECIFIED)
			strcpy (mskimg_filename, parameter_data.mskimg);
		else {
			fprintf (stderr,
			 "Wrong parameter specified for keyword (mskimg) in parameter file %s.\n",
				 parameter_list_file);
			free (image_list);
			exit (FAIL);
		}

/*
 *	Check the sxr file and use the peak slice if one is identified
 */
        getsxr (full_sxrfile, &sxr);
		if (sxr.peakslice > 0) {
			peakslice = sxr.peakslice;
		} else
			peakslice = 0;

/*
 * Compute mean and update image_list activation_mean and control_mean fields.
 */
		template (full_imgfile, num_image_files, oxygen_processing,
			  mskimg_filename, parameter_data, image_list + i, peakslice);
	}

	savelist (image_list_file, num_image_list, image_list);

	free (image_list);
	free (sxr.rlpetdim);
	free (sxr.rlcenter);
	free (sxr.vertical_petdim);
	free (sxr.rlslicedim);
}
