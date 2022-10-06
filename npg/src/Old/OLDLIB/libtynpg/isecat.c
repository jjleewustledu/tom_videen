/*$Id: isecat.c,v 1.1 1996/04/30 19:37:20 ty7777 Exp $*/
/*$Log: isecat.c,v $
 * Revision 1.1  1996/04/30  19:37:20  ty7777
 * Initial revision
 *
 * Revision 1.12  1996/04/03  17:58:56  ty7777
 * Save before making any further changes.
 *
 * Revision 1.11  1996/01/17  18:11:11  ty7777
 * Add fclose ().
 *
 * Revision 1.10  1995/11/04  00:00:53  ty7777
 * Fix the bug.
 *
 * Revision 1.9  1995/11/03  23:00:52  ty7777
 * Added image_type.
 *
 * Revision 1.8  1995/11/03  22:56:40  ty7777
 * Added system_type.
 *
 * Revision 1.7  1995/10/30  20:16:21  ty7777
 * Use SYSTEM_TYPE_PETT6.
 *
 * Revision 1.6  1995/10/27  16:32:41  ty7777
 * Use OpenEcat () to get the total number of slices.
 *
 * Revision 1.5  1995/10/17  20:37:25  ty7777
 * Updated for freeing memories.
 *
 * Revision 1.4  1995/09/28  18:35:15  ty7777
 * Updated for 961 data.
 *
 * Revision 1.3  1995/06/22  20:55:59  ty7777
 * Added processing for denove images.
 *
 * Revision 1.2  1995/06/16  19:04:06  ty7777
 * Added OLD_NPG_SYSTEM_TYPE.
 *
 * Revision 1.1  1995/06/14  20:11:23  ty7777
 * Initial revision
 **/

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/isecat.c,v 1.1 1996/04/30 19:37:20 ty7777 Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	isecat.c
 *
 *	Function Name:	int isecat (char *filename, int *image_type, short *num_slices)
 *
 *	Arguments:	filename:		image file name.
 *
 *	Return Value:	TRUE:	image is filename is in ECAT format.
 *			FALSE:	image is not in ECAT format.	
 *
 *	Description:	Read the main header of filename and determine if it is in
 *			ECAT format.
 *
 *	Called by:	getimage_type.c	
 *			
 *	Calling:	Functions in libcti.a.	
 *
 *	Author:		Tom Yang
 *
 *	History:	Created by Tom Yang on 06/14/1995.
 *
___________________________________________________________________________________*/

#include <petutil/petutil.h>
#include <petutil/matrix_64.h>

int isecat (filename, image_type, num_slices)
char	*filename;
int	*image_type;
short	*num_slices;
{
	FILE		*fptr; 
	Main_header	main_header;
	MatrixFile	*matfptr;		/* ECAT MatrixFile pointer */
	float		pixel_size;
	float		plane_separation;
	int		return_value;
	short		xdim;			/* dimension-x */
	short		ydim;			/* dimension-y */
	struct Matval	matval;

	return_value	= FALSE;
	matfptr		= NULL;
	fptr		= mat_open (filename, "rb");
	if (fptr != NULL)
	{
		mat_read_main_header (fptr, &main_header);
		mat_close (fptr);

		if (main_header.system_type == SYSTEM_TYPE_953 
		|| main_header.system_type == SYSTEM_TYPE_961 
		|| main_header.system_type == 0 
		|| main_header.system_type == 1 
		|| main_header.system_type == SYSTEM_TYPE_PETT6) 
		{

			matval.frame	= 1;
			matval.plane	= 1;
			matval.gate	= 1;
			matval.data	= 0;
			matval.bed	= 0;

			if ((matfptr = OpenEcat (filename, num_slices, &xdim, &ydim, 
			&pixel_size, &plane_separation, matval)) != NULL)
			{
				if (main_header.system_type == SYSTEM_TYPE_961)
					*image_type	= ECAT961_SCANNER;	
				else
					*image_type	= ECAT953_SCANNER;	
				return_value	= TRUE;
				matrix_close (matfptr);
			}
			else
			{
				return_value	= FALSE;
			}
		}
	}

	return return_value;
}
