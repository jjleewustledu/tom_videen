/*$Id: OpenEcat.c,v 1.14 1993/11/03 17:43:18 ty7777 Exp $*/
/*$Log: OpenEcat.c,v $
 * Revision 1.14  1993/11/03  17:43:18  ty7777
 * delete ssccsid.
 *
 * Revision 1.13  1993/10/27  21:23:07  ty7777
 * Added checking for frames.
 *
 * Revision 1.12  1993/10/27  20:27:41  ty7777
 * Change error message for make_list.
 *
 * Revision 1.11  1993/10/26  14:35:07  ty7777
 * Added processing if matval.frame is <= 0.
 *
 * Revision 1.10  1993/10/26  14:17:48  ty7777
 * Modified to use different frames.
 *
 * Revision 1.9  1993/10/25  20:46:17  ty7777
 * Change rcsid content from id to header.
 *
 * Revision 1.8  1993/10/25  20:43:58  ty7777
 * Modified so that it can open any plane in any frame and plane.
 *
 * Revision 1.7  1993/08/06  16:31:38  ty7777
 * Changed error message.
 *
 * Revision 1.6  1993/06/22  22:30:37  ty7777
 * matval is nonger used.
 * matval is no longer used.
 *
 * Revision 1.5  1993/04/28  16:01:45  ty7777
 * Make sure we get the correct number of slices by calling make_list.
 *
 * Revision 1.4  1993/04/21  16:59:40  ty7777
 * initial checking in.
 *
 * Revision 1.3  1993/03/08  18:14:52  ty7777
 * Before changing filter code and filter parameters.
 *
 * Revision 1.2  1993/03/04  17:09:00  ty7777
 * Adding error checking.
 *
 * Revision 1.1  1993/03/03  20:32:05  ty7777
 * Initial revision
 **/

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/OpenEcat.c,v 1.14 1993/11/03 17:43:18 ty7777 Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	OpenEcat.c
 *	Function Name:	OpenEcat
 *	Arguments:	fd:			ECAT file descriptor, not used
 *						currently. Reserved for latter use 
 *						for the function to work with 
 *						FORTRAN functions.
 *			ecat_image_file:	ECAT image file name.	
 *			num_slices:		number of slices in image file,
 *						returned.
 *			xdim:			x-dimension, returned.
 *			ydim:			y-dimension, returned.
 *			matval:			structure containing frame, plane,
 *						gate, data, and bed numbers. Not used,
 *						this is here just for backward 
 *						compability with earlier versions of
 *						libpetutil.a and libhp.a.
 *	Return Value:	MatrixFile *:		Pointer to a MatrixFile structure or
 *						NULL if there is error. The user is
 *						responsible to free memory after the 
 *						MatrixFile pointer is no longer used 
 *						by using  the matrix_close ()
 *						function from the CTI library or our
 *						own library libcti.a.
 *	Description:	Open the ECAT image file and return matfptr, xdim, ydim, 
 *			num_slices, pixel_size, and plane_separation.
 *	Called by:	ecat2pett6.c in ecat2pett6.bin
 *			getimage_type.c in libpetutil.a
 *	Calling:	matrix_open (), matrix_read ().
 *	Author:		Tom Yang
 *	History:	Created by Tom Yang on 04/13/1992
 *
___________________________________________________________________________________*/

#include <petutil/petutil.h>
#include <petutil/matrix.h>

MatrixFile *OpenEcat (ecat_image_file, num_slices, xdim, ydim, 
			pixel_size, plane_separation, matval)
	char		*ecat_image_file;
	short		*num_slices;
	short		*xdim;
	short		*ydim;
	float		*pixel_size;
	float		*plane_separation;
	struct Matval	matval;
{
	MatrixFile	*matrix_open ();
	MatrixData	*matrix_read ();
	int		mat_numcod ();

	Image_subheader	*image_subheader;
	Main_header	*main_header;
	MatrixData	*matrixx_data;
	MatrixFile	*matfptr;
	Mlist		*mlist;
	char		list [MAXLINE];
	int		diff;  /* number of differences between spec and actual */

	matfptr	= matrix_open (ecat_image_file, "r");
	if (matfptr == NULL || matfptr->mhptr->data_type != IMAGE_DATA)
	{
		if (matfptr != NULL)
			printf ("Incompatible data_type: %d. Correct data type should be: %d\n", 
				matfptr->mhptr->data_type, IMAGE_DATA);
		fprintf (stderr, "Error in opening matrix file %s.\n", ecat_image_file);
		*num_slices	= 0;
		*xdim		= 0;
		*ydim		= 0;

		return NULL;
	}

	main_header		= matfptr->mhptr;
	*plane_separation	= main_header->plane_separation;

	/*
	 * Get the number of frames in image file.
	 */
	strcpy (list, "*;*;*;*");
	mlist	= make_list (ecat_image_file, (char *) NULL, list, &diff);
	if (!mlist) 
	{
		fprintf (stderr, "make_list error in OpenEcat () for image %s.\n", ecat_image_file);
		fprintf (stderr, "list is %s.\n", list);
		matrix_close (matfptr);
		return NULL;
	}

	/*
 	 * The number of mats in a list always gives the correct number of
 	 * slices.
 	 */
	if (matval.frame > 0 && matval.frame <= mlist->frames)
	{
		free_Mlist (mlist);
		sprintf (list, "%d;*;*;*", matval.frame);
		mlist	= make_list (ecat_image_file, (char *) NULL, list, &diff);
		if (!mlist) 
		{
			fprintf (stderr, "make_list error in OpenEcat () for image %s.\n", ecat_image_file);
			fprintf (stderr, "list is %s.\n", list);
			matrix_close (matfptr);
			return NULL;
		}
	}
	*num_slices	= mlist->nmats;

	/*
	 * Get the dimension and pixel_size of slice.
	 */
	matrixx_data	= matrix_read (matfptr, mlist->list [0]);
	image_subheader	= (Image_subheader *) matrixx_data->shptr;
	*xdim		= image_subheader->dimension_1; 
	*ydim		= image_subheader->dimension_2; 
	*pixel_size	= image_subheader->pixel_size;

	free_Mlist (mlist);
	free_matrix_data (matrixx_data);

	return matfptr;
}
