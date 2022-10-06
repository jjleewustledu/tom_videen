/*	$Id: OpenEcat.c,v 1.21 1995/11/10 23:29:58 tom Exp $
	$Log: OpenEcat.c,v $
 * Revision 1.21  1995/11/10  23:29:58  tom
 * *** empty log message ***
 * */
/* ===============================================================================
 *	Module:			OpenEcat.c
 *	Date:			04/13/1992
 *	Author:			Tom Yang
 *	Description:	Open an ECAT image file and return
 *					matfptr, xdim, ydim. num_slices, pixel_size, and plane_separation.
 *	Calls:			matrix_open, matrix_read, free_matrix_data
 *	Returns:		pointer to a MatrixFile structure or NULL.
 *	Note:			memory must be freed externally with matrix_close
 *	History:
 *		10-Nov-95	TOV modification
 * ===============================================================================
 */
#include <petutil.h>
#include <matrix7.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/OpenEcat.c,v 1.21 1995/11/10 23:29:58 tom Exp $";

MatrixFile     *OpenEcat (ecat_image_file, num_slices, xdim, ydim,
			                  pixel_size, plane_separation, matval)
	char           *ecat_image_file;	/* filename */
	short          *num_slices;			/* number of slices */
	short          *xdim;				/* number of pixels */
	short          *ydim;				/* number of pixels */
	float          *pixel_size;	
	float          *plane_separation;
	struct Matval   matval;				/* CTI structure (no longer used) */
{
	MatrixFile     *matrix_open ();
	MatrixData     *matrix_read ();

	Image_subheader *image_subheader;
	Main_header    *main_header;
	MatDirNode     *node;
	MatrixData     *volume;
	MatrixFile     *matfptr;

	matfptr = matrix_open (ecat_image_file, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	main_header = matfptr->mhptr;
	if (matfptr == NULL || (main_header->file_type != PetImage
		     && main_header->file_type != PetVolume && main_header->file_type != ByteImage
	   && main_header->file_type != ByteVolume && main_header->file_type != InterfileImage)) {
		if (matfptr != NULL) fprintf (stderr, "ERROR [OpenEcat]: ",
			 "incompatible data_type: %d. Correct data type should be: %d, %d %d, %d, or %d\n",
			 main_header->file_type, PetImage, PetVolume, ByteImage, ByteVolume, InterfileImage);
		fprintf (stderr, "ERROR [OpenEcat]: cannot open %s\n", ecat_image_file);
		*num_slices = 0;
		*xdim = 0;
		*ydim = 0;
		return NULL;
	}
	*plane_separation = main_header->plane_separation;
	*num_slices = 0;
	node = matfptr->dirlist->first;
	while (node) {
		volume = matrix_read (matfptr, node->matnum, MAT_SUB_HEADER);
		*num_slices += volume->zdim;
		free_matrix_data (volume);
		node = node->next;
	}

/*
 * Get the dimension and pixel_size of slice.
 */
	node = matfptr->dirlist->first;
	volume = matrix_read (matfptr, node->matnum, MAT_SUB_HEADER);
	image_subheader = (Image_subheader *) volume->shptr;

	*xdim = image_subheader->x_dimension;
	*ydim = image_subheader->y_dimension;
	*pixel_size = image_subheader->x_pixel_size;

	free_matrix_data (volume);
	return matfptr;
}
