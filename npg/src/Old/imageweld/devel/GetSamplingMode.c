/*$Id: GetSamplingMode.c,v 1.1 1993/09/08 16:37:02 ty7777 Exp $*/
/*$Log: GetSamplingMode.c,v $
 * Revision 1.1  1993/09/08  16:37:02  ty7777
 * Initial revision
 **/

#include "imageweld.h"
#include <petutil/matrix.h>

static char rcsid [] = "$Id";

PUBLIC int GetSamplingMode (ecat_filename, mode)
	char	*ecat_filename;
	int	*mode;
{
	MatrixData	*matrix_read ();
	MatrixFile	*matrix_open ();

	Image_subheader	*image_subheader;
	Main_header	*main_header;
	MatrixData	*matrixx_data;
	MatrixFile	*matfptr;
	Mlist		*mlist;
	int		diff;  /* number of differences between spec and actual */

	matfptr	= matrix_open (ecat_filename, "r");
	if (matfptr == NULL)
	{
		fprintf (stderr, "Error: matrix_open in GetSamplingMode ().\n");
		return FAIL;
	}

	main_header	= matfptr->mhptr;

	/*
	 * Check if the ECAT file is a really an ECAT image.
	 */
	if (main_header->data_type != IMAGE_DATA)
	{
		fprintf (stderr, "Error: Data type is not compatible with image data type.\n");
		matrix_close (matfptr);
		return FAIL;
	}

	/*
	 * The number of mats in a list always gives the correct number of slices.
 	 */
	mlist	= make_list (ecat_filename, (char *) NULL, "*;*;*;*", &diff);
	if (!mlist) 
	{
		fprintf (stderr, "make_list error in GetSamplingMode ().\n");
		matrix_close (matfptr);
		return FAIL;
	}

	/*
 	 * Get the image subheader for slice 1. 
 	 */
	matrixx_data	= matrix_read (matfptr, mlist->list [0]);
	if (matrixx_data == NULL)
	{
		fprintf (stderr, "Error: matrixx_read in GetSamplingMode ().\n");
		matrix_close (matfptr);
		free_Mlist (mlist);
		return FAIL;
	}
	image_subheader	= (Image_subheader *) matrixx_data->shptr;

	if (main_header->wobble_speed == 0 && image_subheader->filter_params [0] == 1.0)
		*mode	= SAMPLING_MODE_3D;
	else 
		*mode	= SAMPLING_MODE_2D;

	free_Mlist (mlist);
	free_matrix_data (matrixx_data);
	matrix_close (matfptr);

	return SUCCEED;
}
