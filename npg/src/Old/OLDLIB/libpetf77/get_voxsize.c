/*$Id: get_voxsize.c,v 1.3 1995/10/31 14:54:23 tom Exp $*/
/*$Log: get_voxsize.c,v $
 * Revision 1.3  1995/10/31  14:54:23  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1995/10/13  16:19:15  ty7777
 * Use matrix7.h
 *
 * Revision 1.1  1995/10/10  15:32:48  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)getimage_type.c 10/09/92 Copyright Neural Pet Group, Washington University 1992";
#endif

/*___________________________________________________________________________________
 File Name:	get_voxsize.c

 Function Name:	int get_voxsize (int fd, char *image_name, cmppix [])

 Arguments:	fd:		File descriptor, assigned before calling.	
 		image_name:	Image name.
		cmppix:		Array holding pixel width, pixel height and
				pixel depth, returned.

 Return Value:	FAIL: failed; SUCCEED: success.

 Description:	Get the voxel size of an ECAT image.

 Calling:	OpenEcat () in OpenEcat.c of libpetutil.a	

 Author:	Tom Yang

 History:	Created by Tom Yang on 01/18/1992.
___________________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

static char rcsid []= "$Header: /home/npggw/tom/src/libpetf77/RCS/get_voxsize.c,v 1.3 1995/10/31 14:54:23 tom Exp $";

int get_voxsize (fd, image_name, cmppix)
int		fd;
char		*image_name;
float		cmppix [3];
{
	MatrixFile	*matfptr;		/* ECAT MatrixFile pointer */
	float		pixel_size;
	float		plane_separation;
	short		num_slices;
	short		xdim;			/* dimension-x */
	short		ydim;			/* dimension-y */
	struct Matval	matval;

	/*
	 * Assign default values for matval.
	 */
	matval.frame	= 1;
	matval.plane	= 1;
	matval.gate	= 1;
	matval.data	= 0;
	matval.bed	= 0;

	if ((matfptr = OpenEcat (fd, image_name, &num_slices, &xdim, &ydim, 
					&pixel_size, &plane_separation, matval)) != NULL)
	{
		cmppix [0]	= pixel_size;
		cmppix [1]	= pixel_size;
		if (pkg_abs (plane_separation) > ERROR_BOUND) 
			cmppix [2]	= plane_separation;
		else
		{
			if (matfptr->mhptr->system_type == SYSTEM_TYPE_961)
				cmppix [2]	= DEFAULT_ECAT961_SEP;
			else
				cmppix [2]	= DEFAULT_ECAT_SEP;
		}
		matrix_close (matfptr);
	}
	else
	{
		fprintf (stderr, "ECAT image opening error in get_voxsize ().\n");
		return;
	}
}
