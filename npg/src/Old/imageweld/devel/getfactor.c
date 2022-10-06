/*$Id: getfactor.c,v 1.3 1993/10/19 20:58:57 ty7777 Exp $*/
/*$Log: getfactor.c,v $
 * Revision 1.3  1993/10/19  20:58:57  ty7777
 * Adding documentation.
 *
 * Revision 1.2  1993/10/19  19:08:21  ty7777
 * Transform each pixel to get rid of movement artifacts.
 *
 * Revision 1.1  1993/10/15  20:52:05  ty7777
 * Initial revision
 **/

/*_________________________________________________________________
  File:		getfactor.c

  Function:	int getfactor (PARAMETER_DATA parameter_data, 
		IMAGE_LIST_DATA image_list, int image_mode, BOOLEAN demigrate, 
		MATRIX_DATA *matrix_list, int list_size, char *complete_top_scan, 
		char *complete_bottom_scan, float *normalization_factor, 
		BOOLEAN *top_bottom_flipped)

  Arguments:	parameter_data,		structure containing the necessary data
					for searching and reading files.
		image_list,		a structure which specify the scan id,
					scan number, etc.			
		image_mode,		image mode, used for displaying error
					messages.
		demigrate:		TRUE: migrate data files; FALSE: no
					migration.
		matrix_list,		a list of structures correspoing to
					data in a matrix file.
		list_size,		size of matrix_list.
		complete_top_scan,	complete file name for the top scan.
		complete_bottom_scan,	complete file name for the bottom scan.
		normalization_factor,	normalization factor applied to the
					bottom scan to make it compatible with
					the top scan, returned.
		top_bottom_flipped,	TRUE: the top ans the bottom scans are
					changed; FALSE: no change.

  Return Value:	SUCCEED:	successful;
		FAIL:		failed.

  Description:	This function get the normalization factor between
		the top and bottom reference scans.

  Calling:	FindReferenceScans () in this file.	
		GetIndexingScanOffset () in GetIndexingScanOffset.c.
   
		Functions in libhp.a includes:
		getimagedim ()
		getrealimg ()

  Called by:	BuildTransform () in BuildTransform.c.

  Author:	Created by Tom Yang on 10/14/93. 
_________________________________________________________________*/

#include <petutil/imglist.h>
#include <petutil/matrix.h>
#include "imageweld.h"

static char rcsid []= "$Id: getfactor.c,v 1.3 1993/10/19 20:58:57 ty7777 Exp $";

PRIVATE int FindReferenceScans ();

