/*$Id: OpenEcat.c,v 1.3 1995/10/31 14:54:16 tom Exp $*/
/*$Log: OpenEcat.c,v $
 * Revision 1.3  1995/10/31  14:54:16  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1995/10/13  16:02:08  ty7777
 * Use matrix7.h.
 *
 * Revision 1.1  1995/10/10  15:29:08  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)OpenEcat.c	10/09/92 Copyright Neural Pet Group, Washington University 1992";
#endif
static char rcsid []= "$Id: OpenEcat.c,v 1.3 1995/10/31 14:54:16 tom Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	OpenEcat.c
 *	Function Name:	OpenEcat
 *	Arguments:	fd:			ECAT file descriptor.
 *			ecat_image_file:	ECAT image file name.	
 *			num_slices:		number of slices in image file,
 *						returned.
 *			xdim:			x-dimension, returned.
 *			ydim:			y-dimension, returned.
 *			matval:			structure containing frame, plane,
 *						gate, data, and bed numbers.
 *	Return Value:	MatrixFile *:		Pointer to a MatrixFile structure.
 *						Free memory after the pointer is no 
 *						longer used with the matrix_close ()
 *						function from the CTI library.
 *	Description:	Open the ECAT image file and return matfptr, num_slices,
 *			xdim, ydim.
 *	Called by:	ecat2pett6.c in ecat2pett6.bin
 *			getimage_type.c in libpetutil.a
 *	Calling:	matrixx_open (), matrixx_read ().
 *	Author:		Tom Yang
 *	History:	Created by Tom Yang on 04/13/1992
 *
___________________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

MatrixFile *OpenEcat (fd, ecat_image_file, num_slices, xdim, ydim, 
			pixel_size, plane_separation, matval)
	int		fd;
	char		*ecat_image_file;
	short		*num_slices;
	short		*xdim;
	short		*ydim;
	float		*pixel_size;
	float		*plane_separation;
	struct Matval	matval;
{
	MatrixFile	*pet_matrix_open ();
	MatrixData	*matrix_read ();

	Image_subheader		*image_subheader;
	Main_header		*main_header;
	MatDirNode		*node;
	MatrixData		*volume;
	MatrixFile		*matfptr;

	matfptr	= pet_matrix_open (fd, ecat_image_file, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	main_header		= matfptr->mhptr;
	if (matfptr == NULL || (main_header->file_type != PetImage 
	&& main_header->file_type != PetVolume && main_header->file_type != ByteImage 
	&& main_header->file_type != ByteVolume && main_header->file_type != InterfileImage))
	{
		if (matfptr != NULL)
			fprintf (stderr, 
	"Error: incompatible data_type: %d. Correct data type should be: %d, %d %d, %d, or %d\n", 
				main_header->file_type, PetImage, PetVolume, ByteImage,
				ByteVolume, InterfileImage);
		fprintf (stderr, "Error in opening matrix file %s.\n", ecat_image_file);
		*num_slices	= 0;
		*xdim		= 0;
		*ydim		= 0;

		return NULL;
	}

	*plane_separation	= main_header->plane_separation;

	*num_slices	= 0;
	node		= matfptr->dirlist->first;
	while (node) 
	{
		volume		= matrix_read (matfptr, node->matnum, MAT_SUB_HEADER);
		*num_slices	+= volume->zdim;
		free_matrix_data (volume);
		node = node->next;
	}

	/*
	 * Get the dimension and pixel_size of slice.
	 */
	node		= matfptr->dirlist->first;
	volume		= matrix_read (matfptr, node->matnum, MAT_SUB_HEADER);
	image_subheader	= (Image_subheader *) volume->shptr;

	*xdim		= image_subheader->x_dimension; 
	*ydim		= image_subheader->y_dimension; 
	*pixel_size	= image_subheader->x_pixel_size;

	free_matrix_data (volume);

	return matfptr;
}
