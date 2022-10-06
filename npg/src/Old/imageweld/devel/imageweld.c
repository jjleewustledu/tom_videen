/* $Log$ */
/*_________________________________________________________________
  Description:	This program reads two ECAT images which represent the
		top and bottom scans of the brain with some overlapped
		slices. It then construct an ECAT image for the whole
		brain. The overlapped slices is constructed in such a
		way that the noise is minimum.

  History:	Created by Tom Yang on 05/05/93. 
_________________________________________________________________*/

#include <petutil/imglist.h>
#include "imageweld.h"

static char rcsid []= "$Id$";

main (argc, argv)
	int	argc;
	char	**argv;
{
	BOOLEAN			demigrate;
	IMAGE_LIST_DATA		*image_list;
	PARAMETER_DATA		parameter_data;
	char			complete_image_files [6][MAXPATH];
	char			complete_sxrfile [MAXPATH];
	char			*image_list_file;
	char			*parameter_list_file;
	char			short_image_files [6][MAXLINE];
	char			short_sxrfile [MAXLINE];
	char			short_weldedimg_files [6][MAXLINE];
	float			bottomT4 [4][4];
	float			bottom_scaling_factor;
	float			normalization_factor;
	float			topT4 [4][4];
	float			top_scaling_factor;
	int			filter_mode;
	int			i;
	int			image_mode;
	int			j;
	int			k;
	int			num_image_files;
	int			num_image_list;

/*
 * Get command line arguments.
 */

	parameter_list_file	= argv [1];
	image_list_file		= argv [2];

/* 
 * Read parameters file into the structure INFO_TYPE finfo.
 */

	if (readparm (parameter_list_file, &parameter_data) == FAIL) {
		fprintf (stderr, "Error: from readparm ().\n");
		free (image_list);
		exit (FAIL);
	}

/* 
 * Allocate memory for an array of image list structures. 
 */

	image_list	= (IMAGE_LIST_DATA *) pkg_malloc (sizeof (IMAGE_LIST_DATA) 
			* MAX_IMAGE_LIST_SIZE, "imageweld", "image_list");

/* 
 * Read image_list_file into IMAGE_LIST_DATA structure image_list.
 */

	if (readimglist (image_list_file, image_list, &num_image_list) == FAIL) {
		fprintf (stderr, "Error: from readimglist ().\n");
		free (image_list);
		exit (FAIL);
	}

/* 
 * Compute the mean for each image pair.
 */

	demigrate	= TRUE;
	image_mode	= parameter_data.weld_lstmode;	
	filter_mode	= RECFILTER_MODE;
	for (i = 0; i < num_image_list; i++) {

/*
 * Build the short names for top and bottom scans.
 */

		if (build_shortnames (parameter_data, image_list [i], image_mode, filter_mode, 
		&num_image_files, short_image_files, short_sxrfile) == FAIL) {
			fprintf (stderr, 
				"Error in building short names from image list %s %s %f %s %f %s\n",
				image_list [i].subject_id, 
				image_list [i].scanset1_id, image_list [i].mean1, 
				image_list [i].scanset2_id, image_list [i].mean2, 
				image_list [i].sxrid);
		}

/*
 * Clean the array of complete image files.
 */

		for (j = 0; j < num_image_files; j++)
			strcpy (complete_image_files [j], "");

/*
 * Get the complete top and bottom scans files.
 */
		if (checkimg (parameter_data, image_list [i], image_mode, demigrate, 
		num_image_files, short_image_files, complete_image_files) == FAIL) {
			fprintf (stderr, 
				"Error in checking files from image list %s %s %f %s %f %s\n\n",
				image_list [i].subject_id, 
				image_list [i].scanset1_id, image_list [i].mean1, 
				image_list [i].scanset2_id, image_list [i].mean2, 
				image_list [i].sxrid);
			exit (FAIL);
		}

/*
 * Build the short whole scan files.
 */
		if (parameter_data.weld_lstmode == STDIMAGE_MODE) {
			build_shortnames (parameter_data, image_list [i], WELDED_IMAGE_MODE, 
			filter_mode, &num_image_files, short_weldedimg_files, short_sxrfile);
		} else if (parameter_data.weld_lstmode == PREWELDED_IMAGE_MODE) {
			build_shortnames (parameter_data, image_list [i], STDIMAGE_MODE, 
			filter_mode, &num_image_files, short_weldedimg_files, short_sxrfile);
		} else {
			fprintf (stderr, "Error: Unknown weld_lstmode: %d in the parameter file %s.\n",
				parameter_list_file);
			exit (FAIL);
		}

/*______________________________________________________
 * Welds the top and bottom scans together.
 *______________________________________________________*/

		for (j = 0; j < num_image_files; j++) {
			if (BuildTransform (parameter_data, image_list [i], image_mode, 
					short_image_files [2 * j], short_image_files [2 * j + 1], 
					complete_image_files [2 * j], complete_image_files [2 * j + 1],
					topT4, bottomT4, &top_scaling_factor, &bottom_scaling_factor,
					&normalization_factor) == FAIL) {
				fprintf (stderr, "Error: BuildTransform () failed.\n");
				exit (FAIL);
			}

			if (WeldImages (complete_image_files [2 * j], 
					complete_image_files [2 * j + 1], short_weldedimg_files [j],
					topT4, bottomT4, top_scaling_factor, bottom_scaling_factor,
					normalization_factor) == FAIL) {
				fprintf (stderr, "Error: WeldedImages () failed.\n");
				exit (FAIL);
			}
		}
	}

	free (image_list);
}
