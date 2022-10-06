/*$Id: get_voxsize.c,v 1.11 1995/11/10 16:28:14 tom Exp $*/
/*$Log: get_voxsize.c,v $
 * Revision 1.11  1995/11/10  16:28:14  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.10  1995/11/03  23:12:10  ty7777
 * Added ECAT961_SCANNER processing.
 *
 * Revision 1.9  1995/10/13  17:15:11  ty7777
 * Use matrix7.h
 *
 * Revision 1.8  1994/03/03  20:07:08  ty7777
 * Replace id with header.
 *
 * Revision 1.7  1994/02/25  18:02:49  ty7777
 * Changed the error message.
 *
 * Revision 1.6  1994/02/25  18:01:40  ty7777
 * Changed the error message.
 *
 * Revision 1.5  1994/02/25  17:55:53  ty7777
 * Calling getimagedim ().
 *
 * Revision 1.4  1994/02/25  17:09:09  ty7777
 * Changed the function prototype.
 *
 * Revision 1.3  1994/02/25  16:28:33  ty7777
 * Deleted the fd file descriptor.
 *
 * Revision 1.2  1993/11/03  17:55:33  ty7777
 * Deleted ssccsid.
 *
 * Revision 1.1  1993/03/03  20:34:06  ty7777
 * Initial revision
 **/

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/get_voxsize.c,v 1.11 1995/11/10 16:28:14 tom Exp $";

/*___________________________________________________________________________________
  
  	File Name:	get_voxsize.c

  	Function Name:	int get_voxsize (image_name, cmpix)

  	Arguments:	image_name:	Image name.
  			cmppix:		Array holding pixel width, pixel height and
  					pixel depth in cm, returned.

  	Return Value:	SUCCEED, successful; FAIL, something is wrong.

  	Description:	Get the voxel size of an ECAT image.

  	Calling:	OpenEcat () in OpenEcat.c of libpetutil.a	

  	Author:		Tom Yang.

  	History:	Created by Tom Yang on 01/18/1992.
  
___________________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

PUBLIC int get_voxsize (image_name, cmppix)
char	*image_name;
float	*cmppix;
{
	int		scanner;
	short		height;
	short		num_slices;
	short		width;
	struct Matval	matval;

	/*
	 * Assign default values for matval.
	 */
	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	if (getimagedim (image_name, matval, &scanner, &width, &height, &num_slices, 
	cmppix, cmppix + 1, cmppix + 2) == FAIL) 
	{
		fprintf (stderr, "Error in getimagedim () from reading %s.\n", image_name);
		return FAIL;
	}

	if (pkg_abs (cmppix [2]) < ERROR_BOUND)
	{
		if (scanner == ECAT953_SCANNER)
			cmppix [2]	= DEFAULT_ECAT_SEP;
		else
			cmppix [2]	= DEFAULT_ECAT961_SEP;
	}

	return SUCCEED;
}
