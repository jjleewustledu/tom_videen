/*$Id: create_matrixx_data.c,v 1.3 1995/10/31 14:54:21 tom Exp $*/
/*$Log: create_matrixx_data.c,v $
 * Revision 1.3  1995/10/31  14:54:21  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1995/10/13  16:02:50  ty7777
 * Use matrix7.h
 *
 * Revision 1.1  1995/10/10  15:31:42  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)create_matrixx_data.c	1/15/93 Copyright Neural Pet Group, Washington University 1992";
#endif
static char rcsid []= "$Header: /home/npggw/tom/src/libpetf77/RCS/create_matrixx_data.c,v 1.3 1995/10/31 14:54:21 tom Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	create_matrixx_data.c
 *	Function Name:	CreateMatrixData
 *	Arguments:	image_subheader:	image subheader used to fill the fields
 *						of the MatrixData structure.
 *	Return Value:	MatrixData *:		Pointer to a MatrixData structure.
 *						Free memory after the pointer is no 
 *						longer used with free_matrix_data () 
 *						from the CTI library.
 *	Description:	Create and initialize a MatrixData structure. 
 *	Called by:	filter.c in filter.bin
 *	Calling:	calloc (), bcopy ().
 *	Author:		Tom Yang
 *	History:	Created by Tom Yang on 01/15/1993
 *
___________________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

MatrixData *CreateMatrixData (image_subheader)
	Image_subheader	*image_subheader;
{
	MatrixData	*matrix_data;

	matrix_data = (MatrixData *) calloc (1, sizeof (MatrixData));

	matrix_data->shptr		= (caddr_t) image_subheader;		
	matrix_data->mat_type		= PetImage;
	matrix_data->data_type		= image_subheader->data_type;
	matrix_data->xdim		= image_subheader->x_dimension;
	matrix_data->ydim		= image_subheader->y_dimension;
	matrix_data->zdim		= image_subheader->z_dimension;
	matrix_data->data_size		= matrix_data->xdim * matrix_data->ydim * sizeof (short);
	matrix_data->scale_factor	= image_subheader->scale_factor;
	matrix_data->pixel_size		= image_subheader->x_pixel_size;

	return matrix_data;
}