PUBLIC int getfactor (parameter_data, image_list, image_mode, demigrate, 
matrix_list, list_size, complete_top_scan, complete_bottom_scan, 
normalization_factor, top_bottom_flipped)
	PARAMETER_DATA		parameter_data;
	IMAGE_LIST_DATA		image_list;
	int			image_mode;
	BOOLEAN			demigrate;
	MATRIX_DATA		*matrix_list;
	int			list_size;
	char			*complete_top_scan;
	char			*complete_bottom_scan;
	float			*normalization_factor;
	BOOLEAN			*top_bottom_flipped;
{
	Image_subheader	**bottom_subheader_list;
	Image_subheader	**top_subheader_list;
	Main_header	*bottom_main_header;
	Main_header	*top_main_header;
	float		*bottom_image3d;
	float		*bottom_image3d_overlap;
	float		bottom_overlap_sum;
	float		offset;
	float		*overlap_image3d;
	float		*top_image3d;
	float		*top_image3d_overlap;
	float		top_overlap_sum;
	float		pixel_size;
	float		plane_separation;
	char		complete_reference_scans [2][MAXPATH];
	char		short_reference_scans [2][MAXLINE];
	int		bottom_scan_overlap_end;
	int		dimension;
	int		i;
	int		j;
	int		num_overlapped_slices;
	int		num_shifted_slices;
	int		sampling_mode;
	int		scanner;
	int		slice_dim;
	int		top_scan_overlap_start;
	short		bottom_num_slices;
	short		height;
	short		num_slices;
	short		top_num_slices;
	short		width;
	struct Matval	matval;

	/* 
	 * Get the reference scans and compute the normalization factor.
	 */
	if (FindReferenceScans (matrix_list, list_size, short_reference_scans) == FAIL
	|| (checkimg (parameter_data, image_list, image_mode, demigrate, 
	2, short_reference_scans, complete_reference_scans)) == FAIL)
	{
		strcpy (complete_reference_scans [0], complete_top_scan);
		strcpy (complete_reference_scans [1], complete_bottom_scan);
	}

	/*______________________________________________________
	 * Assign default values for matval.
	 *______________________________________________________*/
	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	/*______________________________________________________
	 * Get image type and do error checking.
	 *______________________________________________________*/
	if (getimagedim (complete_reference_scans [0], matval, &scanner, &width, &height, 
	&top_num_slices, &pixel_size, &pixel_size, &plane_separation) == FAIL) 
	{
		fprintf (stderr, 
		"Error: getimagedim () failed for image %s.\n", 
		complete_reference_scans [0]);
		return FAIL;
	}

	if (scanner != ECAT_SCANNER)
	{
		fprintf (stderr, 
		"Error: top scan file %s is not an ECAT image.\n", 
		complete_reference_scans [0]);
		return FAIL;
	}

	if (getimagedim (complete_reference_scans [1], matval, &scanner, &width, &height, 
	&bottom_num_slices, &pixel_size, &pixel_size, &plane_separation) == FAIL) 
	{
		fprintf (stderr, 
		"Error: getimagedim () failed for image %s.\n",
		complete_reference_scans [1]);
		return FAIL;
	}

	if (scanner != ECAT_SCANNER)
	{
		fprintf (stderr, 
		"Error: bottom scan file %s is not an ECAT image.\n", 
		complete_reference_scans [1]);
		return FAIL;
	}

	/*______________________________________________________
	 * Get the offset for the indexing scan.
	 *______________________________________________________*/
	if (GetIndexingScanOffset (complete_reference_scans [0], 
	complete_reference_scans [1], &offset, top_bottom_flipped, FALSE) == FAIL)
	{
		fprintf (stderr, "Error: in GetIndexingScanOffset ().\n");
		return FAIL;
	}

	if (*top_bottom_flipped)
	{
		if ((strcmp (complete_reference_scans [0], complete_top_scan) == 0 &&
		strcmp (complete_reference_scans [1], complete_bottom_scan) == 0) ||
		(strcmp (complete_reference_scans [1], complete_top_scan) == 0 &&
		strcmp (complete_reference_scans [0], complete_bottom_scan) == 0))
			*top_bottom_flipped	= TRUE;
		else
			*top_bottom_flipped	= FALSE;
	}

	/*______________________________________________________
	 * Get the number of the overlapped slices.
	 *______________________________________________________*/
	num_shifted_slices	= ROUND (offset / (plane_separation * MMS_PER_CM));
	num_overlapped_slices	= top_num_slices - num_shifted_slices;

	if (top_num_slices != bottom_num_slices)
	{
		fprintf (stderr, 
		"Error: the top scan %s and bottom scan %s have unequal number of slices\n",
			complete_reference_scans [0], complete_reference_scans [1]);
		return FAIL;
	}

	if (num_overlapped_slices > top_num_slices)
	{
		fprintf (stderr, 
		"Error: number of overlapped slices %d is greater than %d.\n", 
		num_overlapped_slices, top_num_slices);
		return FAIL;
	}
	else if (num_overlapped_slices < 0)
	{
		fprintf (stderr, 
		"Error: number of overlapped slices %d is less than 0.\n", 
		num_overlapped_slices);
		return FAIL;
	}

	/*______________________________________________________
	 * Get data from the top scan.
	 *______________________________________________________*/
	top_subheader_list	= (Image_subheader **) pkg_malloc (top_num_slices 
				* sizeof (caddr_t), "getscale", "top_subheader_list");

	slice_dim	= width * height;
	dimension	= slice_dim * top_num_slices;
	top_image3d	= (float *) pkg_malloc (dimension * sizeof (float), 
			"getscale", "top_image3d");
	top_main_header	= (Main_header *) calloc (1, sizeof (Main_header));

	if (getrealimg (complete_reference_scans [0], scanner, matval, width, height, 
	top_num_slices, top_image3d, top_main_header, top_subheader_list) == FAIL)
	{
		fprintf (stderr, "Error: getrealimg () failed.\n");
		for (i = 0; i < top_num_slices; i++)
		{
			free (top_subheader_list [i]);
		}
		free (top_subheader_list);
		free (top_main_header);
		free (top_image3d);
		return FAIL;
	}

	/*______________________________________________________
	 * Get data from the bottom scan.
	 *______________________________________________________*/
	bottom_subheader_list	= (Image_subheader **) pkg_malloc (bottom_num_slices 
				* sizeof (caddr_t), "getscale", "bottom_subheader_list");

	bottom_image3d		= (float *) pkg_malloc (dimension * sizeof (float), 
						"getscale", "bottom_image3d");
	bottom_main_header	= (Main_header *) calloc (1, sizeof (Main_header));

	if (getrealimg (complete_reference_scans [1], scanner, matval, width, height, 
	bottom_num_slices, bottom_image3d, bottom_main_header, bottom_subheader_list) == FAIL)
	{
		fprintf (stderr, "Error: getrealimg () failed.\n");
		for (i = 0; i < top_num_slices; i++)
		{
			free (top_subheader_list [i]);
			free (bottom_subheader_list [i]);
		}
		free (top_subheader_list);
		free (top_main_header);
		free (top_image3d);
		free (bottom_subheader_list);
		free (bottom_main_header);
		free (bottom_image3d);
		return FAIL;
	}

	/*______________________________________________________
	 * Get scaling factor;
	 *______________________________________________________*/
	top_scan_overlap_start	= top_num_slices - num_overlapped_slices + 1;
	bottom_scan_overlap_end	= num_overlapped_slices;
	top_image3d_overlap	= top_image3d + top_scan_overlap_start * slice_dim;
	bottom_image3d_overlap	= bottom_image3d + slice_dim;

	top_overlap_sum		= 0.0;
	bottom_overlap_sum	= 0.0;
	j			= (num_overlapped_slices  - 2) * slice_dim;
	for (i = 0; i < j; i++)
	{
		top_overlap_sum		+= top_image3d_overlap [i];
		bottom_overlap_sum	+= bottom_image3d_overlap [i];
	}

	if (bottom_overlap_sum != 0.0)
		*normalization_factor	= top_overlap_sum / bottom_overlap_sum;
	else
	{
		fprintf (stderr, 
		"Error: the sum for the bottom scan %s is 0.0\n", 
		complete_reference_scans [1]);
		for (i = 0; i < top_num_slices; i++)
		{
			free (top_subheader_list [i]);
			free (bottom_subheader_list [i]);
		}
		free (top_subheader_list);
		free (top_main_header);
		free (top_image3d);
		free (bottom_subheader_list);
		free (bottom_main_header);
		free (bottom_image3d);
		return FAIL;
	}

	fprintf (stdout, "Global bottom image multiplier = %f\n\n", *normalization_factor);
	fprintf (stdout, "top	bottom	top/bottom	scaled\n");   
	for (i = 0; i < num_overlapped_slices - 2; i++)
	{
		top_overlap_sum		= 0.0;
		bottom_overlap_sum	= 0.0;
		for (j = 0; j < slice_dim; j++)
		{
			top_overlap_sum		+= top_image3d_overlap [i * slice_dim + j];
			bottom_overlap_sum	+= bottom_image3d_overlap [i * slice_dim + j];
		}
		fprintf (stdout, "%d	%d	%f	%f\n", 
				top_scan_overlap_start + i + 1, 1 + (i + 1), 
				top_overlap_sum / bottom_overlap_sum, 
				top_overlap_sum / (bottom_overlap_sum * *normalization_factor));
	}

	for (i = 0; i < top_num_slices; i++)
	{
		free (top_subheader_list [i]);
		free (bottom_subheader_list [i]);
	}
	free (top_subheader_list);
	free (top_main_header);
	free (top_image3d);

	free (bottom_subheader_list);
	free (bottom_main_header);
	free (bottom_image3d);

	return SUCCEED;
}


PRIVATE int FindReferenceScans (matrix_list, list_size, short_reference_scans)
	MATRIX_DATA	*matrix_list;
	int		list_size;
	char		short_reference_scans [][MAXLINE];
{
	int	i;
	int	j;

	j	= 0;
	for (i = 0; i < list_size && j < 2; i++)
	{
		if (matrix_list [i].scale_factor == 1.00000
		&& matrix_list [i].displacement [0] == 0.0000
		&& matrix_list [i].displacement [1] == 0.0000
		&& matrix_list [i].displacement [2] == 0.0000
		&& matrix_list [i].rotation_angle [0] == 0.0000
		&& matrix_list [i].rotation_angle [1] == 0.0000
		&& matrix_list [i].rotation_angle [2] == 0.0000)
		{
			strcpy (short_reference_scans [j], matrix_list [i].scanid);
			strcat (short_reference_scans [j], IMAGE_EXT);
			j++;
		}
	}

	if (j == 2)
		return	SUCCEED;
	else
		return	FAIL;
}
