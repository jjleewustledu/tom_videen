/*$Log$*/

/*_________________________________________________________________
  File:		BuildTransform.c

  Function:	int BuildTransform (PARAMETER_DATA parameter_data, 
		IMAGE_LIST_DATA image_list, int image_mode, char *top_short_file,
		char *bottom_short_file, float topT4 [][4], float bottomT4 [][4],
		float *normalization_factor);

  Arguments:	
		parameter_data:		data structure contains all the parameters from
					the parameter file. PARAMETER_DATA is defined in
					/usr/local/include/petutil/imglist.h.
		image_list:		data structure containing scan identification,
					activation scan number, and control scan number.
					IMAGE_LIST_DATA is defined in 
					/usr/locla/include/petutil/imglist.h.
		image_mode:		image mode. Used here to determine the image
					category.
		top_short_file:		short file name for top scan.
		bottom_short_file:	short file name for bottom scan.
		topT4:			T4 matrix for top scan.
		bottomT4:		T4 matrix for bottom scan.
		normalization_factor:	normalization factor between the
					top and bottom scans.

  Return Value:	SUCCEED:	successful;
		FAIL:		failed.

  Description:	This function reads the matrix file and returns the
		T4 matrices for both the top and the bottom scans. The 
		normalization factor between the top and the bottom is 
		also returned.

  Calling:	readmat () in readmat.c.

  Called by:

  Author:	Created by Tom Yang on 10/13/93. 
_________________________________________________________________*/

#include <petutil/imglist.h>
#include "imageweld.h"

#define	MATRIX_FILE_EXT		".mat"

PRIVATE int FindT4 ();

static char rcsid []= "$Id: BuildTransform.c,v 1.1 1993/10/15 20:50:38 ty7777 Exp $";

PUBLIC int BuildTransform (parameter_data, image_list, image_mode, short_top_scan, 
	short_bottom_scan, complete_top_scan, complete_bottom_scan, topT4, 
	bottomT4, top_scaling_factor, bottom_scaling_factor, normalization_factor)
	PARAMETER_DATA		parameter_data;
	IMAGE_LIST_DATA		image_list;
	int				image_mode;
	char			*short_top_scan;
	char			*short_bottom_scan;
	char			*complete_top_scan;
	char			*complete_bottom_scan;
	float			topT4 [][4];
	float			bottomT4 [][4];
	float			*top_scaling_factor;
	float			*bottom_scaling_factor;
	float			*normalization_factor;
{
	BOOLEAN		demigrate;
	BOOLEAN		matrix_file_exist;
	BOOLEAN		top_bottom_flipped;
	MATRIX_DATA	*matrix_list;
	char		cbuffer [MAXLINE];
	char		complete_matrix_file [MAXPATH];
	char		short_matrix_file [MAXLINE];
	char		subject_id [MAXNAME];
	float		mmppix [3];
	int			list_size;

	demigrate		= TRUE;
	matrix_file_exist	= TRUE;

/* 
 * Construct the subject id.
 */

	strcpy (subject_id, parameter_data.species);
	strcat (subject_id, image_list.subject_id);
	strcpy (short_matrix_file, subject_id);
	strcat (short_matrix_file, MATRIX_FILE_EXT);

/* 
 * Find the matrix file and read data from matrix file.
 */

	if (CheckDataFile (parameter_data, image_list, demigrate, short_matrix_file, 
			complete_matrix_file) == FAIL) {
		matrix_file_exist	= FALSE;
	}

	matrix_list	= (MATRIX_DATA *) malloc (sizeof (MATRIX_DATA) 
			* MAX_MATRIX_LIST_SIZE);
	if (matrix_list == (MATRIX_DATA *) NULL) {
		fprintf (stderr, "Error: memory allocation for matrix_list in BuildTransform ().\n");
		return FAIL;
	}

	if (!matrix_file_exist)
		list_size	= 0;
	else if (readmat (complete_matrix_file, mmppix, matrix_list, &list_size) == FAIL)
		matrix_file_exist	= FALSE;

/* 
 * Get the reference scans and compute the normalization factor.
 */

	if (getfactor (parameter_data, image_list, image_mode, demigrate, 
			matrix_list, list_size, complete_top_scan, complete_bottom_scan, 
			normalization_factor, &top_bottom_flipped) == FAIL) {
		free (matrix_list);
		return FAIL;
	}

	if (top_bottom_flipped) {
		strcpy (cbuffer, short_top_scan);	
		strcpy (short_top_scan, short_bottom_scan);	
		strcpy (short_bottom_scan, cbuffer);	
	}

/* 
 * Find the top and bottom T4 matrices.
 */

	FindT4 (matrix_list, list_size, short_top_scan, mmppix, topT4, top_scaling_factor);
	FindT4 (matrix_list, list_size, short_bottom_scan, mmppix, bottomT4, bottom_scaling_factor);

	free (matrix_list);
	return SUCCEED;
}


PRIVATE int FindT4 (matrix_list, list_size, short_scan, mmppix, T4, scaling_factor)
	MATRIX_DATA	*matrix_list;
	int		list_size;
	char		*short_scan;
	float		*mmppix;
	float		T4 [][4];
	float		*scaling_factor;
{
	extern		t4set_ ();

	char	scanid [MAXLINE];
	int	i;
	int	index;
	int	j;

/*
 * Initialization.
 */

	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			if (i == j)
				T4 [i][j]	= 1.0;
			else
				T4 [i][j]	= 0.0;
		}
	}
	*scaling_factor	= 1.0;

	for (i = 0; i < list_size; i++) {
		if (strstr (short_scan, matrix_list [i].scanid) != NULL)
			break;
	}

	if (i < list_size) {	
		t4set_ (mmppix, matrix_list [i].displacement, matrix_list [i].rotation_angle,
			matrix_list [i].rotation_center, T4);
		*scaling_factor	= matrix_list [i].scale_factor;
	}

	return	SUCCEED;
}
