/*$Id: getimage.c,v 1.7 1995/10/30 22:39:01 tom Exp $*/
/*$Log: getimage.c,v $
 * Revision 1.7  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.6  1995/10/13  17:03:10  ty7777
 * Use matrix7.h
 *
 * Revision 1.5  1995/09/28  18:31:27  ty7777
 * Updated for 961 data.
 *
 * Revision 1.4  1995/05/18  19:19:32  ty7777
 * Same as the last version.
 *
 * Revision 1.3  1994/03/03  20:12:34  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/10/12  16:50:59  ty7777
 * Adding initialization for image array.
 *
 * Revision 1.1  1993/03/03  20:34:31  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)getimage.c	10/09/92  Copyright Neural Pet Group, Washington University 1992";
#endif
static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getimage.c,v 1.7 1995/10/30 22:39:01 tom Exp $";

/*____________________________________________________________________________
 *
 *	File Name:	getimage.c
 *	Function Name:	getimage ()
 *	Arguments:	slice_data:	image array for slice , must be allocated
 *					with memory and initialized to zero before 
 *					calling. The array size is ENCODED_SLICE_DIM.
 *					Altered upon return.	
 *			header:		header from file for specified slice, altered 
 *					upon return.
 *			fd:		image file descriptor, get it before calling
 *					by using the UNIX system interface call open ().
 *			image_name:	pointer to image file name.	
 *			slice_number:	number of the desired slice of the image, start 
 *					at one, must be assigned before calling.
 *			num_slices:	number of slices in the image, altered upon 
 *					return.
 *			scanner_type:	scanner number of the image (1, 6 - 8, 10), altered 
 *					upon return, -1 means error.
 *			err_value:	TRUE: if unable to open the disk file, unable to 
 *					read the desired slice, or if the file format is 
 *					incorrect, altered upon return.
 *	Return Value:	void.
 *	Description:	General function for reading image files. This function gets one 
 *			slice of an image from a disk file. ECAT, Decoded and encoded 
 *			PETT 6-8 images can be read (7 - 49 slices) (PETT6, Super PETT 
 *			I and Super PETT II). The encoding algorithm is that of ENCODE1 
 *			which was created in April 87 and differs from that used previously. 
 *			All images are returned in a 128 x 128 array (PETT6 images are 
 *			centered in the array).
 *	Called by:
 *	Calling:	pet_matrixx_open () in pet_matrixx.c of libpetutil.a.
 *			Ecat2Pett6 () in Ecat2Pett6.c of libpetutil.a.
 *			GetImagePett6 () in GetImagePett6.c of libpetutil.a.
 *			GetImageSuperPett () in GetImageSuperPett.c of libpetutil.a.
 *	Author:		Tom Yang. 
 *	History:	Created by Tom Yang on 04/17/1992.
 *
____________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>
#define	SCALE_FACTOR		1.00000000

void getimage (slice_data, header, fd, image_name, slice_number, 
	encoded_flag, num_slices, scanner_type, err_value)
short		*slice_data;
short		*header;
int		fd;
char		*image_name;
short		slice_number;
BOOLEAN		encoded_flag;
short		num_slices;
int		scanner_type;
BOOLEAN		*err_value;
{
	MatrixFile	*matrix_open ();

	MatDirNode	*node;
	MatrixFile	*matfptr;
	float		scale;
	int		i;
	int		image_type;
	short		xdim;
	short		ydim;
	struct Matval	matval;

	/*
	 * Zero images.
	 */
	for (i = 0; i < ENCODED_SLICE_DIM; i++)	
		slice_data [i]	= 0;

	if (scanner_type == ERROR_SCANNER || slice_number > num_slices
		|| slice_number < 1)
	{
		*err_value	= FAIL;
		return;
	}

	switch (scanner_type)
	{
		case ECAT_SCANNER:
			matfptr = matrix_open (image_name, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
			if (matfptr == NULL)
			{
				*err_value	= FAIL;
				return;
			}

			node		= matfptr->dirlist->first;
			mat_numdoc (node->matnum, &matval);
			matval.plane	= slice_number;
			scale		= SCALE_FACTOR;
			*err_value	= Ecat2Pett6 (matfptr, scale, 
				(PETT6HDR *) header, slice_data, num_slices, matval);

			matrix_close (matfptr);
			break;
		case PETT6_SCANNER:
			*err_value	= GetImagePett6 (fd, slice_number, encoded_flag, 
								(char *) header, slice_data);
			break;
		case SUPER_PETT_I_SCANNER:
		case SUPER_PETT_II_SCANNER:
		case SUPER_PETT_II_OTHERS:
			*err_value	= GetImageSuperPett (fd, slice_number, encoded_flag, 
								(char *) header, slice_data);
			break;
		default:
			*err_value	= FAIL;
			break;
	}

	*err_value	= SUCCEED;
}
