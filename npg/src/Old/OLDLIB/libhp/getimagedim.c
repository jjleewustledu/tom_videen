/*$Id: getimagedim.c,v 1.15 1995/10/30 22:39:01 tom Exp $*/
/*$Log: getimagedim.c,v $
 * Revision 1.15  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.14  1995/10/13  17:16:33  ty7777
 * Use matrix7.h.
 *
 * Revision 1.13  1995/07/11  16:40:18  ty7777
 * Added else if.
 *
 * Revision 1.12  1995/06/14  20:09:46  ty7777
 * Same as the last version.
 *
 * Revision 1.11  1995/06/13  19:18:45  ty7777
 * Deleted debug messages.
 *
 * Revision 1.10  1995/06/12  21:56:59  ty7777
 * Added processing for HDR file.
 *
 * Revision 1.9  1995/06/07  18:21:32  ty7777
 * Added code to free memories.
 *
 * Revision 1.8  1995/06/06  17:04:51  ty7777
 * Added processing for inter files.
 *
 * Revision 1.7  1994/03/03  21:22:48  ty7777
 * Replace : with ;.
 *
 * Revision 1.6  1994/03/03  20:14:49  ty7777
 * Replace id with header.
 *
 * Revision 1.5  1993/10/26  14:19:10  ty7777
 * Same as the last version.
 *
 * Revision 1.4  1993/08/06  16:20:44  ty7777
 * Modified error messages.
 *
 * Revision 1.3  1993/07/28  19:31:29  ty7777
 * Adding rcsid.
 *
 * Revision 1.2  1993/07/27  14:51:39  ty7777
 * Assign voxel_height at the end.
 *
 * Revision 1.1  1993/04/30  14:03:38  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		getimagedim.c

  Function:	int getimagedim (char *image_file, struct Matval matval, int *image_type,
			short *width, short *height, short *num_slices, float *voxel_width, 
			float *voxel_height, float *voxel_depth)

  Arguments:	
		image_file,	PET image file name, can be PETT VI or ECAT.
		matval,		structure containing number of frames, planes, etc., passed in.
		image_type,	image type, returned.
		width,		width of slices, returned.
		height,		height of slices, returned.
		num_slices,	number of slices in image, returned;
		voxel_width,	voxel width in centimeter, same as pixel size, returned.
		voxel_height,	voxel height in centimeter, same as pixel size, returned.
		voxel_depth,	voxel depth in centimeter, same as plane separation, returned.

  Description:	This function reads a PET image and gets the image width, height, and number of
		slices as well as voxel size.
  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang.

  History:	 Created by Tom Yang on 04/29/93. 
___________________________________________________________________________*/

#include <petutil.h>
#include <ifh.h>
#include <imglist.h>
#include <matrix7.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getimagedim.c,v 1.15 1995/10/30 22:39:01 tom Exp $";

PUBLIC int getimagedim (image_file, matval, image_type, width, height, num_slices, voxel_width,
		voxel_height, voxel_depth)
	char		*image_file;
	struct Matval	matval;
	int		*image_type;
	short		*width;
	short		*height;
	short		*num_slices;
	float		*voxel_width;
	float		*voxel_height;
	float		*voxel_depth;
{
	BOOLEAN		encoded_flag;
	IFH		ifhdr;
	MatrixFile	*fptr;
	char		*extension;
	char		ifhdr_file [MAXLINE];
	char		imgroot [MAXLINE];
	char		*lines [IFH_MAXLEN];
	int		fd;
	int		i;
	int		nlines;
	int		return_value;

	fd	= open (image_file, O_RDONLY, 0);
	if (fd == -1)
	{
		fprintf (stderr, "Error in opening image file %s.\n", image_file);
		return (FAIL);
	}

	*image_type	= getimage_type (fd, image_file, &encoded_flag, num_slices);
	close (fd);
	if (*image_type == ERROR_SCANNER)
	{
		fprintf (stderr, "Image file %s has the wrong image type.\n", image_file);
		return (FAIL);
	}

	/*
	 * Determine the image and voxel dimensions.
	 */
	return_value	= SUCCEED;
	if (*image_type == ECAT_SCANNER)
	{
		fptr	= OpenEcat (image_file, num_slices, width, height, 
				voxel_width, voxel_depth, matval);
		if (fptr == (MatrixFile *) NULL)
		{
			fprintf (stderr, "OpenEcat error in getimagedim ().\n");
			return (FAIL);
		}
		matrix_close (fptr);
		if (*voxel_depth == 0.0)
			*voxel_depth	= DEFAULT_ECAT_SEP;
	}
	else if (*image_type == INTERFILE)
	{
		/* 
 	 	 * Create the interfile header file.
 	 	 */
		memset (ifhdr_file, 0, MAXLINE); 
		extension	= strrchr (image_file, '.');
		if (extension != NULL)
			strncpy (ifhdr_file, image_file, strlen (image_file)  - strlen (extension));
		else
			strcpy (ifhdr_file, image_file);
		strcpy (imgroot, ifhdr_file);
		strcat (ifhdr_file, IFH_EXT);

		if (readifhdr (ifhdr_file, FALSE, IFH_MAXLEN, &nlines, lines) == FAIL
		|| getifhdr (ifhdr_file, FALSE, nlines, lines, &ifhdr) == FAIL)
		{
			strcpy (ifhdr_file, imgroot);
			strcat (ifhdr_file, HDR_EXT);

			return_value	= hdr2ifh (ifhdr_file, TRUE, &ifhdr);

			if (return_value == FAIL)
				return FAIL;
		}

		*width		= ifhdr.matrix_size [0];
		*height		= ifhdr.matrix_size [1];
		*num_slices	= ifhdr.matrix_size [2];
		*voxel_width	= ifhdr.scaling_factor [0] / MMS_PER_CM;
		*voxel_height	= ifhdr.scaling_factor [1] / MMS_PER_CM;
		*voxel_depth	= ifhdr.scaling_factor [2] / MMS_PER_CM;
	}
	else
	{
		*width	= ENCODED_SLICE_WIDTH;
		*height	= ENCODED_SLICE_HEIGHT;
		if (*num_slices == PETT6_7SLICES)
		{
			*voxel_width	= PETT6_PIXEL_SIZE;
			*voxel_depth	= PETT6_SEPARATION;
		}
		else if (*num_slices == PETT6_14)
		{
			*voxel_width	= PETT6_PIXEL_SIZE;
			*voxel_depth	= PETT6_SEPARATION / 2;
		}
		else 
		{
			*voxel_width	= SMG_PIXEL_SIZE;
			*voxel_depth	= SMG_SEPARATION;
		}
	}

	*voxel_height	= *voxel_width;

	return return_value;
}
